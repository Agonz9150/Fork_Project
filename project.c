#include "project.h"


int main(int argc, char *argv[])
{
   const int ARGUMENTS = argc;
   char line[BUFFER]="";
   int valid_arguments = Validation(ARGUMENTS);
   if(valid_arguments == FALSE)
   {
      return NO_ERROR;
   }
   Shared_array shared_mem;
   Shared_array *shared_ptr;
   shared_ptr = &shared_mem;
   Create_shared_memory(shared_ptr, ARGUMENTS);
   if((*shared_ptr).state == FAIL)
   {
      return NO_ERROR;
   }
   Initialize_int_array((*shared_ptr).memory_ptr ,ARGUMENTS);
   Validate_input(argv, ARGUMENTS, (*shared_ptr).memory_ptr);
   if(*((*shared_ptr).memory_ptr) == FALSE)
   {
      return NO_ERROR;
   }
   Fork_process(ARGUMENTS, (*shared_ptr).memory_ptr);
   Delete_shared_memory(shared_ptr);
   sprintf(line,"%s","Parent: Finshed\n");
   write(1,line,strlen(line));
   return NO_ERROR;
}
