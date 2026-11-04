# Local Edition architecture

- Frontend: React + TypeScript + Vite, served locally by Vite during development.
- HTTP API: C++20 + Crow, bound only to 127.0.0.1.
- Service layer: validation and normalization independent of HTTP and persistence.
- Repository layer: SQLite C API with prepared statements.
- Storage: one local file, `data/students.db`, created automatically on first launch.

No accounts, hosted services, database server, or deployment target are part of this beginner-focused edition.

## API
| Method | Endpoint | Purpose |
|---|---|---|
| GET | /api/health | API and local database readiness |
| GET | /api/students | List students |
| POST | /api/students | Create student |
| GET | /api/students/{id} | Read one student |
| PUT | /api/students/{id} | Update student |
| DELETE | /api/students/{id} | Delete student |
