import { useCallback, useEffect, useMemo, useState } from "react";
import type { FormEvent, ReactNode } from "react";
import {
  Activity,
  ArrowDownUp,
  BookOpen,
  Check,
  ChevronDown,
  CircleHelp,
  GraduationCap,
  MoreHorizontal,
  Pencil,
  Plus,
  RefreshCw,
  Search,
  SlidersHorizontal,
  Sparkles,
  Trash2,
  Users,
  X,
} from "lucide-react";
import { checkApi, createStudent, listStudents, removeStudent, updateStudent } from "./api";
import type { Student, StudentInput } from "./types";

type FormValues = { name: string; age: string; major: string; gpa: string };
type SortKey = "newest" | "name" | "gpa" | "age";

const blankForm: FormValues = { name: "", age: "", major: "", gpa: "" };

function initials(name: string): string {
  return name.trim().split(/\s+/).slice(0, 2).map((part) => part[0]?.toUpperCase() ?? "").join("");
}

function formatGpa(value: number): string {
  return Number(value).toFixed(2);
}

function App() {
  const [students, setStudents] = useState<Student[]>([]);
  const [loading, setLoading] = useState(true);
  const [refreshing, setRefreshing] = useState(false);
  const [apiOnline, setApiOnline] = useState(false);
  const [loadError, setLoadError] = useState("");
  const [query, setQuery] = useState("");
  const [majorFilter, setMajorFilter] = useState("all");
  const [sortBy, setSortBy] = useState<SortKey>("newest");
  const [modalOpen, setModalOpen] = useState(false);
  const [editing, setEditing] = useState<Student | null>(null);
  const [form, setForm] = useState<FormValues>(blankForm);
  const [formError, setFormError] = useState("");
  const [saving, setSaving] = useState(false);
  const [deleteTarget, setDeleteTarget] = useState<Student | null>(null);
  const [deleting, setDeleting] = useState(false);
  const [notice, setNotice] = useState("");

  const loadStudents = useCallback(async (showSpinner = true) => {
    if (showSpinner) setLoading(true);
    setLoadError("");
    const online = await checkApi();
    setApiOnline(online);
    try {
      const records = await listStudents();
      setStudents(records);
    } catch (error) {
      setLoadError(error instanceof Error ? error.message : "Unable to load students.");
    } finally {
      setLoading(false);
      setRefreshing(false);
    }
  }, []);

  useEffect(() => {
    void loadStudents();
  }, [loadStudents]);

  const majors = useMemo(
    () => [...new Set(students.map((student) => student.major.trim()).filter(Boolean))].sort((a, b) => a.localeCompare(b)),
    [students],
  );

  const filteredStudents = useMemo(() => {
    const normalizedQuery = query.trim().toLowerCase();
    const result = students.filter((student) => {
      const matchesQuery =
        !normalizedQuery ||
        student.name.toLowerCase().includes(normalizedQuery) ||
        student.major.toLowerCase().includes(normalizedQuery) ||
        String(student.id).includes(normalizedQuery);
      return matchesQuery && (majorFilter === "all" || student.major === majorFilter);
    });
    return result.sort((a, b) => {
      if (sortBy === "name") return a.name.localeCompare(b.name);
      if (sortBy === "gpa") return b.gpa - a.gpa;
      if (sortBy === "age") return a.age - b.age;
      return b.id - a.id;
    });
  }, [students, query, majorFilter, sortBy]);

  const stats = useMemo(() => {
    const total = students.length;
    const averageGpa = total ? students.reduce((sum, student) => sum + student.gpa, 0) / total : 0;
    const highestGpa = total ? Math.max(...students.map((student) => student.gpa)) : 0;
    return { total, averageGpa, highestGpa, majorCount: majors.length };
  }, [students, majors.length]);

  function openCreate() {
    setEditing(null);
    setForm(blankForm);
    setFormError("");
    setModalOpen(true);
  }

  function openEdit(student: Student) {
    setEditing(student);
    setForm({ name: student.name, age: String(student.age), major: student.major, gpa: String(student.gpa) });
    setFormError("");
    setModalOpen(true);
  }

  async function handleSubmit(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();
    const name = form.name.trim();
    const major = form.major.trim();
    const age = Number(form.age);
    const gpa = Number(form.gpa);

    if (!name || !major) {
      setFormError("Please enter the student's name and major.");
      return;
    }
    if (!Number.isInteger(age) || age < 1 || age > 120) {
      setFormError("Age must be a whole number between 1 and 120.");
      return;
    }
    if (!Number.isFinite(gpa) || gpa < 0 || gpa > 4) {
      setFormError("GPA must be between 0.00 and 4.00.");
      return;
    }

    const payload: StudentInput = { name, age, major, gpa };
    setSaving(true);
    setFormError("");
    try {
      if (editing) {
        await updateStudent(editing.id, payload);
        setNotice("Student record updated.");
      } else {
        await createStudent(payload);
        setNotice("Student added successfully.");
      }
      setModalOpen(false);
      await loadStudents(false);
    } catch (error) {
      setFormError(error instanceof Error ? error.message : "Could not save this student.");
    } finally {
      setSaving(false);
    }
  }

  async function handleDelete() {
    if (!deleteTarget) return;
    setDeleting(true);
    try {
      await removeStudent(deleteTarget.id);
      setDeleteTarget(null);
      setNotice("Student record deleted.");
      await loadStudents(false);
    } catch (error) {
      setLoadError(error instanceof Error ? error.message : "Could not delete this student.");
      setDeleteTarget(null);
    } finally {
      setDeleting(false);
    }
  }

  function handleRefresh() {
    setRefreshing(true);
    void loadStudents(false);
  }

  return (
    <div className="app-shell">
      <aside className="sidebar">
        <div className="brand">
          <div className="brand-mark"><GraduationCap size={21} strokeWidth={2.2} /></div>
          <div><div className="brand-name">CampusDesk</div><div className="brand-caption">STUDENT WORKSPACE</div></div>
        </div>
        <div className="sidebar-label">WORKSPACE</div>
        <nav className="side-nav" aria-label="Main navigation">
          <div className="nav-item active"><Users size={17} /><span>Students</span><span className="nav-count">{students.length}</span></div>
          <div className="nav-item muted"><BookOpen size={17} /><span>Academic records</span><span className="soon-tag">SOON</span></div>
          <div className="nav-item muted"><Activity size={17} /><span>Reports & analytics</span><span className="soon-tag">SOON</span></div>
        </nav>
        <div className="sidebar-bottom">
          <div className="help-card"><div className="help-icon"><CircleHelp size={17} /></div><div><strong>Need a hand?</strong><span>Manage your student records with ease.</span></div></div>
          <div className="sidebar-footer"><span className="footer-dot" /> Local workspace <span className="footer-version">V2.0</span></div>
        </div>
      </aside>

      <main className="main-area">
        <header className="topbar">
          <div className="breadcrumb"><span>Workspace</span><span className="crumb-slash">/</span><strong>Students</strong></div>
          <div className="topbar-right">
            <div className={`connection-pill ${apiOnline ? "online" : "offline"}`}><span className="connection-dot" />{apiOnline ? "API connected" : "API unavailable"}</div>
            <div className="profile-avatar" aria-label="Workspace profile">CD</div>
          </div>
        </header>

        <div className="page-content">
          <section className="page-heading">
            <div><div className="eyebrow"><Sparkles size={13} /> STUDENT DIRECTORY</div><h1>Students</h1><p className="page-subtitle">Keep track of your students, academic details, and performance.</p></div>
            <button className="primary-button" onClick={openCreate}><Plus size={17} /> Add student</button>
          </section>

          {notice && <div className="notice-bar"><Check size={15} /> {notice}<button onClick={() => setNotice("")} aria-label="Dismiss notification"><X size={15} /></button></div>}

          <section className="stats-grid" aria-label="Student overview">
            <StatCard icon={<Users size={18} />} label="Total students" value={String(stats.total)} detail="Registered records" />
            <StatCard icon={<GraduationCap size={19} />} label="Average GPA" value={formatGpa(stats.averageGpa)} detail="Across all students" />
            <StatCard icon={<Activity size={18} />} label="Highest GPA" value={formatGpa(stats.highestGpa)} detail="Top academic score" />
            <StatCard icon={<BookOpen size={18} />} label="Study majors" value={String(stats.majorCount)} detail="Distinct disciplines" />
          </section>

          <section className="directory-card">
            <div className="directory-header">
              <div><h2>All students <span className="total-chip">{students.length}</span></h2><p>View and manage student information.</p></div>
              <button className="icon-button refresh-button" onClick={handleRefresh} disabled={refreshing} aria-label="Refresh student list" title="Refresh"><RefreshCw size={16} className={refreshing ? "spin" : ""} /></button>
            </div>

            <div className="toolbar">
              <label className="search-box"><Search size={17} /><input value={query} onChange={(event) => setQuery(event.target.value)} placeholder="Search by name, ID, or major..." aria-label="Search students" />{query && <button type="button" onClick={() => setQuery("")} aria-label="Clear search"><X size={14} /></button>}</label>
              <label className="select-control"><SlidersHorizontal size={15} /><select value={majorFilter} onChange={(event) => setMajorFilter(event.target.value)} aria-label="Filter by major"><option value="all">All majors</option>{majors.map((major) => <option key={major} value={major}>{major}</option>)}</select><ChevronDown size={14} /></label>
              <label className="select-control sort-control"><ArrowDownUp size={15} /><select value={sortBy} onChange={(event) => setSortBy(event.target.value as SortKey)} aria-label="Sort students"><option value="newest">Newest first</option><option value="name">Name A–Z</option><option value="gpa">Highest GPA</option><option value="age">Age: youngest</option></select><ChevronDown size={14} /></label>
            </div>

            {loadError && <div className="error-banner" role="alert"><div><strong>We couldn't load the student list.</strong><span>{loadError}</span></div><button className="text-button" onClick={() => void loadStudents()}>Try again</button></div>}

            <div className="table-wrap">
              <table className="student-table">
                <thead><tr><th>STUDENT</th><th>STUDENT ID</th><th>AGE</th><th>MAJOR</th><th>GPA</th><th className="actions-heading">ACTIONS</th></tr></thead>
                <tbody>
                  {loading ? (
                    Array.from({ length: 5 }).map((_, index) => <tr className="skeleton-row" key={index}><td><span className="skeleton student-skeleton" /></td><td><span className="skeleton" /></td><td><span className="skeleton short" /></td><td><span className="skeleton" /></td><td><span className="skeleton short" /></td><td /></tr>)
                  ) : filteredStudents.length ? (
                    filteredStudents.map((student) => (
                      <tr key={student.id}>
                        <td><div className="student-cell"><div className="student-avatar">{initials(student.name)}</div><div><strong>{student.name}</strong><span>Student</span></div></div></td>
                        <td><span className="id-label">#{student.id}</span></td>
                        <td>{student.age}</td>
                        <td><span className="major-label">{student.major}</span></td>
                        <td><span className={`gpa-value ${student.gpa >= 3.5 ? "gpa-high" : student.gpa < 2 ? "gpa-low" : ""}`}>{formatGpa(student.gpa)}</span></td>
                        <td><div className="row-actions"><button className="row-action" onClick={() => openEdit(student)} aria-label={`Edit ${student.name}`} title="Edit"><Pencil size={15} /></button><button className="row-action danger-action" onClick={() => setDeleteTarget(student)} aria-label={`Delete ${student.name}`} title="Delete"><Trash2 size={15} /></button></div></td>
                      </tr>
                    ))
                  ) : (
                    <tr><td colSpan={6}><div className="empty-state"><div className="empty-icon"><Users size={23} /></div><strong>{students.length ? "No matching students" : "Your student list is empty"}</strong><span>{students.length ? "Try a different search or filter." : "Add your first student to start building your directory."}</span>{students.length ? <button className="text-button" onClick={() => { setQuery(""); setMajorFilter("all"); }}>Clear filters</button> : <button className="primary-button compact-button" onClick={openCreate}><Plus size={15} /> Add first student</button>}</div></td></tr>
                  )}
                </tbody>
              </table>
            </div>
            <div className="table-footer"><span>Showing <strong>{loading ? "—" : filteredStudents.length}</strong> of <strong>{students.length}</strong> students</span><span className="footer-note"><span className="footer-dot" /> Changes sync with your database</span></div>
          </section>
          <footer className="page-footer">CampusDesk <span>·</span> Student Management System</footer>
        </div>
      </main>

      {modalOpen && (
        <div className="modal-backdrop" role="presentation" onMouseDown={(event) => { if (event.target === event.currentTarget && !saving) setModalOpen(false); }}>
          <section className="modal-card" role="dialog" aria-modal="true" aria-labelledby="student-modal-title">
            <div className="modal-header"><div><div className="modal-kicker">{editing ? "UPDATE RECORD" : "NEW RECORD"}</div><h2 id="student-modal-title">{editing ? "Edit student" : "Add a student"}</h2><p>{editing ? "Update the student's information below." : "Enter the student's details to add them to your directory."}</p></div><button className="icon-button" onClick={() => setModalOpen(false)} disabled={saving} aria-label="Close dialog"><X size={18} /></button></div>
            <form onSubmit={handleSubmit}>
              <div className="form-grid">
                <label className="form-field full-field"><span>Full name</span><input autoFocus value={form.name} onChange={(event) => setForm({ ...form, name: event.target.value })} placeholder="e.g. Alex Morgan" maxLength={100} required /></label>
                <label className="form-field"><span>Age</span><input type="number" min="1" max="120" step="1" value={form.age} onChange={(event) => setForm({ ...form, age: event.target.value })} placeholder="e.g. 20" required /></label>
                <label className="form-field"><span>GPA <small>0.00 – 4.00</small></span><input type="number" min="0" max="4" step="0.01" value={form.gpa} onChange={(event) => setForm({ ...form, gpa: event.target.value })} placeholder="e.g. 3.70" required /></label>
                <label className="form-field full-field"><span>Major / Department</span><input value={form.major} onChange={(event) => setForm({ ...form, major: event.target.value })} placeholder="e.g. Computer Science" maxLength={100} required /></label>
              </div>
              {formError && <div className="form-error" role="alert">{formError}</div>}
              <div className="modal-actions"><button type="button" className="secondary-button" onClick={() => setModalOpen(false)} disabled={saving}>Cancel</button><button type="submit" className="primary-button" disabled={saving}>{saving ? "Saving..." : editing ? <><Check size={16} /> Save changes</> : <><Plus size={16} /> Add student</>}</button></div>
            </form>
          </section>
        </div>
      )}

      {deleteTarget && (
        <div className="modal-backdrop" role="presentation" onMouseDown={(event) => { if (event.target === event.currentTarget && !deleting) setDeleteTarget(null); }}>
          <section className="modal-card confirm-card" role="alertdialog" aria-modal="true" aria-labelledby="delete-title">
            <div className="confirm-icon"><Trash2 size={21} /></div><h2 id="delete-title">Delete student?</h2><p>This will permanently remove <strong>{deleteTarget.name}</strong>'s record. This action cannot be undone.</p>
            <div className="modal-actions"><button className="secondary-button" onClick={() => setDeleteTarget(null)} disabled={deleting}>Cancel</button><button className="delete-button" onClick={() => void handleDelete()} disabled={deleting}>{deleting ? "Deleting..." : "Delete record"}</button></div>
          </section>
        </div>
      )}
    </div>
  );
}

function StatCard({ icon, label, value, detail }: { icon: ReactNode; label: string; value: string; detail: string }) {
  return <article className="stat-card"><div className="stat-top"><div className="stat-icon">{icon}</div><MoreHorizontal size={18} className="stat-more" /></div><div className="stat-value">{value}</div><div className="stat-label">{label}</div><div className="stat-detail">{detail}</div></article>;
}

export default App;
