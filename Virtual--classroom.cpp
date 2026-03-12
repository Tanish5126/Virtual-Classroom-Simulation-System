//===========================================================
//
//        VIRTUAL CLASSROOM SIMULATION SYSTEM
//
//===========================================================

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
#include <regex>
#include <fstream>
#include <sstream>

using namespace std;

// ========================================
//         FILE NAME CONSTANTS
// ========================================
const string STUDENTS_FILE   = "students.csv";
const string CLASSES_FILE    = "classes.csv";
const string ATTENDANCE_FILE = "attendance.csv";
const string ENROLLMENT_FILE = "enrollment.csv";

// ========================================
//         INPUT VALIDATION HELPERS
// ========================================

// Clears failed cin state and leftover input
void clearInput()
{
    cin.clear();
    cin.ignore(1000, '\n');
}

// Get a valid non-empty string (no digits allowed for names)
string getValidName(const string &prompt)
{
    string value;
    while (true)
    {
        cout << prompt;
        getline(cin, value);

        if (value.empty())
        {
            cout << "  [Error] Input cannot be empty. Try again.\n";
            continue;
        }

        bool hasDigit = false;
        for (char c : value)
        {
            if (isdigit(c))
            {
                hasDigit = true;
                break;
            }
        }

        if (hasDigit)
        {
            cout << "  [Error] Name cannot contain numbers. Try again.\n";
            continue;
        }

        break;
    }
    return value;
}

// Get a valid email (must contain @ and a dot after @)
string getValidEmail(const string &prompt)
{
    string value;
    regex emailPattern(R"(^[\w\.-]+@[\w\.-]+\.\w{2,}$)");
    while (true)
    {
        cout << prompt;
        getline(cin, value);

        if (regex_match(value, emailPattern)) // --> returns true if value matches
            break;

        cout << "  [Error] Invalid email format. Example: user@example.com\n";
    }
    return value;
}

// Get a valid date in DD-MM-YYYY format with range checks
string getValidDate(const string &prompt)
{
    string value;
    regex datePattern(R"(^(\d{2})-(\d{2})-(\d{4})$)");
    while (true)
    {
        cout << prompt;
        getline(cin, value);

        smatch match;
        if (regex_match(value, match, datePattern))
        {
            // --> string to integer conversion
            int day   = stoi(match[1]);  // --> Date
            int month = stoi(match[2]);  // --> Month
            int year  = stoi(match[3]);  // --> Year

            if (month < 1 || month > 12)
            {
                cout << "  [Error] Month must be between 01 and 12.\n";
                continue;
            }
            if (day < 1 || day > 31)
            {
                cout << "  [Error] Day must be between 01 and 31.\n";
                continue;
            }
            if (year < 2000 || year > 2100)
            {
                cout << "  [Error] Year must be between 2000 and 2100.\n";
                continue;
            }

            break;
        }

        cout << "  [Error] Invalid date format. Use DD-MM-YYYY (e.g. 25-06-2025)\n";
    }
    return value;
}

// Get a valid time in HH:MM (24-hour) format
string getValidTime(const string &prompt)
{
    string value;
    regex timePattern(R"(^([01]\d|2[0-3]):([0-5]\d)$)");
    while (true)
    {
        cout << prompt;
        getline(cin, value);

        if (regex_match(value, timePattern))
            break;

        cout << "  [Error] Invalid time format. Use HH:MM in 24-hour (e.g. 09:30 or 14:00)\n";
    }
    return value;
}

// Get a valid integer within a given range
int getValidInt(const string &prompt, int minVal, int maxVal)
{
    int value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            if (value >= minVal && value <= maxVal)
            {
                clearInput();
                break;
            }
            cout << "  [Error] Please enter a number between "
                 << minVal << " and " << maxVal << ".\n";
        }
        else
        {
            cout << "  [Error] Invalid input. Please enter a number.\n";
            clearInput();
        }
    }
    return value;
}

// Get a valid yes/no character
bool getValidYesNo(const string &prompt)
{
    char ans;
    while (true)
    {
        cout << prompt;
        cin >> ans;
        clearInput();

        if (ans == 'y' || ans == 'Y') return true;
        if (ans == 'n' || ans == 'N') return false;

        cout << "  [Error] Please enter 'y' or 'n'.\n";
    }
}

// Get a valid non-empty general string (for subject, instructor)
string getValidString(const string &prompt)
{
    string value;
    while (true)
    {
        cout << prompt;
        getline(cin, value);

        if (!value.empty())
            break;

        cout << "  [Error] Input cannot be empty. Try again.\n";
    }
    return value;
}


