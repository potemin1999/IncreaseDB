#include "utils.h"

using namespace std;



long UTILS::get_size(File* f){
    fseek(f, 0, SEEK_END);
    return ftell(f);
}


/**returns user head address*/
long UTILS::get_head_by_id(File* _H,Id* _id){
    long count = (get_size(_H)-HEAD_SIZE)/USER_HEAD_SIZE;
    cout << "      utils c[" << count << "] id[" << *_id << "] " << endl;
    long _head_s;
    Id* id = new Id;
    for (long i = 0; i<count; i++){
        fseek(_H,HEAD_SIZE+i*USER_HEAD_SIZE,0);
        fread(id,4,1,_H);
        cout << "      compare ("<<*id<<"=="<<*_id<<")="<<(*id==*_id)<<endl;
        if (*id==*_id){
            _head_s = HEAD_SIZE+i*USER_HEAD_SIZE;
            char* name = new char[USER_HEAD_SIZE-12];
            //fseek(_H,4,1);
            fread(name,1,USER_HEAD_SIZE-12,_H);
            printf(name);
            return _head_s;
        }
    }
    return ERROR_ENTRY_NOT_FOUND;
}


long UTILS::time(db_info* info){
    return std::time(0) - info->creation_time;
}


/** writes (count) empty bytes in (f) file */
int UTILS::write_empty_bytes(int count,File* f){
    char* empty = new char[count];
    for(int i = 0;i<count;i++) empty[i]=0;
    return fwrite(empty,sizeof(char),count,f);
}
