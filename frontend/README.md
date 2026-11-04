# CampusDesk frontend

React + TypeScript + Vite interface for the C++ Student Management API.

## Run locally

1. Install Node.js 20 or later.
2. From this directory, install dependencies:

   ```bash
   npm install
   ```

3. Copy `.env.example` to `.env` and check the API URL:

   ```env
   VITE_API_BASE_URL=http://localhost:8080
   ```

4. Start the development server:

   ```bash
   npm run dev
   ```

5. Open http://localhost:5173.

## Production build

```bash
npm run build
npm run preview
```

The frontend expects these API routes:

- `GET /api/health`
- `GET /api/students`
- `POST /api/students`
- `PUT /api/students/:id`
- `DELETE /api/students/:id`

Student GPA is currently modeled on a 0.00–4.00 scale. Confirm this matches the grading scale you want before importing real records.
