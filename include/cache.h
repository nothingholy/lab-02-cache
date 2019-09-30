//
// Created by nothingholy on 30.09.2019.
//

#ifndef LAB_02_CACHE_CACHE_H
#define LAB_02_CACHE_CACHE_H

#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
using namespace std;


class Data{
public:
    string name;
    int id;
    int64_t time;
    Data(string name1,int id1,int64_t time1){
        name=name1;
        id=id1;
        time=time1;
    }
};

class CacheExp {
private:
    const int iterations=1000;
    vector<int> buffer;
    vector<int> sizes;
    int size;
    vector<Data> experiments;
public:
    CacheExp(int min,int max);
    int toSize(int kbSize);
    void initBuffer(int sizeBuffer);
    void warmingUp(int sizeBuffer);
    void straight(int sizeBuffer);
    void back(int sizeBuffer);
    void random(int sizeBuffer);
    void start();

};


#endif //LAB_02_CACHE_CACHE_H
