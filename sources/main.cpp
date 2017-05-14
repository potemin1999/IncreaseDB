#include "db/api.h"

#include <iostream>

using namespace std;

int main(int argi,char** argc){
    IncreaseDB* db = new IncreaseDB((char*)"people");
    db->init(0);
    cout << " create entry : " << db->create_entry() << endl;
    Id i = 2;
    cout << " open entry : " << db->open_entry(&i,0) << endl;
    cout << " put head  : " << db->put_head("hello test user",15) << endl;
    char* data = new char[8];
    for (int i = 0;i<8;i++)
        *(data+i) = 'a';
    cout << " put data  : " << db->put_data(8,data) << endl;
    cout << " close entry: " << db->close_entry() << endl;
    //db->create_entry();
    return 0;
}
