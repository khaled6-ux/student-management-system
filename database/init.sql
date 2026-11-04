-- SQLite schema reference.
-- The C++ API creates this schema automatically on first launch.
CREATE TABLE IF NOT EXISTS students (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL CHECK(length(trim(name)) BETWEEN 1 AND 100),
    age INTEGER NOT NULL CHECK(age BETWEEN 1 AND 120),
    major TEXT NOT NULL CHECK(length(trim(major)) BETWEEN 1 AND 100),
    gpa REAL NOT NULL CHECK(gpa >= 0.0 AND gpa <= 4.0),
    created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX IF NOT EXISTS idx_students_name ON students(lower(name));
CREATE INDEX IF NOT EXISTS idx_students_major ON students(lower(major));
