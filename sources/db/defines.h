#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

/** GLOBAL VARIABLES */
#define HEAD_SIZE 512
#define USER_HEAD_SIZE 256
#define END -8
//#define DEBUG
#define OUTPUT

/** ERRORS */

/*internal errors*/
#define ERROR_INTERNAL_UNABLE_TO_CREATE_HEAD_MANAGER 20001
#define ERROR_INTERNAL_UNABLE_TO_CREATE_ENTRY 20002
#define ERROR_INTERNAL_UNABLE_TO_CREATE_DATA_MANAGER 20003
/*io system errors*/
#define ERROR_IO_HEAD_FILE_NOT_BINDED 30001
#define ERROR_IO_NO_ACCESS_TO_DATA_FILE 30002
#define ERROR_IO_NO_ACCESS_TO_HEAD_FILE 30003
#define ERROR_IO_DATA_FILE_NOT_BINDED 30004
/*data FILE errors*/
#define ERROR_DATA_ALREADY_OPEN 40001
#define ERROR_DATA_ALREADY_CLOSED 40001
#define ERROR_DATA_INVALID_CURRENT_BLOCK_POINTER 40003
#define ERROR_DATA_INVALID_START_BLOCK_POINTER 40004
#define ERROR_DATA_INVALID_END_BLOCK_POINTER 40005
/*database entry system*/
#define ERROR_ENTRY_NEGATIVE_ID 50001
#define ERROR_ENTRY_NOT_FOUND 50002
/*head file errors*/
#define ERROR_HEAD_INVALID_START_BLOCK_POINTER 60001
#define ERROR_HEAD_INVALID_CURRENT_BLOCK_POINTER 60002
#define ERROR_HEAD_INVALID_END_BLOCK_POINTER 60003

/** HEAD FILE CONSTRUCTION */
/* head - first HEAD_SIZE bytes
 * [last used id(4)]
 */
/* entry segment - ENTRY_HEAD_SIZE bytes
 * [id(4)]
 * [information(244)]
 * [first address(4)]   // points at first address of first data segment
 * [last address(4)]    // points at first address of last data segment
 */
/* head segment errors
 * 1. first address == last address == -1    :  data segment wasn't found, to create
 * 2. last address == 0 & != first address  :  data segment wasn't closed
 */

/** DATA FILE CONSTRUCTION */

/* data segment
 * [id(4)]             - [0]
 * [prev address(4)]   - [4]  // points at first address of previous data segment
 * [open time(4)]      - [8]
 * [close time(4)]     - [12]
 * [size(4)]           - [16]
 * === 20 bytes
 * [content(size)]
 * === size bytes
 * [next address(4)]   - [20+size]
 * === 4 byte
 */
/* data segment errors
 * 1. close time == 0                :  session wasn't closed, check data block
 * 2. open time == close time == 0   :  time unknown
 * 3. next address == 0              :  there is no next block
 */
/* data entry structure
 * [type(1)            - [0]
 * [name size(2)]      - [1]
 * [name(name size)]   - [3]
 * [operation type(1)  - [3+name size]
 * [data size(4)]      - [4+name size]
 * [data(data size)]   - [8+name size]
 * [ \0 (1)]           - [8+name size+data size]
 */
/* data entry types
 * 32 = long integer
 * 33 = long unsigned integer
 * 34 = short integer
 * 35 = short unsigned integer
 * 48 = char[]
 * 49 = unsigned char[]
 * 64 = long long
 * 72 = float
 * 78 = double
 */

 #include <cstdio>
 #include <io.h>
 typedef FILE File;

 struct entry{
     long id;
 };

 typedef long Id;
 #define ID_SIZE 4
 typedef long Address;
 #define ADDR_SIZE 4
 typedef long Time;
 #define TIME_SIZE 4


#endif // DEFINES_H_INCLUDED
