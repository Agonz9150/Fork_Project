/*=============================================================
| Source code:SharedProcessMain.c
|      Author:Alexis Gonzalez
|   Assigment:Specifications
|
|      Course:Programing 3
|     Section:U03
|  Instructor:William Feild
|    Due Date:29 November, 2018
|
|	I hereby certify that this collective work is my own
|	and none of it is the work of any other person or entity
|
|	___________________________________________________
|
|    Language:C
| Compile/Run: make
|             ./SharedProcess [1-7 arguments, each int 0-9, no repeat]
|
|               gcc SharedProcessMain.c SharedProcessFunction.c 
|			   -o SharedProcess
|			   ./SharedProcess [1-7 arguments, each int 0-9, no repeat]
|+--------------------------------------------------------------
| Description: This program takes in 1 to 7 inputs from the command line 
|              , request shared memory, and stores the input in the shared
|              memory. The program then forks, creating a child for each 
|              input, and modifying the shared memory. After the child 
|              processes finsh, the shared memory is outputted and the 
|              memory is removed.			   
| 
|       Input: 1 to 7 input values on the command line, each an int 0 to 9
|              , all unique
|
|      Output: The program will constantly print to the console the state of  
|              each process and the state of the shared memory from start to 
|              finish
|
|     Process: The program starts by validating the input size. After validation,
|              the program creates the shared memory, and fills the shared memory.
|    		   Once complete, the program then starts creating children, one for 
|	    	   each input, and waits for their completion. The children then 
|		       operate on their input of the shared mamory, (value at child index =
|		       value at child index * index) while printing the state of the memory.
|		       once complete, the parent then prints the resulting memory, and  
|		       detaches the memory.
|
| Required Features Not Included:N/A
|
|  Known Bugs: N/A
|*===============================================================*/


#include "SharedProcess.h"
/*============================================================================
Main-

The Main does the following: 
Send argc and argv to get validated
Create shared memory
fill shared memory 
starts fork process
Remove shared memory

============================================================================*/

int main(int argc, char *argv[])
{
   const int ARGUMENTS = argc;
   char line[BUFFER]="";
   int input[ARGUMENTS];
   Initialize_int_array(input ,ARGUMENTS);
   int valid_arguments = Validation(ARGUMENTS);
   if(valid_arguments == FALSE)
   {
      return NO_ERROR;
   }
   Validate_input(argv, ARGUMENTS, input);
   if(input[VALIDATION_INDEX] == FALSE)
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
   Fill_shared_memory(input, ARGUMENTS, (*shared_ptr).memory_ptr);
   Fork_process(ARGUMENTS, (*shared_ptr).memory_ptr);
   Delete_shared_memory(shared_ptr);
   sprintf(line,"%s","Parent: Finshed\n");
   write(1,line,strlen(line));
   return NO_ERROR;
}
