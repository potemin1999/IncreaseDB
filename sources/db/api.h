#ifndef API_H_INCLUDED
#define API_H_INCLUDED

#include "db.h"
#include "head.h"
#include "data.h"
#include "defines.h"

#define NEW_NAMES 1

using namespace DB;

namespace{
    DB::IncreaseDB* db;
    DB::HeadManager* hm;
    DB::DataManager* dm;
}

int dbInitDB(char* name,char* options);
int dbDestoyDB();
int dbCreateEntry(entry* new_entry_p);
int dbOpenEntry(entry* entry_p);
int dbCloseEntry();


int dbInitDB(char* name,char* options){
    db = new DB::IncreaseDB(name);
    db->init(options);
    hm = db->getHeadManager();
    dm = db->getDataManager();
    return 0;
}

int dbDestoyDB(){
    return db->free();
}

int dbCreateEntry(entry* new_entry_p){
    new_entry_p->id = db->create_entry();
    return 0;
}

int dbOpenEntry(entry* entry_p){
    return 0;// db->open_entry(entry_p);
}





#ifdef NEW_NAMES





/**db init*/
typedef int (db_init)(char* name,char* options);
#define db_init dbInitDB

/**db destroy*/
typedef int (db_destroy)();
#define db_destroy dbDestroyDB

/**db create user*/
typedef int (db_create_entry)(entry* new_entry);
#define db_create_entry dbCreateEntry

/**db open user*/
typedef int (db_open_entry)(entry* entry);
#define db_open_entry dbOpenEntry

/**db close user*/
typedef int (db_close_entry)();
#define db_close_entry dbCloseEntry







#endif // NEW_NAMES


#endif // API_H_INCLUDED