// ========================================
//              STUDENT CLASS
// ========================================
class Student
{
private:
    int id;
    string name;
    string email;

public:
    Student(int id, string name, string email) : id(id), name(name), email(email) {}

    int getId() const {
        return id;
    }

    string getName() const {
        return name;
    }

    string getEmail() const {
        return email;
    }

    void display() const
    {
        cout << left << setw(5) << id
             << setw(20) << name
             << email << endl;
    }
};

// ========================================
//          CLASS SESSION CLASS
// ========================================
class ClassSession
{
private:
    int id;
    string subject;
    string instructor;
    string date;
    string time;
    int maxCapacity;

    vector<int> enrolledStudents;
    map<int, bool> attendance; // studentId -> present/absent

public:
    ClassSession(int id, string sub, string inst, string d, string t, int cap)
        : id(id), subject(sub), instructor(inst), date(d), time(t), maxCapacity(cap) {}

    int getId() const {
        return id;
    }

    string getSubject() const {
        return subject;
    }

    string getInstructor() const {
        return instructor;
    }

    string getDate() const {
        return date;
    }

    string getTime() const {
        return time;
    }

    int getMaxCapacity() const {
        return maxCapacity;
    }

    void display() const
    {
        cout << left << setw(5) << id
             << setw(18) << subject
             << setw(15) << instructor
             << setw(12) << date
             << setw(8) << time
             << enrolledStudents.size() << "/" << maxCapacity << endl;
    }

    bool enrollStudent(int studentId)
    {
        if ((int)enrolledStudents.size() >= maxCapacity)
            return false;

        if (find(enrolledStudents.begin(), enrolledStudents.end(), studentId) != enrolledStudents.end())
            return false;

        enrolledStudents.push_back(studentId);
        return true;
    }

    void markAttendance(int studentId, bool present)
    {
        attendance[studentId] = present;
    }

    void showAttendance(const vector<Student> &students) const
    {
        int presentCount = 0;
        cout << "\nAttendance for " << subject << ":\n";
        for (int sid : enrolledStudents)
        {
            auto it = find_if(students.begin(), students.end(), [sid](const Student &s) {
                return s.getId() == sid;
            });

            if (it != students.end())
            {
                bool present = attendance.count(sid) && attendance.at(sid);
                cout << left << setw(20) << it->getName()
                     << (present ? "Present" : "Absent") << endl;
                if (present)
                    presentCount++;
            }
        }

        cout << "Summary: " << presentCount << "/"
             << enrolledStudents.size() << " present\n";
    }

    const vector<int> &getEnrolledStudents() const
    {
        return enrolledStudents;
    }

    const map<int, bool> &getAttendance() const
    {
        return attendance;
    }
};

// =======================================
//          MAIN SYSTEM CLASS
// =======================================
class VirtualClassroomSystem
{
private:
    vector<Student> students;
    vector<ClassSession> classes;

    int nextStudentId = 1;
    int nextClassId = 1;

    Student *findStudent(int id)
    {
        for (auto &s : students)
            if (s.getId() == id)
                return &s;
        return nullptr;
    }

    ClassSession *findClass(int id)
    {
        for (auto &c : classes)
            if (c.getId() == id)
                return &c;
        return nullptr;
    }

    // ========================================
    //           FILE HANDLING - SAVE
    // ========================================

    // Save all students to students.csv
    // Format: id,name,email
    void saveStudents() const
    {
        ofstream file(STUDENTS_FILE);   // --> It is used to write data to a file
        if (!file.is_open())
        {
            cout << "  [Warning] Could not save students to file.\n";
            return;
        }

        file << "id,name,email\n"; // --> CSV header
        for (const Student &s : students)
            file << s.getId() << "," << s.getName() << "," << s.getEmail() << "\n";

        file.close();
    }

    // Save all classes to classes.csv
    // Format: id,subject,instructor,date,time,capacity
    void saveClasses() const
    {
        ofstream file(CLASSES_FILE);
        if (!file.is_open())
        {
            cout << "  [Warning] Could not save classes to file.\n";
            return;
        }

        file << "id,subject,instructor,date,time,capacity\n"; // CSV header
        for (const ClassSession &c : classes)
            file << c.getId() << ","
                 << c.getSubject() << ","
                 << c.getInstructor() << ","
                 << c.getDate() << ","
                 << c.getTime() << ","
                 << c.getMaxCapacity() << "\n";

        file.close();
    }

