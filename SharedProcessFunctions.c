/*============================================================================
 |Alexis Gonzalez
 |SharedProcessFunctions.c

 ============================================================================*/

#include "SharedProcess.h"
/*============================================================================
|Int Validation
|
|Input -> ARGUMENTS (total user input)
|
|The function checks if ARGUMENTS is valid, by making sure it's between 1 and 7
|
|The function returns True if valid or False if invalid
============================================================================*/
int Validation(const int ARGUMENTS)

{  
   char line[BUFFER] = "";
   sprintf(line,"%s","Parent: validate command line \n");
   write(1,line,strlen(line));
   const int USER_INPUT = ARGUMENTS - EXECUTABLE_ARGUMENT;
   int input_counter = START_COUNT;
   if(USER_INPUT > MAX_ARGUMENTS || USER_INPUT < MIN_ARGUMENTS)
   {
      sprintf(line,"%s%d%s%d%s",
      "Error: Incorrect number of inputs, the program requires ",
      MIN_ARGUMENTS," to ",MAX_ARGUMENTS,"\n");
      write(1,line,strlen(line));
      return FALSE;
   }
   return TRUE;
} 
/*============================================================================
|Initialize_int_array
|Input -> Array ptr, LENGTH (length of array)
|
|This function modifies the given array, replacing every value within with 0
|using a loop 
|    For each index in array:
|	    array_index = 0
|
============================================================================*/
void Initialize_int_array(int *array, const int LENGTH)
{
   int array_counter = START_COUNT;
   for(array_counter; array_counter < LENGTH; array_counter++)
   {
     *(array + array_counter) = EMPTY_VALUE;
   }
}
/*============================================================================
|Validate_input
|
|Input -> argv ( user input string) ARGUMENTS (number of user input) input_array
|         (array to store values)
|		 
|This function does the following: 
|         This function checks if each input is valid by using a combination
|         of checks (using loops)
|
|		 
|	for every value the user inputs:
|	    check if input is valid ( using function) if no, mark input as bad 
|		       (index 0 in array)
|		Check if input has been used prior ( using a loop). If it has, mark 	   
|             input as bad
|		input value into array	  
|
============================================================================*/
void Validate_input(char *argv[], const int ARGUMENTS, int *input_array)
{  
   char line[BUFFER] = "";
   *(input_array) = TRUE;
   int array_counter = START_COUNT_ONE;
   for(array_counter; array_counter < ARGUMENTS; array_counter++)
   {
      int input_value = EMPTY_VALUE;
      input_value = String_to_int(argv[array_counter]);
      if(input_value == FALSE)
      {
         *(input_array) = FALSE;
         sprintf(line,"%s%s%s","Error: user input: ",argv[array_counter],
               " is invalid \nInput a int value between 0 and 9\n");
         write(1,line,strlen(line));
      }
      else
      {
         int previous_value = EMPTY_VALUE;
         previous_value = Search_array(input_array, input_value, array_counter);
         if(previous_value == TRUE)
         {
            sprintf(line,"%s%d%s","Error: ",input_value,
            " is used multiple times \n");
            write(1,line,strlen(line));
            *(input_array) = FALSE;
         }
      }
      *(input_array + array_counter) = input_value;
   }
}
/*============================================================================
|Fill_shared_memory
|
|Input -> input (array where validated values are being stored)
|         Arguments ( number of user inputs)
|         shared memory (shared memory array)
|
|      This function inserts user inputed values into shared memory
============================================================================*/
void Fill_shared_memory(int *input, const int ARGUMENTS, int * shared_memory)
{
   char line[BUFFER] = "";
   int array_counter = START_COUNT_ONE;
   sprintf(line,"%s","Parent: Filling shared memory\n");
   write(1,line,strlen(line));
   for(array_counter; array_counter < ARGUMENTS; array_counter++)
   {
	   *(shared_memory + array_counter) = *(input + array_counter);   
   }
      
}
/*============================================================================
|String to int 
|
|Input -> string* (inputed string to convert)
|
|This function takes each char in the string, converts them into their ascii
|value, and subtracts ascii_0 value from it. If the value is between 0 and
|9, it saves it into current value. The loop also checks for overflow. If
|the string contains a non-int value, overflows, or is not between 0 and 9 
|False is returned. Otherise current value is returned.
============================================================================*/
int String_to_int(char *string)
{
   int array_counter = START_COUNT;
   char current_char = *(string);
   int current_value = EMPTY_VALUE;
   int char_value = EMPTY_VALUE;
   int test_value = EMPTY_VALUE;
   while(current_char != '\0')
   {
      if(current_char > ASCII_9 || current_char < ASCII_0)
      {
         current_value = FALSE;
      }
      else
      {
         if(current_value != FALSE)
         {
            test_value = current_value * LEFT_SHIFT;
            char_value = current_char - ASCII_0;
            test_value = test_value + char_value;
            if(test_value < current_value)
            {
               current_value = FALSE;
            }
            else
            {
               current_value = test_value;
            }
         }
      }
   array_counter++;
   current_char = *(string + array_counter);
   }
   if(current_value > MAX_INTERGER)
   {
      current_value = FALSE;
   }
   return current_value;
}
/*============================================================================
|Search_array 
|
|Input -> *array (array to be searched), VALUE (value to search for), LENGTH
|         (array length)
|
|This function does the following loop
|    For item in array:
|      if item = VALUE return true
|    Return False
============================================================================*/
int Search_array(int *array, const int VALUE, const int LENGTH)
{
   int array_counter = START_COUNT_ONE;
   for(array_counter; array_counter < LENGTH; array_counter++)
   {
      if(*(array + array_counter) == VALUE)
      {
         return TRUE;
      }
   }
   return FALSE;
}
/*============================================================================
|Fork Process 
|
|input - ARGUMENTS (number of user inputs ), int *shared_array (shared memory)
|
|This function does the following:
|
|      Display the Shared memory 
|	  Alert the user forking is about to start
|	  Operate the forking loop, creating 1 for each input 
|	     Fork process
|		 determine if the process is parent or child (using pid_t value
|		 that fork returns):
|		     If parent - continue loop, then enter parent process function.
|			 If child - Run child process function
============================================================================*/
void Fork_process(const int ARGUMENTS, int *shared_array)
{
   int child_counter = START_COUNT_ONE;
   char line[BUFFER] = "";
   pid_t determine_child;
   determine_child = getpid();
   sprintf(line,"%s","Parent: Displaying array : ");
   Print_array(shared_array,ARGUMENTS,START_COUNT_ONE,line);
   while(child_counter < ARGUMENTS)
   {
      sprintf(line,"%s%d%s","Parent: Forking child ",child_counter,"\n");
      write(1,line,strlen(line));
      determine_child = fork();
      if(determine_child == CHILD_PID_RESULT)
      {
         Child_process(child_counter,shared_array,ARGUMENTS);
      }
      child_counter++;
   }
   Parent_process(ARGUMENTS,shared_array);
}


