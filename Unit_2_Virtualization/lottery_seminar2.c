// This program is a lottery scheduler in C for Seminar 2
// Carlos Zarco-ECE 4310

#include <stdlib.h> // for malloc function
//#include <math.h> // to calculate the execution time
#include <stdio.h>
#include <time.h> //used for random number generation(srand to seed the time)


int gtickets = 0; //global number of tickets for all processes

//typedef is alias for a structure
// says replace everything in the brackets for the word at the end
// it is like a class (each process is like a NODE in the list)
typedef struct _process {
    int pid;
    char name;
    int length; // how long the process will run for (how many time units)
    int tickets; // number of tickets process has for the lottery
    struct _process * next; //each node of list has link to next process in the list
} process;

// Using a list: 
//      - a list is ordered (determined by their position/index)
//      - have accesss to all elements unlike the queue where you
//        can only access the first and the last one.
typedef struct _list {
    int count; // how many elements are in the list
    process* first; //public member variable first of type process* in the structure(class) List
} list;

//this function returns a  pointer to the  address of the newly created list structure
list * create_list() {
    // malloc usually returns a pointer to void, so we cast it to a pointer
    //sizeof returns the size of the structure 'list' in bytes
    list* result = (list *)malloc(sizeof(list)); //malloc = memory allocation (enough bytes for list struct)
    result ->count = 0; // count of list = 0 for a brand new list

    //empty list so null
    result->first = NULL;

    return result; // return the address of the newly created list
}


//names in this function don't have to be the same as earlier
// this function returns a pointer to the process structure
process* create_process(int pid, char name, int length) {
    //1. allocate the memory = malloc(sizeof(structure));
    process* result = (process *)malloc(sizeof(process)); //first thing we need to do is allocate the proper amount of memory

    //2. initialize
    result-> length = length; // set new process length to the given length of the function (rest of lines are similar)
    result->name = name;
    result->pid = pid;
    result->next = NULL;

    return result; // result is the pointer of process structure to the memory location with the newly created process structure
}


//add elements to the list
// (new elements can be added wherever in the list)
// to insert we have three paramaters:
//      1. the pointer to the list we are dealing with 
//      2. the new process to add
//      3. the previous process (new one goes after)
//      4. the number of tickets in that process
// the new process will go after the given Previous process
int add (list* my_list, process* new_process, process* pred_process, int tickets) {
    if (my_list == NULL) // list does not exist 
        return -1; //error
    ++my_list->count; // add 1 to count of processes in the list
    new_process->tickets = tickets; // assign the new process with the given number of tickets from the user

    if (pred_process != NULL) { //list is not empty
        new_process->next = pred_process->next;
        pred_process->next = new_process;
    }
    else { //list is empty
        new_process->next = my_list->first; // since the list is empty, first is pointing to nullptr, 
							                // therefore the next process that new_process points too is also nullptr since you are only inserting one process
        my_list->first = new_process;   // the first element is now the new process being added
    }

    return 0; //success
}

// remove a process from the list
// to remove a process from the list its length is 0.
//NOTE: this function takes the predecessor of the item to be deleted, NOT the item itself
// to remove a process we need to know the following previous process
// process* remove_process(list* my_list, process* pred_process) {
//     //there is a special case where the count is 0; and a point (don't have a list at all)
//     if (my_list == NULL) //list does not exist (point)
//         return NULL;   //error (need to return a pointer so return NULL)

//     if (my_list->first == NULL) //list empty
//         return NULL;   //error (need to return a pointer so return NULL)

//     --my_list->count;  //decrement the count 

//     //process* removed_process = pred_process->next; // Process to be removed (based om the previous)

//     if (pred_process == NULL) { //erasing the first process
//         process* removed_process = my_list->first;
//         my_list->first = my_list->first->next;
//         //free(removed_process);
//     } else if (pred_process->next != NULL) { // removing other processes
//         // Update the predecessor process's 'next' pointer to skip the process to be removed
//         process* removed_process = pred_process->next;
//         pred_process->next = removed_process->next;
//         //free(removed_process);
//     }
// }


