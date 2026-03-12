# 🎓 Virtual Classroom Simulation System

A console-based Virtual Classroom Management System built in **C++** using Object-Oriented Programming principles. Developed as part of **Case Study 22** at **ITM Skills University – School of Future Tech**.

---

## 📋 Table of Contents

* [Overview](https://www.google.com/search?q=%23overview)
* [Features](https://www.google.com/search?q=%23features)
* [Project Structure](https://www.google.com/search?q=%23project-structure)
* [Classes](https://www.google.com/search?q=%23classes)
* [Getting Started](https://www.google.com/search?q=%23getting-started)
* [Usage](https://www.google.com/search?q=%23usage)
* [Input Validation](https://www.google.com/search?q=%23input-validation)
* [Tech Stack](https://www.google.com/search?q=%23tech-stack)
* [Author](https://www.google.com/search?q=%23author)

---

## Overview

This system simulates the core operations of an online classroom management platform. It allows institutions to manage students, schedule classes, enroll students, and track attendance — all through an interactive menu-driven console interface. The latest version now includes persistent storage, saving all data automatically to CSV files.

---

## Features

* ✅ **Persistent Storage**: Saves and loads data from `students.csv`, `classes.csv`, `attendance.csv`, and `enrollment.csv`.
* ✅ **Add and List Students**: Manage student profiles with validated input.
* ✅ **Schedule Classes**: Organize sessions with subject, instructor, date, time, and capacity.
* ✅ **Enroll Students**: Link students to classes with capacity and duplicate enrollment checks.
* ✅ **Mark Attendance**: Track presence (present/absent) for each enrolled student.
* ✅ **View Attendance Summary**: Display a breakdown of attendance with present/total counts.
* ✅ **Advanced Input Validation**: Uses Regular Expressions (Regex) and range checks for data integrity.
* ✅ **Auto-incremented Unique IDs**: Automatically manages student and class IDs, even after restarting the program.

---

## Project Structure

```
Virtual--classroom.cpp
│
├── Input Validation Helpers    # Regex and range-based functions for clean user input
├── Student Class               # Encapsulates student identity (ID, Name, Email)
├── ClassSession Class          # Manages session details, enrollment list, and attendance map
└── VirtualClassroomSystem Class# Main controller — handles file I/O and menu operations

```

---

## Classes

### `Student`

| Member | Type | Description |
| --- | --- | --- |
| `id` | `int` | Unique student ID |
| `name` | `string` | Student full name |
| `email` | `string` | Student email address |

### `ClassSession`

| Member | Type | Description |
| --- | --- | --- |
| `id` | `int` | Unique class ID |
| `subject` | `string` | Subject name |
| `instructor` | `string` | Instructor name |
| `date` | `string` | Date in DD-MM-YYYY format |
| `time` | `string` | Time in HH:MM (24-hr) format |
| `maxCapacity` | `int` | Maximum students allowed |
| `enrolledStudents` | `vector<int>` | List of enrolled student IDs |
| `attendance` | `map<int, bool>` | Attendance records (ID -> Present/Absent) |

### `VirtualClassroomSystem`

The main controller class that manages the `vector` containers for students and classes. It handles the logic for loading data from files on startup and saving data on exit.

---

## Getting Started

### Prerequisites

* A C++ compiler supporting **C++11 or later** (g++, clang++, MSVC)
* Any IDE: VS Code, Code::Blocks, CLion, or terminal

### Compile

```bash
g++ -std=c++11 -o virtual_classroom Virtual--classroom.cpp

```

### Run

```bash
./virtual_classroom

```

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

1. **Add Students**: Enter a name and a valid email.
2. **Schedule Class**: Define a subject, teacher, and time/capacity.
3. **Enroll Student**: Use Student ID and Class ID to register a student for a session.
4. **Mark Attendance**: Go through the enrollment list to mark 'y' or 'n'.
5. **Exit**: Choose `0` to save all progress to CSV files.

---

## Input Validation

All inputs are strictly validated to prevent system crashes:

| Field | Rule |
| --- | --- |
| **Name** | Non-empty, no digits allowed |
| **Email** | Must match `user@domain.com` regex pattern |
| **Date** | DD-MM-YYYY format with day (1–31), month (1–12), and year (2000–2100) checks |
| **Time** | HH:MM in 24-hour format (00:00 – 23:59) |
| **Integer** | Range-bound checks (e.g., choice 0–7 or capacity 1–500) |
| **Yes/No** | Case-insensitive `y` or `n` |

---

## Tech Stack

* **Language:** C++ (C++11)
* **Libraries:** `<iostream>`, `<vector>`, `<map>`, `<algorithm>`, `<regex>`, `<iomanip>`, `<fstream>`, `<sstream>`
* **Paradigm:** Object-Oriented Programming (OOP)
* **Storage:** CSV-based Persistent Storage (`.csv`)

---

## Author

**Tanish Tushar Gawade** Roll No: 150096725020

ITM Skills University – School of Future Tech

Academic Year 2025–2026
