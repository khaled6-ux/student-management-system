# Local backend and SQLite

The C++ API owns SQLite initialization. On startup it opens the file configured by `STUDENT_DB_PATH`, or defaults to `data/students.db`, creates the parent directory, enables foreign keys, sets a five-second busy timeout, and creates the student table and indexes if they do not exist.

No PostgreSQL service or database installation is needed.

## Tests

- `student-core-tests`: service validation and normalization.
- `student-repository-tests`: SQLite repository create/read/update/delete and persistence behavior using a temporary database file.
- `backend/tests/api_integration.py`: HTTP health, list, create, read, update, validation, delete, and not-found behavior against a running local API.

Run C++ tests with:

```powershell
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Run the HTTP test after starting `student-api` with `STUDENT_DB_PATH` set to a temporary local file:

```powershell
$env:STUDENT_DB_PATH = "$env:TEMP\student-management-test.db"
python backend/tests/api_integration.py
```
