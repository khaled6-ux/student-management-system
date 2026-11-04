# Phase 1 — Repository audit

## Baseline
The original repository is a small Visual Studio C++20 console application. It contains Student and studentManager classes, a menu-driven main program, and pipe-delimited students.txt persistence. There is no HTTP API, frontend, relational database, or automated test suite in the original baseline.

## Existing capabilities
- Add, list, search, update, and delete students
- Sort by ID, name, grade, and age
- Calculate count, average, highest, and lowest grade
- Load/save records from a text file

## Findings
1. Console I/O and business operations are coupled in studentManager, so the class cannot be reused cleanly from an HTTP API.
2. updateStudent does not consistently validate age and grade; search/update/delete input extraction is not robust against invalid input.
3. loadFromFile uses unchecked delimiters and stoi/stod; malformed lines can throw and stop startup.
4. Pipe-delimited persistence cannot safely represent values containing the delimiter.
5. The sample file has grades 3.1 and 3.0 while validation accepts 0–100. Version 2 must confirm the grading convention before importing real records.
6. The Visual Studio project is the only build definition; a cross-platform CMake build and automated tests are needed.

## Target architecture
React + TypeScript + Vite frontend -> C++20 HTTP API -> service layer -> repository layer -> PostgreSQL. Crow and libpqxx are proposed dependencies; confirm their Windows toolchain compatibility in CI and local builds before treating the backend as release-ready.

## Safety and scope
Keep the original console project and Git history intact. Develop on feat/fullstack-rebuild. Do not overwrite main. Do not expose student data or a public API until authentication, authorization, TLS, rate limiting, audit logging, backup/restore, and production configuration are implemented and reviewed.

## Decisions still requiring confirmation
- Grade scale: the sample suggests a 4.0 GPA but this is not confirmed.
- Authentication: administrator-only, role-based accounts, or no login for a local demo.
- Migration: validate and back up students.txt before importing; do not silently reinterpret grades.

## Validation status
This audit is based on static inspection of the default branch source files. No local compiler, database, browser, or production integration tests have been run as part of this audit.