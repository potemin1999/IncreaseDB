#ifndef DB_H_INCLUDED
#define DB_H_INCLUDED

#include <cstdio>
#include <ctime>
#include <io.h>
#include <iostream>
#include "string.h"

#include "defines.h"
#include "head.h"
#include "data.h"
#include "utils.h"

#ifdef DEBUG
#define DB_DEBUG
#endif // DEBUG



namespace DB{
    class IncreaseDB{
    private:
        HeadManager* _hm;
        DataManager* _dm;
        File* _H;
        File* _D;
        char* name;
        bool entry_opened=false;
    public:
        IncreaseDB(char* db_name);
        ~IncreaseDB();

        int init(char* options);
        int free();

        HeadManager* getHeadManager(){
            return _hm;
        }
        DataManager* getDataManager(){
            return _dm;
        }
        Id create_entry(){
            return _hm->create_entry();
        }
        int open_entry(Id* entry,int mode){
            Address* addr = new Address[2];
            int a = _hm->open_entry(entry,addr);
            _dm->open_entry(entry,addr);
            if (a==0) entry_opened = true;
            return a;
        }
        int close_entry(){
            //if (*_hm->get_current_entry())
            int a = _dm->close_entry();
            int b = _hm->close_entry();
            entry_opened = false;
            return a+b;
        }
        int put_head(char* head,int size){
            _hm->put_head(head,size);
        }
        int put_head(char* head){
            return  put_head(head,strlen(head));
        }

        int put_data(char type,char* name,short int nsize,void* data, long int dsize){
            return _dm->put_data(type,name,nsize,data,dsize);
        }
        int put_data(long size,char* bytes){
            return _dm ->put_data(size,bytes);

        }
    };
}


#endif // DB_H_INCLUDED
