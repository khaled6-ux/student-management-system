# Student Management System — Local Edition

A beginner-friendly local student management application. It keeps the original C++ backend and adds a React + TypeScript browser interface. Everything runs on your Windows computer.

## Stack

- Frontend: React, TypeScript, Vite
- Backend: C++20, Crow REST API
- Database: SQLite (one local file; no database server)
- Build: CMake and vcpkg

No login, registration, PostgreSQL, Docker, hosting, or cloud account is required. Once dependencies are installed, the app works offline.

## Features

- Add, view, search, edit, and delete student records
- Filter by major and sort by newest, name, GPA, or age
- Dashboard statistics: total students, average GPA, highest GPA, and number of majors
- Local persistence: records stay in `data/students.db` after closing the app

## Windows setup

### One-time prerequisites

Install:
1. Git
2. CMake 3.25 or later
3. Visual Studio 2022 with **Desktop development with C++**
4. Node.js 20 or later
5. vcpkg, with its `VCPKG_ROOT` environment variable pointing to the vcpkg folder

The CMake configuration uses vcpkg to install Crow and SQLite3.

### Run the application

Open PowerShell in the repository folder and run:

```powershell
.\scripts\run-local.ps1
```

On first run, the script configures and builds the C++ API, installs frontend packages if needed, starts both local processes, and opens the browser at http://127.0.0.1:5173.

Keep the PowerShell window open while using the application. Press Ctrl+C to stop the local processes.

### Data and backups

The database is stored at `data/students.db`. Back it up by copying that file while the application is stopped. Do not commit the database file to GitHub.

To use another database location, set `STUDENT_DB_PATH` before launching the API.

## API

- `GET /api/health`
- `GET /api/students`
- `POST /api/students`
- `GET /api/students/{id}`
- `PUT /api/students/{id}`
- `DELETE /api/students/{id}`

The API binds to the loopback interface (127.0.0.1), so it is not exposed as a public web service.

## Tests

```powershell
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

GitHub Actions also compiles the C++ API, runs validation and SQLite repository tests, starts the local API with a temporary SQLite database, and exercises HTTP CRUD operations.

## Grading scale

The current app validates GPA on a 0.00–4.00 scale. Confirm that this matches the grading system you want before importing legacy records.
