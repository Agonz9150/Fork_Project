#include "project.h"

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

void Initialize_int_array(int *array, const int LENGTH)
{
   int array_counter = START_COUNT;
   for(array_counter; array_counter < LENGTH; array_counter++)
   {
     *(array + array_counter) = EMPTY_VALUE;
   }
}

void Validate_input(char *argv[], const int ARGUMENTS, int *input_array)
{  
   char line[BUFFER] = "";
   *(input_array) = TRUE;
   int array_counter = START_COUNT_ONE;
   sprintf(line,"%s","Parent: Filling shared memory\n");
   write(1,line,strlen(line));
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


void Print_array(int *array, const int ARGUMENTS, const int START,
                 char *full_line)
{
   int array_counter = START;
   char line_segment[BUFFER] = "" ;
   for(array_counter; array_counter < ARGUMENTS; array_counter ++)
   {
      sprintf(line_segment,"%d%s",*(array + array_counter),"|");
      strcat(full_line,line_segment);
   }
   sprintf(line_segment,"%s","\n");
   strcat(full_line,line_segment);
   write(1,full_line,strlen(full_line));
}

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
