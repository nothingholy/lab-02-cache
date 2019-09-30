//
// Created by nothingholy on 30.09.2019.
//
#include <cache.h>

void toString(string& str,int iter,string name,int64_t time,int bufSize){
    str+= "investigation:\n\ttravel_variant: " + name + "\n\t- experiments:\n\t\tnumber: " + to_string(iter) +
          "\n\t\tinput_data:\n\t\t\tbuffer_size: " + to_string(bufSize) + "\n\t\tresults:\n\t\t\tduration: " + to_string(time) + "\n";
}


CacheExp::CacheExp(int min, int max) {
    size=log2(max/min)+3;
    sizes.reserve(size);
    int iter=0;
    while(min<=max*3/2){
        sizes[iter]=min;
        min *=2;
        iter++;
    }
    sizes[iter]=max*3/2;
    buffer={};
}

int CacheExp::toSize(int kbSize) {
    return kbSize*1024/ sizeof(int);
}

void CacheExp::initBuffer(int sizeBuffer){
    if(!buffer.empty())
        buffer.clear();
    for(int i=0;i<sizeBuffer;i++)
        buffer[i]=rand()%101;
}

void CacheExp::warmingUp(int sizeBuffer){
    int temp=0;
    for(int i=0;i<sizeBuffer;i++){
        temp=buffer[i];
    }
    temp++;
}

void CacheExp::straight(int sizeBuffer){
    int temp=0;
    for(int j=0;j<iterations;j++) {
        for (int i = 0; i < sizeBuffer; i++) {
            temp = buffer[i];
        }
    }
    temp++;
}

void CacheExp::back(int sizeBuffer) {
    int temp=0;
    for(int i=0;i<iterations;i++){
        for(int j=sizeBuffer-1;j>=0;j--){
            temp=buffer[j];
        }
    }
    temp++;
}

void CacheExp::random(int sizeBuffer) {
    int temp=0;
    for(int i=0;i<iterations;i++){
        for(int j=0;j<sizeBuffer;j++){
            temp=buffer[rand()%sizeBuffer];
        }
    }
    temp++;
}

void CacheExp::start() {
    string str="";
    for(int i=0;i<size;i++){
        int temp=sizes[i];
        initBuffer(temp);
        warmingUp(temp);

        auto time1=chrono::high_resolution_clock::now();
        straight(temp);
        auto time2=chrono::high_resolution_clock::now();
        auto time=chrono::duration_cast<chrono::microseconds>(time1-time2).count();
        Data data("Forward",i+1,time);
        experiments.push_back(data);
        toString(str,i+1,"Straight",time,temp);

        time1=chrono::high_resolution_clock::now();
        back(temp);
        time2=chrono::high_resolution_clock::now();
        time=chrono::duration_cast<chrono::microseconds>(time1-time2).count();
        Data data1("Back",i+1,time);
        toString(str,i+1,"Back",time,temp);

        time1=chrono::high_resolution_clock::now();
        random(temp);
        time2=chrono::high_resolution_clock::now();
        time=chrono::duration_cast<chrono::microseconds>(time1-time2).count();
        Data data2("Random",i+1,time);
        toString(str,i+1,"Back",time,temp);
    }
    ofstream loi;
    loi.open("..\\result\\res.txt");
    if(loi.is_open())
        loi << str << endl;
}