    // Save enrollment data to enrollment.csv
    // Format: classId,studentId
    void saveEnrollment() const
    {
        ofstream file(ENROLLMENT_FILE);
        if (!file.is_open())
        {
            cout << "  [Warning] Could not save enrollment to file.\n";
            return;
        }

        file << "classId,studentId\n"; // CSV header
        for (const ClassSession &c : classes)
            for (int sid : c.getEnrolledStudents())
                file << c.getId() << "," << sid << "\n";

        file.close();
    }

    // Save attendance data to attendance.csv
    // Format: classId,studentId,present
    void saveAttendance() const
    {
        ofstream file(ATTENDANCE_FILE);
        if (!file.is_open())
        {
            cout << "  [Warning] Could not save attendance to file.\n";
            return;
        }

        file << "classId,studentId,present\n"; // CSV header
        for (const ClassSession &c : classes)
            for (const auto &entry : c.getAttendance())
                file << c.getId() << "," << entry.first << "," << entry.second << "\n";

        file.close();
    }

    // Save all data at once
    void saveAll() const
    {
        saveStudents();
        saveClasses();
        saveEnrollment();
        saveAttendance();
    }

    // ========================================
    //           FILE HANDLING - LOAD
    // ========================================

    // Load students from students.csv
    void loadStudents()
    {
        ifstream file(STUDENTS_FILE);
        if (!file.is_open())
            return; // No file yet, skip silently on first run

        string line;
        getline(file, line); // Skip header line

        while (getline(file, line))
        {
            if (line.empty()) continue;

            stringstream ss(line);  // --> stringstream allows us to treat the string like a stream,
            string idStr, name, email;

            getline(ss, idStr, ',');
            getline(ss, name,  ',');
            getline(ss, email, ',');

            if (idStr.empty() || name.empty() || email.empty()) continue;

            int id = stoi(idStr);
            students.emplace_back(id, name, email);

            if (id >= nextStudentId)
                nextStudentId = id + 1;
        }

        file.close();
    }

    // Load classes from classes.csv
    void loadClasses()
    {
        ifstream file(CLASSES_FILE);
        if (!file.is_open())
            return; // No file yet, skip silently on first run

        string line;
        getline(file, line); // Skip header line

        while (getline(file, line))
        {
            if (line.empty()) continue;

            stringstream ss(line);
            string idStr, subject, instructor, date, time, capStr;

            getline(ss, idStr,       ',');
            getline(ss, subject,     ',');
            getline(ss, instructor,  ',');
            getline(ss, date,        ',');
            getline(ss, time,        ',');
            getline(ss, capStr,      ',');

            if (idStr.empty() || subject.empty()) continue;

            int id  = stoi(idStr);
            int cap = stoi(capStr);
            classes.emplace_back(id, subject, instructor, date, time, cap);

            if (id >= nextClassId)
                nextClassId = id + 1;
        }

        file.close();
    }

    // Load enrollment from enrollment.csv
    void loadEnrollment()
    {
        ifstream file(ENROLLMENT_FILE);
        if (!file.is_open())
            return;

        string line;
        getline(file, line); // Skip header

        while (getline(file, line))
        {
            if (line.empty()) continue;

            stringstream ss(line);
            string cidStr, sidStr;

            getline(ss, cidStr, ',');
            getline(ss, sidStr, ',');

            if (cidStr.empty() || sidStr.empty()) continue;

            int cid = stoi(cidStr);
            int sid = stoi(sidStr);

            ClassSession *c = findClass(cid);
            if (c) c->enrollStudent(sid);
        }

        file.close();
    }

    // Load attendance from attendance.csv
    void loadAttendance()
    {
        ifstream file(ATTENDANCE_FILE);
        if (!file.is_open())
            return;

        string line;
        getline(file, line); // Skip header

        while (getline(file, line))
        {
            if (line.empty()) continue;

            stringstream ss(line);
            string cidStr, sidStr, presentStr;

            getline(ss, cidStr,     ',');
            getline(ss, sidStr,     ',');
            getline(ss, presentStr, ',');

            if (cidStr.empty() || sidStr.empty()) continue;

            int  cid     = stoi(cidStr);
            int  sid     = stoi(sidStr);
            bool present = (presentStr == "1");

            ClassSession *c = findClass(cid);
            if (c) c->markAttendance(sid, present);
        }

        file.close();
    }

