//
// Created by nothingholy on 30.09.2019.
//
#include <cache.h>

void toString(string& str,int iter,string name,int64_t time,int bufSize){
    str+= "investigation:\n\ttravel_variant: " + name + "\n\t- experiments:\n\t\tnumber: " + to_string(iter) +
          "\n\t\tinput_data:\n\t\t\tbuffer_size: " + to_string(bufSize) + "\n\t\tresults:\n\t\t\tduration: " + to_string(time) + "\n";
}

std::string experimentDataToHtmlString(const vector<int> &sizes, const vector<Data> &data)
{
    std::string result = R"(<html>
        <head>
          <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
            <script type="text/javascript">
              google.charts.load('current', {'packages':['corechart']});
              google.charts.setOnLoadCallback(drawChart);
            function drawChart() {
              var data = new google.visualization.DataTable();
              data.addColumn('number', 'X');
        )";

    std::vector<std::string> columnNames = {
            "Forward",
            "Backward",
            "Random",
    };

    std::vector<size_t> bufferSizes(sizes.cbegin(), sizes.cend());
    std::vector<std::vector<size_t>> rows;

    for (size_t i = 0; i < data.size(); i++) {
        if (i % 3 == 0) {
            rows.emplace_back();
        }

        rows[i / 3].emplace_back(data[i].time);
    }

    // Do not touch it
    for (const std::string &column : columnNames) {
        result += "data.addColumn('number', '" + column + "');\n";
    }

    result += "\ndata.addRows([\n";
    for (size_t i = 0; i < bufferSizes.size(); i++) {
        result += "[" + std::to_string(bufferSizes[i]) + ", ";

        for (size_t value : rows[i]) {
            result += std::to_string(value) + ", ";
        }

        result += "],\n";
    }
    result += "]);\n";


    result += R"(
              var options = {
                chart: {
                  title: 'CPU cache',
                },
                hAxis: {
                  title: 'Buffer size, KiB',
                  logScale: false
                },
                vAxis: {
                  title: 'Time, milliseconds',
                  logScale: false
                },
                explorer: {
                  axis: 'horizontal',
                  keepInBounds: true,
                  maxZoomIn: 8.0,
                },
                width: 1200,
                height: 800,
              };
              var chart = new google.visualization.LineChart(document.getElementById('line_top_x'));
              chart.draw(data, options);
            }
          </script>
        </head>
        <body>
          <div id="line_top_x"></div>
        </body>
        </html>
        )";


    return result;
}


CacheExp::CacheExp(int min, int max) {
    size=log2(max/min)+3;
    sizes.resize(size);
    int iter=0;
    while(min<=max*3/2){
        sizes[iter]=min;
        min *=2;
        iter++;
    }
    sizes[iter]=max*3/2;
    buffer={};
}

void CacheExp::cicle(int key,int sizeBuffer) {
    if(key==1){
        for(int i=0;i<iterations;i++)
            straight(sizeBuffer);
    }
    else if(key==2){
        for(int i=0;i<iterations;i++)
            back(sizeBuffer);
    }
    else if(key==3){
        for(int i=0;i<iterations;i++)
            random(sizeBuffer);
    }

}

int CacheExp::toSize(int kbSize) {
    return kbSize*1024/ sizeof(int);
}

void CacheExp::initBuffer(int sizeBuffer){
    if(!buffer.empty())
        buffer.clear();
    buffer.resize(sizeBuffer);
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
    for (int i = 0; i < sizeBuffer; i++) {
        temp = buffer[i];
    }
    temp++;
}

void CacheExp::back(int sizeBuffer) {
    int temp=0;
    for(int j=sizeBuffer-1;j>=0;j--){
        temp=buffer[j];
    }
    temp++;
}

void CacheExp::random(int sizeBuffer) {
    int temp=0;
    for(int j=0;j<sizeBuffer;j++){
        temp=buffer[rand()%sizeBuffer];
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
        cicle(1,temp);
        auto time2=chrono::high_resolution_clock::now();
        auto time=chrono::duration_cast<chrono::microseconds>(time2-time1).count();
        Data data("Forward",i+1,time);
        experiments.push_back(data);
        toString(str,i+1,"Straight",time,temp);

        time1=chrono::high_resolution_clock::now();
        cicle(2,temp);
        time2=chrono::high_resolution_clock::now();
        time=chrono::duration_cast<chrono::microseconds>(time2-time1).count();
        Data data1("Back",i+1,time);
        experiments.push_back(data1);
        toString(str,i+1,"Back",time,temp);

        time1=chrono::high_resolution_clock::now();
        cicle(3,temp);
        time2=chrono::high_resolution_clock::now();
        time=chrono::duration_cast<chrono::microseconds>(time2-time1).count();
        Data data2("Random",i+1,time);
        experiments.push_back(data2);
        toString(str,i+1,"Back",time,temp);
    }


    ofstream loi{"../result/res.txt"};
    if(loi.fail()) {
        std::cout << "Some error when asdasdas\n";
        return;
    }

    loi << str << endl;

    ofstream lou{"../result/res.html"};
    if(lou.fail()) {
        std::cout << "Some error when asdasdas\n";
        return;
    }

    lou << experimentDataToHtmlString(sizes, experiments) << endl;
}