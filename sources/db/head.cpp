#include "head.h"

using namespace UTILS;
using namespace DB;
using namespace std;

HeadManager::HeadManager(db_info info){
    #ifdef HEAD_DEBUG
    cout << "Head Manager created\n";
    #endif // HEAD_DEBUG
    _CurrentId = -1;
    _HeadS = -1;
    _Data = new Address[2];
    if (info.is_first_creation){
        _LastId=-2;
    }else{
        _LastId=-1;
    }
}

HeadManager::~HeadManager(){
    #ifdef HEAD_DEBUG
    cout << "Head Manager destroyed\n";
    #endif // HEAD_DEBUG
}

int HeadManager::init(File* head){
    if (!head) return ERROR_IO_HEAD_FILE_NOT_BINDED;
    _H = head;
    if (_LastId==-2){
        fseek(_H,0,SEEK_SET);
        write_empty_bytes(HEAD_SIZE,_H);
        fseek(_H,0,SEEK_SET);
        fwrite( &(++_LastId),sizeof _LastId,1,_H);
        long create_time = std::time(0)-1;
        fwrite(&create_time,sizeof create_time,1,_H);
    }else{
        fseek(_H,0,SEEK_SET);
        fread(&(_LastId),sizeof _LastId,1,_H);
        cout << "read last id : " << _LastId << std::endl;
    }
    fseek(_H,sizeof(Id),SEEK_SET);
    fread(&_CreationTime,TIME_SIZE,1,_H);
    return 0;
}

Id HeadManager::create_entry(){
    fseek(_H,0,SEEK_SET);
    fwrite(&(++_LastId),sizeof(_LastId),1,_H);
    fseek(_H,0,SEEK_END);
    fwrite(&_LastId,sizeof(_LastId),1,_H);

    write_empty_bytes(USER_HEAD_SIZE-sizeof(_LastId)-2*sizeof(long),_H);
    long* a = new long[2]{-1,-1};
    fwrite(a,sizeof(long),2,_H);

    fseek(_H,0,SEEK_END);
    fread(&_LastId,sizeof(_LastId),1,_H);

    return _LastId;
}

int HeadManager::open_entry(Id* entry,Address* addr){
    _HeadS = get_head_by_id(_H,entry);
    if (_HeadS==ERROR_ENTRY_NOT_FOUND){
        _CurrentId = -1;
        return ERROR_ENTRY_NOT_FOUND;
    }
    //moving to address block in the segment
    fseek(_H,_HeadS+USER_HEAD_SIZE-2*sizeof(long),0);
    //reading adress
    fread(addr,ADDR_SIZE,2,_H);
    //get_data_addresses();
    _CurrentId = *entry;
    return *entry;
}

int HeadManager::put_head(char* head,int size){
    if (_CurrentId==-1) return ERROR_ENTRY_NEGATIVE_ID;
    if (_HeadC==-1) return ERROR_HEAD_INVALID_CURRENT_BLOCK_POINTER;
    fseek(_H,_HeadS,0);
    int max = USER_HEAD_SIZE-3*sizeof(long);
    fseek(_H,sizeof(long),1);
    fwrite(head,sizeof(char),(size>max?max:size),_H);
    return 0;
}

int HeadManager::get_entries(Id* array){
    long count = (get_size(_H)-HEAD_SIZE)/USER_HEAD_SIZE;
    array = new Id[count];
    for (long i = 0;i<count;i++){
        long id = 0;
        fseek(_H,HEAD_SIZE+i*USER_HEAD_SIZE,0);
        fread(&id,sizeof(id),1,_H);
        cout << "id[" << i << "]: " << id << std::endl;
    }
    return 0;
}

int HeadManager::update_address_on_create_ds(Address* addr){
    if ((addr[0]==addr[1])&(addr[0]==-1)){
        /* if there are no data segments created yet */
        #ifdef HEAD_DEBUG
        cout << "   creating data segment\n";
        #endif // DEBUG
        fseek(_H,_HeadS+USER_HEAD_SIZE-2*sizeof(long),SEEK_SET);
        fwrite(addr,ADDR_SIZE,1,_H);
    }
    /* set last address as zero to symbolize an unclosed session */
    fseek(_H,_HeadS+USER_HEAD_SIZE-sizeof(long),SEEK_SET);
    write_empty_bytes(ADDR_SIZE,_H);
}

int HeadManager::update_address_on_close_ds(Address* addr){
    /* updating headers */
    fseek(_H,_HeadS+USER_HEAD_SIZE-sizeof(long),SEEK_SET);
    fwrite(addr,ADDR_SIZE,1,_H);
}

