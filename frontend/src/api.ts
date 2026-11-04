import type { Student, StudentInput } from "./types";

const API_BASE = (import.meta.env.VITE_API_BASE_URL || "http://localhost:8080").replace(/\/$/, "");

async function request<T>(path: string, init?: RequestInit): Promise<T> {
  let response: Response;
  try {
    response = await fetch(API_BASE + path, {
      ...init,
      headers: {
        Accept: "application/json",
        ...(init?.body ? { "Content-Type": "application/json" } : {}),
        ...init?.headers,
      },
    });
  } catch {
    throw new Error("Cannot connect to the C++ API. Check that the backend is running.");
  }

  const payload: unknown = await response.json().catch(() => null);
  if (!response.ok) {
    const message =
      payload && typeof payload === "object" && "error" in payload && typeof payload.error === "string"
        ? payload.error
        : `Request failed with status ${response.status}.`;
    throw new Error(message);
  }
  return payload as T;
}

export async function listStudents(): Promise<Student[]> {
  const result = await request<{ data: Student[] }>("/api/students");
  return Array.isArray(result.data) ? result.data : [];
}

export function createStudent(input: StudentInput): Promise<Student> {
  return request<Student>("/api/students", {
    method: "POST",
    body: JSON.stringify(input),
  });
}

export function updateStudent(id: number, input: StudentInput): Promise<Student> {
  return request<Student>(`/api/students/${id}`, {
    method: "PUT",
    body: JSON.stringify(input),
  });
}

export function removeStudent(id: number): Promise<{ message: string }> {
  return request<{ message: string }>(`/api/students/${id}`, {
    method: "DELETE",
  });
}

export async function checkApi(): Promise<boolean> {
  try {
    const result = await request<{ status: string }>("/api/health");
    return result.status === "ok";
  } catch {
    return false;
  }
}
