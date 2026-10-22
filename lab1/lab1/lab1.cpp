#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct StudentDat {
    char fullName[50];
    int course;
    int group;
    char address[100];
    char phone[20];
};

struct StudentTxt {
    char fullName[50];
    int course;
    int group;
    int grades[4];
    double avgGrade;
};

// Write DAT
void RW(const string& filename, StudentDat students[], int size) {
    ofstream out(filename, ios::binary | ios::trunc);
    if (!out) {
        cerr << "Помилка відкриття файлу " << filename << " для запису!" << endl;
        return;
    }
    out.write((char*)students, size * sizeof(StudentDat));
    out.close();
    cout << "Дані успішно записано у бінарний файл " << filename << endl;
}

// Write TXT
void RW(const string& filename, StudentTxt students[], int size) {
    ofstream out(filename, ios::trunc);
    if (!out) {
        cerr << "Помилка відкриття файлу " << filename << " для запису!" << endl;
        return;
    }
    for (int i = 0; i < size; i++) {
        out << students[i].fullName << ";"
            << students[i].course << ";"
            << students[i].group;
        for (int j = 0; j < 4; j++) {
            out << ";" << students[i].grades[j];
        }
        out << endl;
    }
    out.close();
    cout << "Дані успішно записано у текстовий файл " << filename << endl;
}

// Students with no phone n DAT, 8 var
void RD(const string& filename, StudentDat /*tag*/) {
    ifstream in(filename, ios::binary);
    if (in.fail()) {
        cerr << "Помилка відкриття файлу " << filename << " для читання!" << endl;
        return;
    }

    cout << "\n РЕЗУЛЬТАТИ ОБРОБКИ INFO.DAT (Студенти без телефону) " << endl;
    StudentDat st;
    bool found = false;

    while (in.read((char*)&st, sizeof(StudentDat))) {
        // check epmtyness phone line
        string phoneStr(st.phone);
        if (phoneStr == "немає" || phoneStr == "-" || phoneStr.empty()) {
            cout << "Прізвище та ім'я: " << st.fullName << " | Адреса: " << st.address << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Усі студенти мають телефон." << endl;
    }
    in.close();
}

// Sort grade TXT, 8 var
void RD(const string& filename, StudentTxt /*tag*/) {
    ifstream in(filename);
    if (in.fail()) {
        cerr << "Помилка відкриття файлу " << filename << " для читання!" << endl;
        return;
    }

    vector<StudentTxt> students;
    string line;

    while (in >> ws) { // Skip empty lines
        if (!getline(in, line)) break;
        StudentTxt st;
        size_t pos = 0;
        string token;

        pos = line.find(';');
        string name = line.substr(0, pos);
        line.erase(0, pos + 1);
        strcpy_s(st.fullName, name.c_str());

        pos = line.find(';');
        st.course = stoi(line.substr(0, pos));
        line.erase(0, pos + 1);

        pos = line.find(';');
        st.group = stoi(line.substr(0, pos));
        line.erase(0, pos + 1);

        int sum = 0;
        for (int j = 0; j < 4; j++) {
            pos = line.find(';');
            st.grades[j] = stoi(line.substr(0, pos == string::npos ? line.length() : pos));
            sum += st.grades[j];
            if (pos != string::npos) line.erase(0, pos + 1);
        }
        st.avgGrade = sum / 4.0;
        students.push_back(st);
    }
    in.close();

    sort(students.begin(), students.end(), [](const StudentTxt& a, const StudentTxt& b) {
        if (a.course != b.course) return a.course < b.course;
        return a.avgGrade > b.avgGrade;
        });

    cout << "\n РЕЗУЛЬТАТИ ОБРОБКИ INFO.TXT (Успішність за курсами) " << endl;
    int currentCourse = -1;
    for (const auto& s : students) {
        if (s.course != currentCourse) {
            currentCourse = s.course;
            cout << "\n Курс " << currentCourse << endl;
        }
        cout << "Студент: " << s.fullName
            << " | Група: " << s.group
            << " | Оцінки: " << s.grades[0] << ", " << s.grades[1] << ", " << s.grades[2] << ", " << s.grades[3]
            << " | Середній бал: " << s.avgGrade << endl;
    }
}

// Main
void main() {
    setlocale(LC_ALL, "Ukrainian");

    const int count = 5;

    // Test inf DAT
    StudentDat datStudents[count] = {
        {"Петренко Олександр", 2, 9, "вул. Шевченка, 12", "123-45-67"},
        {"Ковальчук Марія", 1, 8, "вул. Кам'янецька, 45", "немає"},
        {"Сидорук Іван", 2, 9, "вул. Проскурівська, 10", "-"},
        {"Мельник Ольга", 3, 7, "вул. Бандери, 5", "987-65-43"},
        {"Бондар Андрій", 1, 8, "вул. Грушевського, 22", "немає"}
    };

    // Test info TXT
    StudentTxt txtStudents[count] = {
        {"Шевченко Д.", 1, 8, {5, 4, 5, 5}, 0},
        {"Гончар О.", 2, 9, {3, 4, 4, 3}, 0},
        {"Франко І.", 1, 8, {5, 5, 5, 5}, 0},
        {"Українка Л.", 2, 9, {4, 5, 5, 4}, 0},
        {"Костенко Л.", 1, 8, {3, 3, 4, 3}, 0}
    };

    // Overload
    RW("INFO.DAT", datStudents, count);
    RW("INFO.TXT", txtStudents, count);

    // Overload reading
    RD("INFO.DAT", StudentDat());
    RD("INFO.TXT", StudentTxt());

    cout << "\nРоботу програми завершено. Натисніть Enter для виходу.";
    cin.get();
}