#!/usr/bin/env python3
"""Black-box CRUD smoke tests for a running student-api instance."""

import json
import os
import sys
import urllib.error
import urllib.request

BASE_URL = os.environ.get("API_BASE_URL", "http://127.0.0.1:8080").rstrip("/")


def request(method, path, payload=None, expected_status=200):
    data = None if payload is None else json.dumps(payload).encode("utf-8")
    headers = {"Accept": "application/json"}
    if data is not None:
        headers["Content-Type"] = "application/json"
    req = urllib.request.Request(BASE_URL + path, data=data, headers=headers, method=method)
    try:
        with urllib.request.urlopen(req, timeout=5) as response:
            status = response.status
            raw = response.read()
    except urllib.error.HTTPError as error:
        status = error.code
        raw = error.read()

    if status != expected_status:
        raise AssertionError(
            f"{method} {path}: expected HTTP {expected_status}, got {status}: "
            f"{raw.decode('utf-8', errors='replace')}"
        )
    if not raw:
        return {}
    return json.loads(raw.decode("utf-8"))


def main():
    health = request("GET", "/api/health")
    assert health.get("status") == "ok", health
    assert health.get("database") == "connected", health

    before = request("GET", "/api/students")
    assert isinstance(before.get("data"), list), before
    assert before.get("count") == len(before["data"]), before

    created = request(
        "POST",
        "/api/students",
        {"name": "Integration Test Student", "age": 21, "major": "Computer Science", "gpa": 3.75},
        expected_status=201,
    )
    student_id = created.get("id")
    assert isinstance(student_id, int) and student_id > 0, created
    assert created.get("name") == "Integration Test Student", created

    fetched = request("GET", f"/api/students/{student_id}")
    assert fetched.get("id") == student_id, fetched

    updated = request(
        "PUT",
        f"/api/students/{student_id}",
        {"name": "Updated Integration Student", "age": 22, "major": "Software Engineering", "gpa": 3.9},
    )
    assert updated.get("name") == "Updated Integration Student", updated
    assert updated.get("gpa") == 3.9, updated

    invalid = request(
        "POST",
        "/api/students",
        {"name": "Invalid GPA", "age": 20, "major": "Computer Science", "gpa": 4.5},
        expected_status=422,
    )
    assert "error" in invalid, invalid

    missing_fields = request(
        "POST",
        "/api/students",
        {"name": "Incomplete"},
        expected_status=400,
    )
    assert "error" in missing_fields, missing_fields

    request("DELETE", f"/api/students/{student_id}")
    request("GET", f"/api/students/{student_id}", expected_status=404)
    request("DELETE", f"/api/students/{student_id}", expected_status=404)

    print("API integration tests passed: health, list, create, read, update, validation, delete, and not-found.")


if __name__ == "__main__":
    try:
        main()
    except Exception as error:  # Report a useful CI failure.
        print(f"API integration tests failed: {error}", file=sys.stderr)
        raise
