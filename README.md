# 🎓 Virtual Classroom Simulation System

A console-based Virtual Classroom Management System built in **C++** using Object-Oriented Programming principles. Developed as part of **Case Study 22** at **ITM Skills University – School of Future Tech**.

---

## 📋 Table of Contents

- [Overview]
- [Features]
- [Project Structure]
- [Classes]
- [Getting Started]
- [Usage]
- [Input Validation]
- [Tech Stack]
- [Author]

---

## Overview

This system simulates the core operations of an online classroom management platform. It allows institutions to manage students, schedule classes, enroll students, and track attendance — all through an interactive menu-driven console interface.

---

## Features

- ✅ Add and list students with validated input
- ✅ Schedule classes with subject, instructor, date, time, and capacity
- ✅ Enroll students into classes with capacity and duplicate checks
- ✅ Mark attendance (present/absent) per class session
- ✅ View attendance summary with present/total count
- ✅ Full input validation using Regular Expressions
- ✅ Auto-incremented unique IDs for students and classes

---

## Project Structure

```
Virtual-classroom.cpp
│
├── Input Validation Helpers     # Global functions for validated user input
├── Student Class                # Stores student ID, name, email
├── ClassSession Class           # Stores session details, enrollment, attendance
└── VirtualClassroomSystem Class # Main controller — manages all operations
```

---

## Classes

### `Student`
| Member | Type | Description |
|--------|------|-------------|
| `id` | `int` | Auto-assigned unique student ID |
| `name` | `string` | Student full name |
| `email` | `string` | Student email address |

### `ClassSession`
| Member | Type | Description |
|--------|------|-------------|
| `id` | `int` | Auto-assigned unique class ID |
| `subject` | `string` | Subject name |
| `instructor` | `string` | Instructor name |
| `date` | `string` | Date in DD-MM-YYYY format |
| `time` | `string` | Time in HH:MM (24-hr) format |
| `maxCapacity` | `int` | Maximum students allowed |
| `enrolledStudents` | `vector<int>` | List of enrolled student IDs |
| `attendance` | `map<int, bool>` | Attendance records per student |

### `VirtualClassroomSystem`
The main controller class that manages all students and class sessions and drives the menu interface.

---

## Getting Started

### Prerequisites
- A C++ compiler supporting **C++11 or later** (g++, clang++, MSVC)
- Any IDE: VS Code, Code::Blocks, CLion, or terminal

### Compile

```bash
g++ -std=c++11 -o virtual_classroom Virtual-classroom.cpp
```

### Run

```bash
./virtual_classroom
```

> On Windows:
> ```bash
> virtual_classroom.exe
> ```

---

## Usage

Once running, you will see the main menu:

```
===== Virtual Classroom System =====
======= ITM SKILLS UNIVERSITY ======
 1. Add Student
 2. List Students
 3. Schedule Class
 4. List Classes
 5. Enroll Student
 6. Mark Attendance
 7. View Attendance
 0. Exit
Choice:
```

### Example Workflow

```
1 → Add students (enter name + email)
3 → Schedule a class (subject, instructor, date, time, capacity)
5 → Enroll a student (student ID + class ID)
6 → Mark attendance (present/absent for each enrolled student)
7 → View attendance summary
```

---

## Input Validation

All inputs are validated before being accepted:

| Field | Rule |
|-------|------|
| Name | Non-empty, no digits allowed |
| Email | Must match `user@domain.com` pattern |
| Date | DD-MM-YYYY format, valid day/month/year ranges |
| Time | HH:MM in 24-hour format (00:00 – 23:59) |
| Integer | Must be within the specified range |
| Yes/No | Only `y` or `n` accepted |

---

## Tech Stack

- **Language:** C++ (C++11)
- **Libraries:** `<iostream>`, `<vector>`, `<map>`, `<algorithm>`, `<regex>`, `<iomanip>`, `<limits>`
- **Paradigm:** Object-Oriented Programming (OOP)
- **Storage:** In-memory (STL containers)

---

## Author

**Tanish Tushar Gawade**  
Roll No: 150096725020  
ITM Skills University – School of Future Tech  
Academic Year 2025–2026