#include "data.h"

using namespace UTILS;
using namespace DB;
using namespace std;

DataManager::DataManager(db_info info_){
    #ifdef DATA_DEBUG
    cout << "Data Manager created\n";
    #endif // DATA_DEBUG
    info = info_;
}

DataManager::~DataManager(){
    #ifdef DATA_DEBUG
    cout << "Data Manager destroyed\n";
    #endif // DATA_DEBUG
}

int DataManager::init(File* data){
    if (!data) return ERROR_IO_DATA_FILE_NOT_BINDED;
    _D = data;
    return 0;
}

int DataManager::open_entry(Id* entry,Address* data){
     _DataOpened = false;
    //if first==last address
    if (*(data)==*(data+1)){
        if (*data = -1){
            //data have not created yet for this user
            //No info, to create first data block
            _DataS = 0;
            _DataE = 0;
            _DataP = 0;
            create_data_segment();
        }
    }
    return 0;
}




Address DataManager::create_data_segment(){
    if (_DataOpened) return ERROR_DATA_ALREADY_OPEN;
    Address _HeadS = *(_hm->get_head_start());
    File* _H = _hm->get_file();
    /* get data segments first address */
    fseek(_D,0,SEEK_END);
    _DataS = ftell(_D);
    /* get first address of first and last data segments */
    fseek(_H,_HeadS+USER_HEAD_SIZE-2*sizeof(long),SEEK_SET);
    long* addr = new long[2];
    fread(addr,sizeof(long),2,_H);

    /* creating data head */
    fwrite(_hm->get_current_entry(),sizeof(Id),1,_D);
    fwrite(&_DataS,sizeof _DataS,1,_D);
    _DataSegmentTimeS = time(&info);
    fwrite(&_DataSegmentTimeS,sizeof _DataSegmentTimeS,1,_D);
    write_empty_bytes(8,_D);

    /* updating headers */
    #ifdef DATA_DEBUG
    cout << "   address = [ " << addr[0] << " ; " << addr[1] << " ]\n";
    #endif // DATA_DEBUG
    if ((addr[0]==addr[1])&(addr[0]==-1)){
        /* if there are no data segments created yet */
        #ifdef DATA_DEBUG
        cout << "   creating data segment\n";
        #endif // DATA_DEBUG
        fseek(_H,_HeadS+USER_HEAD_SIZE-2*sizeof(long),SEEK_SET);
        fwrite(&_DataS,sizeof _DataS,1,_H);
    }else{
        /* set data segments connected */
        fseek(_D,*(addr+1)+16,SEEK_SET); //gets size
        long* size = new long;
        fread(size,sizeof size,1,_D);
        #ifdef DATA_DEBUG
        cout << "   last data segment size : " << *size << "\n";
        #endif // DATA_DEBUG
        fseek(_D,*size,SEEK_CUR);  //moves to last segment last address
        fwrite(&_DataS,sizeof _DataS,1,_D); //connect it to current segment
        #ifdef DATA_DEBUG
        cout << "   connected address = [ " << addr[0] << " ; " << _DataS << " ]\n";
        #endif // DATA_DEBUG
    }
    /* set last address as zero to symbolize an unclosed session */
    fseek(_H,_HeadS+USER_HEAD_SIZE-sizeof(long),SEEK_SET);
    write_empty_bytes(sizeof(_DataS),_H);
    _DataOpened=true;
    return _DataS;
}

long DataManager::close_data_segment(){
    if (!_DataOpened) return ERROR_DATA_ALREADY_CLOSED;

    /* writing closing time */
    fseek(_D,_DataS+12,SEEK_SET);
    long time_l = time(&info);
    fwrite(&time_l,sizeof(long),1,_D);
    /* writing size */
    long size = get_size(_D) - _DataS - 20;
    fseek(_D,_DataS+16,SEEK_SET);
    fwrite(&size,sizeof(long),1,_D);
    /* creating finishing block */
    fseek(_D,0,SEEK_END);
    fwrite(&size,sizeof(long),1,_D);
    //write_empty_bytes(sizeof _DataS,_D);

    /* updating headers */
    Address _HeadS = *(_hm->get_head_start());
    File* _H = _hm->get_file();
    fseek(_H,_HeadS+USER_HEAD_SIZE-sizeof(long),SEEK_SET);
    fwrite(&_DataS,sizeof(long),1,_H);

    _DataP = _DataS;
    _DataS = -1;
    _DataSegmentStart = -1;
    _DataSegmentTimeE = -1;
    _DataSegmentTimeS = -1;
    _DataOpened = false;
    return ftell(_D);
}

int DataManager::put_data(char type,char* name,long int nsize,void* data, long int dsize){

}

int DataManager::put_data(long size,char* data){
    if (!_hm->get_current_entry()) return ERROR_ENTRY_NEGATIVE_ID;
    if (!_DataOpened) create_data_segment();
    if (_DataS==-1) return ERROR_DATA_INVALID_START_BLOCK_POINTER;
    fseek(_D,_DataS+20,SEEK_SET);
    fwrite(data,1,size,_D);
    return _DataS;
}