/*============================================================================
|Create_shared_memory
|Input -> number_array (pointer that shared memory will attach to)
|         ARGUMENTS ( number of user inputs)
|        
|This function creates shared memory by doing the following:
|  The function will attempt to request shared memory using the following
|     equation ARGUMENTS * sizeof(int) for the size.(. If unsuccessful 
|     it will alert the user and return out.
|  Next the program will attept to attach the memoryto the pointer. If 
|     unsucessful, the function will return out while alerting the user.
============================================================================*/
void Create_shared_memory(Shared_array *number_array, const int ARGUMENTS)
{
   (*number_array).memory_id = shmget(IPC_PRIVATE,
   ARGUMENTS * sizeof(int), IPC_CREAT | 0666);
   char line[BUFFER] = "";
   sprintf(line,"%s","Parent: Requesting shared memory \n");
   write(1,line,strlen(line));
   if ((*number_array).memory_id < 0)
   {
      sprintf(line,"%s","ERROR, Shared memory request failed\n");
      write(1,line,strlen(line));
      (*number_array).state = FAIL;
      return;
   }
   sprintf(line,"%s%s","Parent: Recives shared memory\n",
           "Parent: Attaches shared memory\n");
   write(1,line,strlen(line));
   (*number_array).memory_ptr = (int *) shmat((*number_array).memory_id,
                                              NULL, 0);
   if ((long) (*number_array).memory_ptr == FAIL)
   {
      printf("%s","ERROR, Shared memory attachment failed \n");
      (*number_array).state = FAIL;
   }
}
/*============================================================================
|Delete shared memory
| Input ->  number_array (attached memory)
|
|The function will detach and remove the shared memory while alerting the user.
|
============================================================================*/
void Delete_shared_memory(Shared_array *number_array)
{   
   char line[BUFFER]="";
   sprintf(line,"%s","Parent: Detaching shared memory\n");
   write(1,line,strlen(line));
   shmdt((*number_array).memory_ptr);
   sprintf(line,"%s","Parent: Removing shared memory\n");
   write(1,line,strlen(line));
   shmctl((*number_array).memory_id, IPC_RMID, NULL);
}
/*============================================================================
|Child Process
| Input -> INDEX (child identifier/ index for it to access) 
|          shared_array (shared memory)
|          * NOTE: Index 0 is not used. 1|5|6 has 1 at index 1 etc.

|          ARGUMENTS (Number of user inputs)
| The function handles child process :
|      The function starts by the process notifying the user it's assigned
|      identity. Next the process will display the current state of the 
|      shared memory. Then the process will identify it's PID code.
|     
|      The process then updates the shared memory with the following:
|            The assigned index of the child's value will get multiplied by
|            that index
|  			 ex) 1|5|6 , child 2 -> 1|10|6
|      The new state of the array is then printed to the screen, and the 
|	   child exits and ends process.
============================================================================*/
void Child_process(const int INDEX, int *shared_array, const int ARGUMENTS)
{
   char line[BUFFER]="";
   int array_counter = START_COUNT_ONE;
   pid_t current_pid;
   
   sprintf(line,"%s%d%s","      Child ",INDEX,": Starts \n");
   write(1,line,strlen(line));
   sprintf(line,"%s%d%s","      Child ",INDEX,
          ": Displaying shared memory: ");
   Print_array(shared_array,ARGUMENTS,START_COUNT_ONE,line);
   current_pid = getpid();
   sprintf(line,"%s%d%s%d%s","      Child ",INDEX,": Private ID: ",
           current_pid,"\n");
   write(1,line,strlen(line));
   sprintf(line,"%s%d%s","      Child ",INDEX,": Updating shared memory\n");
   write(1,line,strlen(line));
   *(shared_array + INDEX) = *(shared_array + INDEX) * INDEX;
   sprintf(line,"%s%d%s","      Child ",INDEX,": Displaying updated memory: ");
   Print_array(shared_array,ARGUMENTS,START_COUNT_ONE,line);
   sprintf(line,"%s%d%s%d%s","      Child ",INDEX,": Ending with exit code ",
           NO_ERROR,"\n");
   write(1,line,strlen(line));
   exit(NO_ERROR);

}

