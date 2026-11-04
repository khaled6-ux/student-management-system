#include "student_repository.hpp"
#include <pqxx/pqxx>
#include <cstdlib>
#include <stdexcept>
namespace { std::string db(){const char* p=std::getenv("DATABASE_URL"); if(!p||!*p) throw std::runtime_error("DATABASE_URL is not configured"); return p;} Student read(const pqxx::row&r){return {r["id"].as<int>(),r["name"].as<std::string>(),r["age"].as<int>(),r["major"].as<std::string>(),r["gpa"].as<double>()};}}
std::vector<Student> StudentRepository::list()const{pqxx::connection c(db());pqxx::read_transaction t(c);auto rows=t.exec("SELECT id,name,age,major,gpa FROM students ORDER BY id DESC");std::vector<Student> out;for(auto const&r:rows)out.push_back(read(r));return out;}
std::optional<Student> StudentRepository::get(int id)const{pqxx::connection c(db());pqxx::read_transaction t(c);auto r=t.exec_params("SELECT id,name,age,major,gpa FROM students WHERE id=$1",id);if(r.empty())return {};return read(r[0]);}
Student StudentRepository::create(const Student&s)const{pqxx::connection c(db());pqxx::work t(c);auto r=t.exec_params("INSERT INTO students(name,age,major,gpa) VALUES($1,$2,$3,$4) RETURNING id,name,age,major,gpa",s.name,s.age,s.major,s.gpa);t.commit();return read(r[0]);}
bool StudentRepository::update(int id,const Student&s)const{pqxx::connection c(db());pqxx::work t(c);auto r=t.exec_params("UPDATE students SET name=$1,age=$2,major=$3,gpa=$4,updated_at=NOW() WHERE id=$5",s.name,s.age,s.major,s.gpa,id);t.commit();return r.affected_rows()==1;}
bool StudentRepository::remove(int id)const{pqxx::connection c(db());pqxx::work t(c);auto r=t.exec_params("DELETE FROM students WHERE id=$1",id);t.commit();return r.affected_rows()==1;}