//function to print the list
void print_list(list* my_list) {
    if (my_list == NULL) { //null
        printf("List is NULL.\n");
        return;
    }

    if (my_list->count == 0) { //empty based on count
        printf("List is empty.\n");
        return;
    }

    process* current = my_list->first; //current process is the first one

    printf("List Contents:\n");
    while (current != NULL) { //goes from process to process in list until it reaches the last one
        printf("Process PID: %d, Name: %c, Length: %d, Tickets: %d\n",
        current->pid, current->name, current->length, current->tickets);
        
        current = current->next;
    }
}

//Lottery Scheduler function
int lottery (list* my_list, int time_slice) {
    int total = 0; // total execution time
    // I did not DEAL WITH TOTAL EXECUTION TIME
    if (my_list == NULL)
    {
        printf("No processes in the list to schedule.\n");
        return -1; //error there is no queue
    }        

    // calculate the total number of tickets
    process* current = my_list->first; //current process is the first one
    while (current != NULL){ // while the list is not over
        gtickets += current->tickets; // add the current processes tickets to the global ticket count
        current = current->next; // go to next process
    }

    // selecting a random number
    //srand(time(NULL)); // seed the random number generator
    
    // Generate a random ticket between 1 and total tickets
    // rand()% gtickets = random number from 0-total tickets -1
    // rand()% gtickets = random number from 1-total tickets!
    int winning_ticket = (rand() % gtickets) + 1;
    printf("\nRandom Number: %d", winning_ticket);
    
    // Find the winning process based on the winning ticket
    current = my_list->first; //track current process we are testing
    process* prev_process = NULL; // Track the previous process

    while (current != NULL) {
        if (winning_ticket <= current->tickets) { //winning process
        
        total +=1; // +1 =The time it takes to SWITCH PROCESSES, total = execution time
        printf("\nprocess %c won and is seleceted to run (length = %d)", current->name, current-> length);
        current->length -= time_slice; // general case! if the process executes, update the length

        if (time_slice > current->length) {// if the time slice is higher than the remaining length of the process
            total +=  current->length; //total execution time will be added with remaining length
            current->length = 0; // the process can finish within the time slice allocated so the length can be set to  0
        }
        else
            total += time_slice; // add the time_slice to the total execution time

        //remove process w previous
        if(current->length == 0){
            if (prev_process == NULL) { //process to be removed is the first one
                // update the list's first pointer with the current one that is done's next
                my_list->first = current->next;
            }
            else {
                prev_process->next = current->next;
            }
            //free(current);
            my_list->count--;
            //current = NULL; // used only to exit the loop
        }        
        
        break;
        
        }

        //lost the lottery
        winning_ticket -= current->tickets;
        prev_process = current; // updates the prev process for next iteration where current process points to next one
        current = current->next; // point current to the next item on the list
    }
    return total;
}


int main(int argc, char** argv) {
    list* my_list = create_list();
    process* p1 = create_process(1, 'a', 10);
    process* p2 =create_process(2, 'b', 7);
    process* p3 = create_process(3, 'c', 3);
    add(my_list, p1, NULL, 100);
    add(my_list, p2, p1, 50);
    add(my_list, p3 , p2, 200);
    
    // selecting a random number
    srand(time(NULL)); // seed the random number generator
    int total_execution_time = 0;

    // Call lottery scheduler function until the list is empty
    while (my_list->count > 0) {
        
        int result = lottery(my_list, 5);
        if (result == -1) {
            printf("No processes in the list to schedule.\n");
            break; // Exit the loop if there are no processes
        }
        total_execution_time += result;
    }

    print_list(my_list); // Print the final state of the list
    //printf("Total Execution Time: %d\n", total_execution_time);

    return 0;
}
