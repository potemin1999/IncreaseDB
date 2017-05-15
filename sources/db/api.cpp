#include "db/api.h"



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
    return db->open_entry((Id*)&entry_p->id,0);
}
