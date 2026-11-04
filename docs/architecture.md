# Proposed Version 2 architecture

- Frontend: React, TypeScript, Vite.
- HTTP API: C++20 and Crow.
- Service layer: validation and business rules independent of HTTP and persistence.
- Repository layer: libpqxx with parameterized SQL.
- Database: PostgreSQL with constraints, indexes, and migration scripts.

## Proposed endpoints
| Method | Endpoint | Purpose |
|---|---|---|
| GET | /api/health | API/database readiness |
| GET | /api/students | List students |
| POST | /api/students | Create student |
| GET | /api/students/{id} | Read one student |
| PUT | /api/students/{id} | Update student |
| DELETE | /api/students/{id} | Delete student |
| GET | /api/stats | GPA statistics |

## Release boundary
The first implementation should be local-development only. CORS is not authentication. Do not expose the API publicly before implementing authentication, authorization, TLS, rate limits, secure secret management, audit logging, and operational backups.