# Student Management System — Rebuild

This branch begins the full-stack modernization while preserving the original Visual Studio C++ console project in `PROJECT 1/`.

## Target stack
- C++20 + Crow HTTP API
- libpqxx + PostgreSQL
- React + TypeScript + Vite frontend (next implementation step)

## Backend endpoints
- GET /api/health
- GET /api/students
- POST /api/students
- GET /api/students/{id}
- PUT /api/students/{id}
- DELETE /api/students/{id}

## Local database
Install Docker Desktop and run `docker compose up -d database`. The schema is in `database/init.sql`.

## C++ dependencies
Install CMake and vcpkg, then install Crow and libpqxx using the manifest. Configure CMake with the vcpkg toolchain. Set DATABASE_URL, CORS_ORIGIN and PORT before starting the API.

## Important
This is an early development branch, not a production-ready application. The frontend, authentication, authorization, automated integration tests, migration tooling, and deployment hardening are not complete. Do not expose it publicly or store real student records yet. Confirm the institution's GPA scale before importing legacy records.

See [Phase 1 audit](docs/phase-1-audit.md) and [architecture](docs/architecture.md).
