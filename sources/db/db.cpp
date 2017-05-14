#include "db/db.h"

using namespace DB;
using namespace std;

IncreaseDB::IncreaseDB(char* db_name){
    name = db_name;
    #ifdef DB_DEBUG
    cout << "db '" << name << "' created\n";
    #endif
}

IncreaseDB::~IncreaseDB(){
    #ifdef DB_DEBUG
    cout << "db '" << name << "' destroyed\n";
    #endif
}


int IncreaseDB::init(char* options){
    int name_length = strlen(name);
    char* data = new char[name_length+4];
    char* head = new char[name_length+4];
    strcpy(data,name);
    strcpy(data+name_length,".dbd");
    strcpy(head,name);
    strcpy(head+name_length,".dbh");
    bool access_head = access(head,0);
    bool access_data = access(data,0);
    cout << access_head << access_data << endl;
    if ( (access_head!=1) && (access_data !=1) ){
        _H = fopen(head,"r+b");
        _D = fopen(data,"r+b");
    }else{
        if ( (access_data==1) && (access_head==1) ){
            _H = fopen(head,"w+b");
            _D = fopen(data,"w+b");
        }else{
            return ( (access_head==-1)?ERROR_IO_NO_ACCESS_TO_HEAD_FILE:ERROR_IO_NO_ACCESS_TO_DATA_FILE );
        }
    }
    long size;
    fseek(_H, 0, SEEK_END);
    //size = _fHeaders->_bufsiz;
    size = ftell(_H);
    //cout << "size : " << size << std::endl;

    db_info* info = new db_info();
    info->is_first_creation = (size==0?true:false);
    //info->creation_time
   // fseek(_H,0,0);
   // long last = 0;
   // if (size==0){
    //    last = -1;
        //fwrite(&last,sizeof(last),1,_fHeaders);
   // }else{
        //fread(&last,sizeof(last),1,_fHeaders);
   // }


    _hm = new HeadManager(*info);
    _dm = new DataManager(*info);

    //_UM = new DB::UserManager((size==0?true:false));
    if (!_hm) return ERROR_INTERNAL_UNABLE_TO_CREATE_HEAD_MANAGER;
    if (!_dm) return ERROR_INTERNAL_UNABLE_TO_CREATE_DATA_MANAGER;
    //_UM->bind_files(_fHeaders,_fData);
    _hm->init(_H);
    _dm->init(_D);
    _dm->bind_head_manager(_hm);
    cout << "db '" << name << "' initialized with code (" << size << ")\n";
    //if (size==0) cout << "     first launch\n";
    return 0;
}


int IncreaseDB::free(){
    int ret = 0;
    delete _hm;
    delete _dm;
    #ifdef DB_DEBUG
    cout << "db closed with code (" << ret << ")\n";
    #endif // DB_DEBUG
    return 0;
}
