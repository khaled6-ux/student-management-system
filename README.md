# Student Management System

A full-stack modernization of the original C++ student management coursework project. The original Visual Studio console application is preserved in `PROJECT 1/`.

## Stack

- **Frontend:** React, TypeScript, Vite
- **Backend:** C++20, Crow HTTP framework
- **Database:** PostgreSQL, accessed with libpqxx
- **Build:** CMake and vcpkg

## Current application features

- Student directory with search by name, ID, or major
- Filter by major and sort by newest, name, GPA, or age
- Dashboard statistics: total students, average GPA, highest GPA, and number of majors
- Create, edit, and delete student records
- Responsive layout for desktop, tablet, and mobile
- API connection indicator and error/empty/loading states

## Run locally

### 1. Start PostgreSQL

Install Docker Desktop, then from the repository root run:

```bash
docker compose up -d database
```

The schema is initialized from `database/init.sql`.

### 2. Build and run the C++ API

Install CMake, a C++20 compiler, and vcpkg. The vcpkg manifest installs Crow and libpqxx.

Configure and build:

```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

Set the database connection string and API port before running the executable:

```text
DATABASE_URL=postgresql://postgres:postgres@localhost:5432/student_manager
PORT=8080
CORS_ORIGIN=http://localhost:5173
```

On Windows PowerShell, set these with `$env:DATABASE_URL = "..."`, `$env:PORT = "8080"`, and `$env:CORS_ORIGIN = "http://localhost:5173"`.

### 3. Run the React frontend

```bash
cd frontend
npm install
```

Copy `.env.example` to `.env`, then start Vite:

```bash
npm run dev
```

Open http://localhost:5173.

## API

| Method | Endpoint | Description |
| --- | --- | --- |
| GET | `/api/health` | API and database readiness |
| GET | `/api/students` | List students |
| POST | `/api/students` | Create a student |
| GET | `/api/students/{id}` | Get one student |
| PUT | `/api/students/{id}` | Update a student |
| DELETE | `/api/students/{id}` | Delete a student |

## Tests and CI

- C++ service validation tests run through CTest.
- API integration tests exercise CRUD and validation against PostgreSQL.
- GitHub Actions runs the C++ build, unit tests, PostgreSQL-backed API integration tests, and frontend TypeScript build.

See [backend integration testing](docs/backend-integration.md), [the repository audit](docs/phase-1-audit.md), [architecture notes](docs/architecture.md), and [frontend setup](frontend/README.md).

## Important deployment and data notes

This is a development-stage application, not a production-ready student information system. Authentication, role-based authorization, audit logging, production secret management, TLS, rate limiting, and operational backup/restore still need implementation before public deployment or use with real student records.

The original console application accepts a grade from 0 to 100, while its sample records contain values such as 3.1 and 3.0. The rebuilt database currently treats this field as a 0.00–4.00 GPA. Confirm the intended grading scale and validate any legacy data before migration.