    // Load all data at once
    // Order matters: classes must load before enrollment & attendance
    void loadAll()
    {
        loadStudents();
        loadClasses();
        loadEnrollment();
        loadAttendance();
    }

//=============== OPTION 1 ==============//
public:
    void addStudent()
    {
        string name  = getValidName("Name  : ");
        string email = getValidEmail("Email : ");

        students.emplace_back(nextStudentId++, name, email);
        saveStudents(); // persist immediately after adding
        cout << "Student added successfully.\n";
    }

//=============== OPTION 2 ==============//
    void listStudents() const
    {
        if (students.empty())
        {
            cout << "No students registered yet.\n";
            return;
        }
        cout << "\nID   Name                Email\n";
        cout << string(50, '-') << "\n";
        for (const Student &s : students)
            s.display();
    }

//=============== OPTION 3 ==============//
    void scheduleClass()
    {
        string sub  = getValidString("Subject    : ");
        string inst = getValidName("Instructor : ");
        string date = getValidDate("Date (DD-MM-YYYY) : ");
        string time = getValidTime("Time (HH:MM)      : ");
        int cap     = getValidInt("Capacity   : ", 1, 500);

        classes.emplace_back(nextClassId++, sub, inst, date, time, cap);
        saveClasses(); // persist immediately after scheduling
        cout << "Class scheduled successfully.\n";
    }

//=============== OPTION 4 ==============//
    void listClasses() const
    {
        if (classes.empty())
        {
            cout << "No classes scheduled yet.\n";
            return;
        }
        cout << "\nID   Subject           Instructor     Date        Time    Enrolled\n";
        cout << string(70, '-') << "\n";
        for (const auto &c : classes)
            c.display();
    }

//=============== OPTION 5 ==============//
    void enrollStudent()
    {
        if (students.empty() || classes.empty())
        {
            cout << "Please add students and schedule classes first.\n";
            return;
        }

        int sid = getValidInt("Student ID: ", 1, nextStudentId - 1);
        int cid = getValidInt("Class ID  : ", 1, nextClassId - 1);

        Student *s = findStudent(sid);
        ClassSession *c = findClass(cid);

        if (!s || !c)
        {
            cout << "Invalid student or class.\n";
            return;
        }

        if (c->enrollStudent(sid))
        {
            saveEnrollment(); // persist immediately after enrolling
            cout << "Enrollment successful.\n";
        }
        else
            cout << "Enrollment failed (duplicate or class is full).\n";
    }

//=============== OPTION 6 ==============//
    void markAttendance()
    {
        if (classes.empty())
        {
            cout << "No classes available.\n";
            return;
        }

        int cid = getValidInt("Class ID: ", 1, nextClassId - 1);

        ClassSession *c = findClass(cid);
        if (!c)
        {
            cout << "Invalid class.\n";
            return;
        }

        if (c->getEnrolledStudents().empty())
        {
            cout << "No students enrolled in this class.\n";
            return;
        }

        for (int sid : c->getEnrolledStudents())
        {
            Student *s = findStudent(sid);
            if (!s) continue;

            bool present = getValidYesNo(s->getName() + " present? (y/n): ");
            c->markAttendance(sid, present);
        }

        saveAttendance(); // persist immediately after marking
        cout << "Attendance recorded.\n";
    }

//=============== OPTION 7 ==============//
    void viewAttendance()
    {
        if (classes.empty())
        {
            cout << "No classes available.\n";
            return;
        }

        int cid = getValidInt("Class ID: ", 1, nextClassId - 1);

        ClassSession *c = findClass(cid);
        if (!c)
        {
            cout << "Invalid class.\n";
            return;
        }

        c->showAttendance(students);
    }

    void run()
    {
        loadAll(); // Load all saved data on startup

        int choice;
        do
        {
            cout << "\n===== Virtual Classroom System =====\n"
                 << "\n======= ITM SKILLS UNIVERSITY ======\n"
                 << " 1. Add Student\n"
                 << " 2. List Students\n"
                 << " 3. Schedule Class\n"
                 << " 4. List Classes\n"
                 << " 5. Enroll Student\n"
                 << " 6. Mark Attendance\n"
                 << " 7. View Attendance\n"
                 << " 0. Exit\n"
                 << "Choice: ";

            if (cin >> choice)
            {
                clearInput();
            }
            else
            {
                cout << "  [Error] Please enter a valid menu option.\n";
                clearInput();
                choice = -1;
                continue;
            }

            switch (choice)
            {
            case 1: addStudent();     break;
            case 2: listStudents();   break;
            case 3: scheduleClass();  break;
            case 4: listClasses();    break;
            case 5: enrollStudent();  break;
            case 6: markAttendance(); break;
            case 7: viewAttendance(); break;
            case 0:
                saveAll(); // Final save on clean exit
                cout << "Data saved. Goodbye!\n";
                break;
            default: cout << "  [Error] Invalid option. Choose between 0-7.\n";
            }

        } while (choice != 0);
    }
};

// =======================================
//             MAIN FUNCTION
// =======================================
int main()
{
    VirtualClassroomSystem system;
    system.run();
    return 0;
}