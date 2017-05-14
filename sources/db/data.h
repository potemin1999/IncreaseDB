#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include <io.h>

#include "head.h"
#include "defines.h"
#include "utils.h"

#ifdef DEBUG
#define DATA_DEBUG
#endif // DEBUG

namespace DB{
    class DataManager{
    private:
        HeadManager* _hm;
        long _DataS;
        long _DataE;
        long _DataP;
        bool _DataOpened;
        File* _D;
        db_info info;
        long int _DataSegmentStart;
        long int _DataSegmentTimeS;
        long int _DataSegmentTimeE;
        long int _DataSegmentSize;
    public:
        DataManager(db_info info);
        ~DataManager();
        int init(File* data);
        int bind_head_manager(HeadManager* hm){_hm = hm; return 0;}
        int open_entry(Id* entry,Address* a_se);
        int close_entry(){
            if (_DataOpened){
                close_data_segment();
            }
            return 0;
        }
        /** returns start address(on head) of segment */
        Address create_data_segment();
        /** returns first empty byte address of segment */
        Address close_data_segment();
        int put_data(char type,char* name,long int nsize,void* data, long int dsize);
        int put_data(long size,char* bytes);
    };
}

#endif // DATA_H_INCLUDED
