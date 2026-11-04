export interface Student {
  id: number;
  name: string;
  age: number;
  major: string;
  gpa: number;
}

export interface StudentInput {
  name: string;
  age: number;
  major: string;
  gpa: number;
}

export interface StudentStats {
  total: number;
  averageGpa: number;
  highestGpa: number;
  majorCount: number;
}
