#include "db/api.h"

#include <iostream>
#include <chrono>

#ifdef __unix__

#elif _WIN32

#endif // defined


#undef OUTPUT

using namespace std;
using namespace std::chrono;


int main(int argi,char** argc){
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    IncreaseDB* db = new IncreaseDB((char*)"testdb");
    db->init(0);
    for (int ii = 0;ii<20;ii++){
    int i_cr = db->create_entry();
    #ifdef OUTPUT
    cout << " create entry : " << i_cr << endl;
    #endif // OUTPUT
    Id i = 0;
    int i_o =  db->open_entry(&i,0);
    #ifdef OUTPUT
    cout << " open entry : " << i_o << endl;
    #endif // OUTPUT
    int i_ph = db->put_head("hello test user",15);
    #ifdef OUTPUT
    cout << " put head  : " << i_ph << endl;
    #endif // OUTPUT
    char* data = new char[8];
    for (int i = 0;i<8;i++)
        *(data+i) = 'a';
    int i_pd = db->put_data(8,data);
    #ifdef OUTPUT
    cout << " put data  : " <<  i_pd << endl;
    #endif // OUTPUT
    int i_cl = db->close_entry();
    #ifdef OUTPUT
    cout << " close entry: " <<  i_cl << endl;
    #endif // OUTPUT
    }


    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

    std::cout << " EXECUTION TIME " << time_span.count() << " seconds.";
    return 0;
}
