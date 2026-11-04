# Backend and database integration

The C++ API is tested at two levels:

1. `student-core-tests` validates service-layer input normalization and GPA/age/name rules without a database.
2. `backend/tests/api_integration.py` performs HTTP-level health, list, create, read, update, validation, delete, and not-found checks against a running API backed by PostgreSQL.

Run the full test sequence locally after starting PostgreSQL and building the API:

```bash
psql "$DATABASE_URL" -v ON_ERROR_STOP=1 -f database/init.sql
ctest --test-dir build --output-on-failure
./build/student-api
```

In a second terminal:

```bash
python3 backend/tests/api_integration.py
```

GitHub Actions runs the same checks against a disposable PostgreSQL 16 service for relevant pushes and pull requests.
