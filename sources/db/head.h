#ifndef HEAD_H_INCLUDED
#define HEAD_H_INCLUDED

#include <iostream>
#include <io.h>
#include "strings.h"

#include "defines.h"
#include "utils.h"

#ifdef DEBUG
#define HEAD_DEBUG
#endif // DEBUG

namespace DB{
    class HeadManager{
    private:
        File* _H;
        Address _HeadS;
        Address _HeadC;
        Address _HeadE;
        Id _LastId;
        Id _CurrentId;
        Time _CreationTime;
        Address* _Data;
    public:
        HeadManager(db_info info);
        ~HeadManager();
        int init(File* head);
        Id create_entry();
        int open_entry(Id* entry,Address* addr);
        int close_entry(){
            _CurrentId = -1;
            _HeadS = -1;
            return 0;
        }
        Address* get_data_addresses(){
            if (!_Data) _Data = new Address[2];
            fseek(_H,_HeadS+USER_HEAD_SIZE-2*sizeof(long),0);
            fread(_Data,ADDR_SIZE,2,_H);
            return _Data;
        }
        int put_head(char* head,int size);
        int put_head(char* head){
              return  put_head(head,strlen(head));
        };
        int get_entries(Id* array);
        int update_address_on_create_ds(Address* addr);
        int update_address_on_close_ds(Address* addr);
        Address* get_head_start(){return &_HeadS;}
        Address* get_head_end(){return &_HeadE;}
        File* get_file(){return _H;}
        Id* get_current_entry(){
            return &_CurrentId;}
    };
}

#endif // HEAD_H_INCLUDED
