#include "student_repository.hpp"
#include "student_service.hpp"

#include <crow.h>

// Windows headers define DELETE as a macro for access rights.
// That macro conflicts with Crow's crow::HTTPMethod::DELETE enum constant.
#ifdef DELETE
#undef DELETE
#endif

#include <cstdint>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {
std::string environment(const char* name, const std::string& fallback) {
    const char* value = std::getenv(name);
    return value == nullptr || std::string(value).empty() ? fallback : value;
}

void add_cors(crow::response& response) {
    response.set_header("Access-Control-Allow-Origin", environment("CORS_ORIGIN", "http://127.0.0.1:5173"));
    response.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    response.set_header("Access-Control-Allow-Headers", "Content-Type");
    response.set_header("Vary", "Origin");
}

crow::response json_response(int status, crow::json::wvalue body) {
    crow::response response(status, body.dump());
    response.set_header("Content-Type", "application/json; charset=utf-8");
    add_cors(response);
    return response;
}

crow::response error_response(int status, const std::string& message) {
    crow::json::wvalue body;
    body["error"] = message;
    return json_response(status, std::move(body));
}

crow::json::wvalue encode_student(const Student& student) {
    crow::json::wvalue body;
    body["id"] = student.id;
    body["name"] = student.name;
    body["age"] = student.age;
    body["major"] = student.major;
    body["gpa"] = student.gpa;
    return body;
}

Student decode_student(const crow::json::rvalue& body) {
    if (!body || !body.has("name") || !body.has("age") ||
        !body.has("major") || !body.has("gpa")) {
        throw std::invalid_argument("Required fields: name, age, major and gpa.");
    }

    Student student;
    student.name = body["name"].s();
    student.age = body["age"].i();
    student.major = body["major"].s();
    student.gpa = body["gpa"].d();
    return normalize_student(std::move(student));
}

crow::response database_error(const std::exception& error) {
    std::cerr << "[student-api] " << error.what() << '\\n';
    return error_response(500, "The local database request failed. Check the API logs.");
}
} // namespace

int main() {
    try {
        crow::SimpleApp app;
        StudentRepository repository;

        CROW_ROUTE(app, "/api/health")
        ([&repository]() {
            try {
                repository.health();
                crow::json::wvalue body;
                body["status"] = "ok";
                body["database"] = "sqlite";
                return json_response(200, std::move(body));
            } catch (const std::exception& error) {
                return database_error(error);
            }
        });

        CROW_ROUTE(app, "/api/students").methods(crow::HTTPMethod::GET)
        ([&repository]() {
            try {
                const auto students = repository.list();
                crow::json::wvalue::list data;
                data.reserve(students.size());
                for (const auto& student : students) data.emplace_back(encode_student(student));
                crow::json::wvalue body;
                body["data"] = std::move(data);
                body["count"] = static_cast<int>(students.size());
                return json_response(200, std::move(body));
            } catch (const std::exception& error) {
                return database_error(error);
            }
        });

        CROW_ROUTE(app, "/api/students").methods(crow::HTTPMethod::POST)
        ([&repository](const crow::request& request) {
            try {
                const auto body = crow::json::load(request.body);
                if (!body) return error_response(400, "Request body must contain valid JSON.");
                Student student = decode_student(body);
                if (const auto problem = validate_student(student)) return error_response(422, *problem);
                return json_response(201, encode_student(repository.create(student)));
            } catch (const std::invalid_argument& error) {
                return error_response(400, error.what());
            } catch (const std::exception& error) {
                return database_error(error);
            }
        });

        CROW_ROUTE(app, "/api/students/<int>").methods(crow::HTTPMethod::GET)
        ([&repository](int id) {
            try {
                const auto student = repository.get(id);
                if (!student) return error_response(404, "Student not found.");
                return json_response(200, encode_student(*student));
            } catch (const std::exception& error) {
                return database_error(error);
            }
        });

        CROW_ROUTE(app, "/api/students/<int>").methods(crow::HTTPMethod::PUT)
        ([&repository](const crow::request& request, int id) {
            try {
                const auto body = crow::json::load(request.body);
                if (!body) return error_response(400, "Request body must contain valid JSON.");
                Student student = decode_student(body);
                if (const auto problem = validate_student(student)) return error_response(422, *problem);
                if (!repository.update(id, student)) return error_response(404, "Student not found.");
                const auto updated = repository.get(id);
                if (!updated) return error_response(404, "Student not found.");
                return json_response(200, encode_student(*updated));
            } catch (const std::invalid_argument& error) {
                return error_response(400, error.what());
            } catch (const std::exception& error) {
                return database_error(error);
            }
        });

        CROW_ROUTE(app, "/api/students/<int>").methods(crow::HTTPMethod::DELETE)
        ([&repository](int id) {
            try {
                if (!repository.remove(id)) return error_response(404, "Student not found.");
                crow::json::wvalue body;
                body["message"] = "Student deleted.";
                return json_response(200, std::move(body));
            } catch (const std::exception& error) {
                return database_error(error);
            }
        });

        CROW_ROUTE(app, "/api/<path>").methods(crow::HTTPMethod::OPTIONS)
        ([](const crow::request&, const std::string&) {
            crow::response response(204);
            add_cors(response);
            return response;
        });

        const int port = std::stoi(environment("PORT", "8080"));
        std::cout << "Student Management API listening on http://127.0.0.1:" << port << '\\n';
        app.bindaddr("127.0.0.1").port(static_cast<std::uint16_t>(port)).multithreaded().run();
    } catch (const std::exception& error) {
        std::cerr << "Could not start Student Management API: " << error.what() << '\\n';
        return 1;
    }
    return 0;
}
