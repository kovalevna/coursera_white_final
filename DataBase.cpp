#include <iostream>
#include <exception>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>

using namespace std;

class Date {
public:
    Date() {};
    Date(int y, int m, int d) {
        if (m < 1 || m>12) {
            throw invalid_argument("Month value is invalid: " + to_string(m));

        }
        else if (d < 1 || d>31) {
            throw invalid_argument("Day value is invalid: " + to_string(d));
        }
        year = y;
        month = m;
        day = d;
    }
    int GetYear() const {
        return year;
    }
    int GetMonth() const {
        return month;
    }
    int GetDay() const {
        return day;
    }    
private:
    int year;
    int month;
    int day;
};

ostream& operator<<(ostream& stream, const Date& date) {
    stream << setw(4) << setfill('0') << date.GetYear() << "-"
        << setw(2) << setfill('0') << date.GetMonth() << "-"
        << setw(2) << setfill('0') << date.GetDay();
    return stream;
}

Date ParseDate(const string& s) {
    stringstream stream(s);
    int y, m, d;
    string check;
    char ch1, ch2;
    stream >> y >> ch1 >> m >> ch2 >> d ;
    if (!stream||ch1 != '-' || ch2 != '-' || stream.peek()!=EOF) {
        throw runtime_error("Wrong date format: "+s);
    }
    return {y,m,d};
}

bool operator<(const Date& lhs, const Date& rhs) {
    if (lhs.GetYear() == rhs.GetYear()) {
        if (lhs.GetMonth() == rhs.GetMonth()) {
            return lhs.GetDay() < rhs.GetDay();
        }
        else {
            return lhs.GetMonth() < rhs.GetMonth();
        }
    }
    else {
        return lhs.GetYear() < rhs.GetYear();
    }
}

class Database {
public:
    void AddEvent(const Date& date, const string& event) {
        if (dates[date].count(event) == 0) {
            dates[date].insert(event);
        }
    };
    bool DeleteEvent(const Date& date, const string& event) {
        if (dates[date].count(event) == 0) {
            return false;
        }
        else {
            dates[date].erase(dates[date].find(event));
            return true;
        }
    }
    int DeleteDate(const Date& date) {
        int n = dates[date].size();
        dates.erase(dates.find(date));
        return n;
    }
    set<string> Find(const Date& date) const {
        set<string> ss;
        if (dates.count(date) > 0) {
            return dates.at(date);
        }
        else {
            return ss;
        }
    }
    void Print() const {
        for (const auto& ds : dates) {
            for (const auto& d : ds.second) {
                cout << ds.first << " " << d << endl;
            }
        }
    }
private:
    map<Date, set<string>> dates;
};


int main() {
    Database db;
    string line;
    while (getline(cin, line)) {
        stringstream stream(line);
        string command;
        stream >> command;
        if (command == "Add") {
            Date date;
            string dat;
            string event;
            try {
                stream >> dat >> event;
                date = ParseDate(dat);
            }
            catch (runtime_error& ex) {
                cout << ex.what();
                return 0;
            } catch(invalid_argument& ex) {
                cout << ex.what();
                return 0;
            }
            db.AddEvent(date, event);
        }
        else if (command == "Del") {
            Date date;
            string event,dat;
            try{
                stream >> dat >> event;
                date = ParseDate(dat);
            }
            catch (runtime_error& ex) {
                cout << ex.what();
                return 0;
            }
            catch (invalid_argument& ex) {
                cout << ex.what();
                return 0;
            }
            if (event.size()==0) {
                cout<<"Deleted "<<db.DeleteDate(date)<<" events"<<endl;
            }
            else {
                if (db.DeleteEvent(date, event)) {
                    cout << "Deleted successfully" << endl;
                }
                else {
                    cout << "Event not found" << endl;
                }
            }
        }
        else if (command == "Find") {
            Date date;
            string dat;
            try{
                stream >> dat;
                date = ParseDate(dat);
            }
            catch (runtime_error& ex) {
                cout << ex.what();
                return 0;
            }
            catch (invalid_argument& ex) {
                cout << ex.what();
                return 0;
            }
            for (const auto& c : db.Find(date)) {
                cout << c << endl;
            }
        }
        else if(command == "Print") {
            db.Print();
        }
        else if (command.size()>0){
            cout << "Unknown command: " << command<<endl;
        }
    }
    return 0;
}
