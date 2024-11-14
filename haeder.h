#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;

struct Intelligence {
    string fio;
    string position;
    int department;
    double salary;
};


void two();
void One();
void RandomData(vector<Intelligence>& staff, int size);
void averageSalary(const vector<Intelligence>& staff, vector<double>& avgSalaries, int start, int end);
void findKolPeople(const vector<Intelligence>& staff, const vector<double>& avgSalaries, vector<Intelligence>& result, int start, int end);
void averageSalaryTwo(const vector<Intelligence>& staff, vector<double>& avgSalaries, int start, int end);