/*============================================================================
|Print Array
|
|Input -> *array (array to be printed), INDEXES(Total indexes in array)
|         START (which index to start from ), full_line ( what string 
|         to concatnate the array to)
|
|This function concatanates an array to a previous message, and prints 
|  the result
|    Ex) [0,2,3,4] start = 1  message = " Array is :" 
|    prints "Array is : 2|3|4"
============================================================================*/
void Print_array(int *array, const int INDEXES, const int START,
                 char *full_line)
{
   int array_counter = START;
   char line_segment[BUFFER] = "" ;
   for(array_counter; array_counter < INDEXES; array_counter ++)
   {
      sprintf(line_segment,"%d%s",*(array + array_counter),"|");
      strcat(full_line,line_segment);
   }
   sprintf(line_segment,"%s","\n");
   strcat(full_line,line_segment);
   write(1,full_line,strlen(full_line));
}
/*============================================================================
|Parent_process
|
|Input -> ARGUMENTS ( number of children/ number of user inputs)
|         shared_array (shared array)
|
|  This function controlls the parent process:
|     The parent will notify the user that it's waiting for all the children 
|     to complete and notifies every time one does (and stores it's end state). 
|     After all child processes end, the parent prints out all of the children
|     exit status and prints the resulting shared memory.
============================================================================*/
void Parent_process(const int ARGUMENTS, int *shared_array)
{
   pid_t returned_pid;
   char line[BUFFER] ="";
   int status = EMPTY_VALUE;
   int status_array[ARGUMENTS];
   int pid_array[ARGUMENTS];
   int child_counter = START_COUNT_ONE;
   Initialize_int_array(status_array, ARGUMENTS);
   Initialize_int_array(pid_array, ARGUMENTS);
   sprintf(line,"%s","Parent : waiting for child completion \n");
   write(1,line,strlen(line));
   
   for(child_counter; child_counter < ARGUMENTS; child_counter++)
   {
      returned_pid = wait(&status);
      sprintf(line,"%s","Parent: detected child completion \n");
      write(1,line,strlen(line));
      status_array[child_counter] = status;
      pid_array[child_counter] = (long) returned_pid;
   }
   child_counter = START_COUNT_ONE;
   for(child_counter; child_counter < ARGUMENTS; child_counter++)
   {
      sprintf(line,"%s%d%s%d%s","Parent: child ",pid_array[child_counter],
              " had exit code ",status_array[child_counter],"\n");
      write(1,line,strlen(line));
  
   }
   sprintf(line,"%s","Parent: Displaying resulting memory: ");
   Print_array(shared_array,ARGUMENTS,START_COUNT_ONE,line);
}
