#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#define NO_ERROR 0
#define EMPTY_VALUE 0
#define NULL_CHAR 0
#define VALIDATION_INDEX 0
#define BAD_STRING -1
#define START_COUNT 0
#define START_COUNT_ONE 1
#define MAX_ARGUMENTS 7 
#define MIN_ARGUMENTS 1
#define MAX_INTERGER 9
#define MIN_INTERGER 0
#define ASCII_0 48
#define ASCII_9 57
#define LEFT_SHIFT 10
#define EXECUTABLE_ARGUMENT 1
#define FAIL -1
#define ACTIVE 0
#define CHILD_PID_RESULT 0
#define BUFFER 200


enum Boolean {FALSE = -1, TRUE = 1};

struct Shared_array
{
   key_t shared_key;
   int memory_id;
   int *memory_ptr;
   int state;
};

typedef struct Shared_array Shared_array;


int Validation(const int);
void Initialize_int_array(int *,const int);
void Validate_input(char **,const int,int *);
int String_to_int(char *);
int Search_array(int *, const int, const int);
void Fork_process(const int, int *);
void Create_shared_memory(Shared_array *, const int);
void Delete_shared_memory(Shared_array *);
void Child_process(const int, int *, const int);
void Parent_process(const int, int *);
void Print_array(int *, const int, const int,char *);
