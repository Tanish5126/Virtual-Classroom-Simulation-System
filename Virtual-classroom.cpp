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
#include <limits>

using namespace std;

// ========================================
//         INPUT VALIDATION HELPERS
// ========================================

// Clears failed cin state and leftover input
void clearInput()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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

        if (regex_match(value, emailPattern))
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
            int day   = stoi(match[1]);
            int month = stoi(match[2]);
            int year  = stoi(match[3]);

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

    string getDate() const {
        return date;
    }

    string getTime() const {
        return time;
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

//=============== OPTION 1 ==============//
public:
    void addStudent()
    {
        
        string name  = getValidName("Name  : ");
        string email = getValidEmail("Email : ");

        students.emplace_back(nextStudentId++, name, email);
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
            cout << "Enrollment successful.\n";
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
            case 0: cout << "Goodbye!\n"; break;
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