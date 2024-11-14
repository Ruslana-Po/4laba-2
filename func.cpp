#include "haeder.h"


mutex mtx;

void One(){
    //объем данных
    int size = 1000000;

    //вектор структуры сотрудников
    vector<Intelligence> staff;

    //заполняем данные
    RandomData(staff, size);

    // время начало
    auto start = chrono::high_resolution_clock::now();
    //Средняя зарплата по отделам
    vector<double> avgSalaries(10, 0.0);
    averageSalary(staff, avgSalaries, 0, size);

    //кол по отделам
    vector<Intelligence> result;
    findKolPeople(staff, avgSalaries, result, 0, size);

    //Завершила время
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> time1 = end - start;
    
   /* for (const auto& bigAS : result) {
        cout << bigAS.fio << " " << bigAS.position<<"(" << bigAS.department << ") - " << bigAS.salary << "\n";
    }*/
    cout << "---Без использования многопоточности: " << time1.count() << " c."<<endl;
}

void two(){
    //объем данных
    int size = 100000;
    //Кол-во потоков
    int numThreads = 4;

    //вектор структуры сотрудников
    vector<Intelligence> staff;

    //заполняем данные
    RandomData(staff, size);

    // время начало
    auto start = chrono::high_resolution_clock::now();
    //Средняя зарплата по отделам
    vector<double> avgSalaries(10, 0.0);

    //кол по отделам
    vector<Intelligence> result;
    //потоки
    //vector<thread> threads;
    //раздилила на рвные части
    int part = size / numThreads;
    int starts=0;
    //сред
    for (int i = 0; i < numThreads; ++i) {
        //откуда начинаем и где заканчиваем
        starts = i * part;
        int ends;
        if((i+1)==numThreads){
            ends=size;
        }else{
            ends=starts+part;
        }
        thread threads(averageSalaryTwo, ref(staff), ref(avgSalaries),starts, ends);
        threads.join();
    }

    starts=0;
    //кол
    for (int i = 0; i < numThreads; ++i) {
        starts = i * part;
        int ends;
        if((i+1)==numThreads){
            ends=size;
        }else{
            ends=starts+part;
        }
        thread threads(findKolPeople, ref(staff), ref(avgSalaries), ref(result), starts, ends);
        threads.join();
    }
    
    

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> time2 = end - start;
   

    // Результаты больше среднего
    cout << "Результат:\n";
    cout<<result.size()<<endl;
   /* for (const auto& bigAS : result) {
        cout << bigAS.fio << " " << bigAS.position<<"(" << bigAS.department << ") - " << bigAS.salary << "\n";
    }*/
    cout << "с использованием многопоточности: " << time2.count() << " с."<<endl;

}

void RandomData(vector<Intelligence>& staff, int size) {
    random_device rd;
    mt19937 gen(rd());
    //ФИО
    vector<string> namesFio = {"Костя А. Л.", "Андрей Е. З.", "Руслана П. А.", "Юлиана Ю. А.", "Ева А. Р.", "Влад П. К.", "Даша К. К.", "Юля С. В."};
    uniform_int_distribution<> named(0, 7);
    //должность
    uniform_int_distribution<> pos(0, 6);
    vector<string> positions = {"Программист", "Учитель", "Бомж", "Стажер", "Хакер", "Водитель", "Повар"};
    //отдел
    uniform_int_distribution<> depar(1, 10);
    //денюшки)))))))))
    uniform_real_distribution<> sal(25000, 100000);
    //заполняем
    for (int i = 0; i < size; ++i) {
        staff.push_back({
            namesFio[size_t(named(gen))],
            positions[size_t(pos(gen))],
            depar(gen),
            sal(gen)
        });
    }
}

void averageSalary(const vector<Intelligence>& staff, vector<double>& avgSalaries, int start, int end) {

    vector<int> kolStaff(10, 0);
    //Общая сумма
    vector<double> departmentSum(10, 0.0);

    //идем полностью по всем сотрудникам
    for (int i = start; i < end; ++i) {
        // минус 1 тк 0
        int dep = staff[size_t(i)].department - 1;
        kolStaff[size_t(dep)]++;
        departmentSum[size_t(dep)] += staff[size_t(i)].salary;
    }

    //lock_guard<mutex> lock(mtx);
    mtx.lock();
    //считаю среднее
    for (size_t i = 0; i < 10; ++i) {
        if (kolStaff[i] > 0) {
            avgSalaries[i] += departmentSum[i] / kolStaff[i];
        }
    }
    mtx.unlock();
}

void averageSalaryTwo(const vector<Intelligence>& staff, vector<double>& avgSalaries, int start, int end) {

    vector<int> kolStaff(10, 0);
    //Общая сумма
    vector<double> departmentSum(10, 0.0);

    //идем полностью по всем сотрудникам
    for (int i = start; i < end; ++i) {
        // минус 1 тк 0
        int dep = staff[size_t(i)].department - 1;
        kolStaff[size_t(dep)]++;
        departmentSum[size_t(dep)] += staff[size_t(i)].salary;
    }

    //lock_guard<mutex> lock(mtx);
    mtx.lock();
    //считаю среднее
    for (size_t i = 0; i < 10; ++i) {
        if (kolStaff[i] > 0) {
            avgSalaries[i] += departmentSum[i] / kolStaff[i];
            if(end==1000000){
                avgSalaries[i]/=4;
            }
        }
    }
    
    mtx.unlock();
}

void findKolPeople(const vector<Intelligence>& staff, const vector<double>& avgSalaries, vector<Intelligence>& result, int start, int end) {
    for (int i = start; i < end; ++i) {
        int dep = staff[size_t(i)].department - 1;
        if (staff[size_t(i)].salary > avgSalaries[size_t(dep)]) {
            //lock_guard<mutex> lock(mtx);
            mtx.lock();
            result.push_back(staff[size_t(i)]);
            mtx.unlock();
        }
    }
}
