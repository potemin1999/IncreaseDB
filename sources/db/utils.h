#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <ctime>
#include <iostream>
#include "defines.h"



struct db_info{
    long creation_time;
    bool is_first_creation;
};

namespace UTILS{

    /** get file size */
    long get_size(File* f);

    /**returns user head address*/
    long get_head_by_id(File* _H,Id* _id);

    long time(db_info* info);

    /** writes (count) empty bytes in (f) file */
    int write_empty_bytes(int count,File* f);
}


#endif // UTILS_H_INCLUDED
