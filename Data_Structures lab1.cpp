#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <chrono>

const int SIZE = 800;
std::string DATASET_FILE = "dataset-titanic.csv";

void SelectionSort(std::string arr[]) {
    auto temp = arr[0];
    for (int i = 0; i < SIZE; i++) {
        for (int j = i; j < SIZE; j++) {
            if (arr[i] > arr[j]) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

void EvenOddSort(std::string arr[]) {
    bool changed = true;
    auto temp = arr[0];
    while (changed) {
        changed = false;
        for (int i = 0; i <= SIZE - 2; i = i + 2) { //odd
            if (arr[i] > arr[i + 1]) {
                temp = arr[i];
                arr[i] = arr[i+1];
                arr[i+1] = temp;
                changed = true;
            }
        }
        for (int i = 1; i <= SIZE - 2; i = i + 2) { //even
            if (arr[i] > arr[i + 1]) {
                temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                changed = true;
            }
        }
    }
}

int main() {
    std::fstream file;
    file.open(DATASET_FILE, std::ios::in);
    std::string lines[SIZE] = {};    //array for simple algorithm
    if (!file.is_open())
    {
        std::cout << "There was en error opening file\n";
        return 1;
    }
    int len = sizeof(lines) / sizeof(lines[0]);
    if (len > SIZE) {
        len = SIZE;
    }
    for (int i = 0; i < len; i++)
    {
        std::getline(file, lines[i]);
    }
    
    //https://stackoverflow.com/a/21856299
    auto start = std::chrono::high_resolution_clock::now();
    SelectionSort(lines);
    auto time = std::chrono::high_resolution_clock::now() - start;
    long long timeSimple = std::chrono::duration_cast<std::chrono::microseconds>(time).count();

    start = std::chrono::high_resolution_clock::now();
    EvenOddSort(lines);
    time = std::chrono::high_resolution_clock::now() - start;
    long long timeQuick = std::chrono::duration_cast<std::chrono::microseconds>(time).count();

    std::cout << "Selection Sort: " << timeSimple << " microseconds\nEven\\Odd Sort: " << timeQuick << " microseconds\n";
    system("pause");
}