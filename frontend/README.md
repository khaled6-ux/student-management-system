# CampusDesk frontend

React + TypeScript + Vite local interface for the C++ Student Management API.

From the repository root, use `.\scripts\run-local.ps1` to build and start both frontend and backend. The frontend is available at http://127.0.0.1:5173 and calls the API at http://127.0.0.1:8080.

For frontend-only work:

```powershell
npm install
npm run dev -- --host 127.0.0.1
```

The frontend expects `GET /api/health`, `GET /api/students`, `POST /api/students`, `PUT /api/students/:id`, and `DELETE /api/students/:id`.

Student GPA is modeled on a 0.00–4.00 scale.
