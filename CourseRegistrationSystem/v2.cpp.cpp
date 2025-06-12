#include <bits/stdc++.h>

using namespace std;

class Student
{
public:
    string ID;
    string name;
    int year;
    unordered_set<string> completedCourses;
    unordered_set<string> enrolledCourses;

    Student() : ID(""), name(""), year(0) {}

    Student(string ID, string name, int year, vector<string> v) : ID(ID), name(name), year(year)
    {
        completedCourses.insert(v.begin(), v.end());
    }
};

class Course
{
public:
    string code;
    string name;
    int credits;
    int capacity;
    string slot;
    vector<string> prerequisites;
    vector<string> enrolledStudents;
    queue<string> waitlist;

    Course() : code(""), name(""), credits(0), capacity(0), slot("") {}

    Course(string code, string name, int credits, int capacity, string slot, vector<string> prerequisites) : code(code), name(name), credits(credits), capacity(capacity), slot(slot), prerequisites(prerequisites) {}
};

class CourseRegSystem
{
public:
    unordered_map<string, Student> students;
    unordered_map<string, Course> courses;

    bool hasPrerequisites(Student &student, Course &course)
    {
        for (string prereq : course.prerequisites)
        {
            if (student.completedCourses.find(prereq) == student.completedCourses.end())
            {
                return false;
            }
        }
        return true;
    }

    void addStudent(string ID, string name, int year, vector<string> completedCourses)
    {
        if (students.find(ID) != students.end())
            return;
        Student s(ID, name, year, completedCourses);
        students[ID] = s;
    }

    void addCourse(string code, string name, int credits, int capacity, string slot, vector<string> prerequisites)
    {
        if (courses.find(code) != courses.end())
            return;
        for (string pre : prerequisites)
        {
            if (courses.find(pre) == courses.end())
                return;
        }
        Course c(code, name, credits, capacity, slot, prerequisites);
        courses[code] = c;
    }

    void enrollStudent(string ID, string code)
    {
        if (students.find(ID) == students.end() || courses.find(code) == courses.end())
            return;
        Student &student = students[ID];
        Course &course = courses[code];

        if (!hasPrerequisites(student, course))
            return;

        for (string cur_courses : student.enrolledCourses)
        {
            if (cur_courses == code)
                return;
        }

        if (course.enrolledStudents.size() == course.capacity)
        {
            course.waitlist.push(ID);
            return;
        }

        for (string cur_courses : student.enrolledCourses)
        {
            if (courses[cur_courses].slot == course.slot)
                return;
        }

        course.enrolledStudents.push_back(ID);
        student.enrolledCourses.insert(code);
    }

    void drop(string ID, string code)
    {
        if (students.find(ID) == students.end() || courses.find(code) == courses.end())
            return;
        Student &student = students[ID];
        Course &course = courses[code];

        for (auto ite = course.enrolledStudents.begin(); ite != course.enrolledStudents.end(); ite++)
        {
            if (*ite == ID)
            {
                course.enrolledStudents.erase(ite);
                break;
            }
        }
        student.enrolledCourses.erase(code);

        while (!course.waitlist.empty() && course.enrolledStudents.size() < course.capacity)
        {
            string newID = course.waitlist.front();
            course.waitlist.pop();
            enrollStudent(newID, code);
        }
    }

    void print(string code)
    {
        if (courses.find(code) == courses.end())
        {
            cout << "Invalid Course " << code << endl;
            return;
        }
        Course &course = courses[code];
        cout << "Enrolled students in " << code << ":" << endl;
        for (string ID : course.enrolledStudents)
        {
            cout << ID << endl;
        }
    }
};

int main()
{
    CourseRegSystem crs;
    int n;
    cin >> n;
    while (n--)
    {
        string s;
        cin >> s;
        if (s == "add_student")
        {
            string ID, name;
            int year;
            cin >> ID >> name >> year;
            int m;
            cin >> m;
            vector<string> completedCourses;
            while (m--)
            {
                string course;
                cin >> course;
                completedCourses.push_back(course);
            }
            crs.addStudent(ID, name, year, completedCourses);
        }
        if (s == "add_course")
        {
            string code, name, slot;
            int credits, capacity;
            cin >> code >> name >> credits >> capacity >> slot;
            int m;
            cin >> m;
            vector<string> prerequisites;
            while (m--)
            {
                string course;
                cin >> course;
                prerequisites.push_back(course);
            }
            crs.addCourse(code, name, credits, capacity, slot, prerequisites);
        }
        if (s == "enroll")
        {
            string ID, code;
            cin >> ID >> code;
            crs.enrollStudent(ID, code);
        }
        if (s == "drop")
        {
            string ID, code;
            cin >> ID >> code;
            crs.drop(ID, code);
        }
        if (s == "print")
        {
            string code;
            cin >> code;
            crs.print(code);
        }
    }
}