// This program is a lottery scheduler in C for Seminar 2
// Carlos Zarco-ECE 4310
//THIS IS THE ONE THAT ACTUALLY WORKS!

#include <stdlib.h> // for malloc function
#include <stdio.h>
#include <time.h> // used for random number generation (srand to seed the time)

int gtickets = 0; // global number of tickets for all processes

//typedef is alias for a structure
// says replace everything in the brackets for the word at the end
// it is like a class (each process is like a NODE in the list)
// Define the process structure
typedef struct _process {
    int pid;
    char name;
    int length; // how long the process will run for (how many time units)
    int tickets; // number of tickets the process has for the lottery
    struct _process* next; // each node of the list has a link to the next process in the list
} process;

// Define the list structure
typedef struct _list {
    int count; // how many elements are in the list
    process* first; // public member variable first of type process* in the structure (class) List
} list;

// Function to create and initialize a new list
list* create_list() {
    list* result = (list*)malloc(sizeof(list)); // Allocate memory for the list (returns address of empty memory so it can be used)
    result->count = 0; // Initialize the count to 0
    result->first = NULL; // Initialize the first element to NULL
    return result;
}

// Function to create and initialize a new process
process* create_process(int pid, char name, int length) {
    process* result = (process*)malloc(sizeof(process)); // Allocate memory for the process
    result->length = length; // Set the process length
    result->name = name;
    result->pid = pid;
    result->next = NULL; // Initialize the next pointer to NULL
    return result;
}

// Function to add a new process to the end of the list
int add(list* my_list, process* new_process, int tickets) {
    ++my_list->count; // Increment the count of processes
    new_process->tickets = tickets; // Assign the number of tickets to the new process

    if (my_list->first == NULL) { //list is empty
        my_list->first = new_process; // set the new process as the first one
    } else {
        process* current = my_list->first; //starting at the first process
        while (current->next != NULL) {
            current = current->next; // Find the last process in the list
        }
        current->next = new_process; // Add the new process to the end of the list
    }

    return 0; // Success
}

// Function to print the list
void print_list(list* my_list) {
    if (my_list == NULL) { //list does not exist
        printf("List is NULL.\n");
        return;
    }

    if (my_list->count == 0) { //list is empty
        printf("\nList is empty.\n");
        return;
    }

    process* current = my_list->first; // starting at the first process
    printf("List Contents:\n");
    while (current != NULL) {
        printf("Process PID: %d, Name: %c, Length: %d, Tickets: %d\n",
            current->pid, current->name, current->length, current->tickets);
        current = current->next; //move on to the next process
    }
}

//remove current process
void remove_process(list* my_list, process* current_process) {
    if (my_list == NULL || my_list->count == 0 || current_process == NULL) { // if list DNE, process count =0, and current process is null = error
        printf("\n Error, list is empty");
        return;  // Error, either the list is empty or the current process is not provided
    }

    // If the current process is the first one in the list
    if (my_list->first == current_process) {
        my_list->first = current_process->next;
        printf("\nProcess %d  has been removed", current_process->pid);
        free(current_process);
        my_list->count--;
        
        return;  // Success
    }

    // If the current process is not the first one, find its predecessor
    process* prev_process = my_list->first;
    while (prev_process != NULL && prev_process->next != current_process) {
        prev_process = prev_process->next;
    }

    // If the current process was found in the list
    if (prev_process != NULL) {
        prev_process->next = current_process->next;
        free(current_process);
        my_list->count--;
        //printf("/n %d process has been removed", current_process->pid);
        return;  // Success
    }
    printf("\n Error, current process not found on the list");
    return;  // Error, the current process was not found in the list
}

// Lottery Scheduler function
int lottery(list* my_list, int time_slice) {

    if (my_list == NULL) { //list is empty
        printf("No processes in the list to schedule.\n");
        return -1; // Error, no processes in the list
    }

    // Calculate the total number of tickets
    process* current = my_list->first;
    while (current != NULL) { // there are processes
        gtickets += current->tickets;
        current = current->next;
    }

    srand(time(NULL)); // Seed the random number generator

    // Generate a random ticket between 1 and total tickets
    int winning_ticket = (rand() % gtickets) + 1;

    // Find the winning process based on the winning ticket
    current = my_list->first;
    process* prev_process = NULL;

    while (current != NULL) {
        if (winning_ticket <= current->tickets) { // Winning process
            printf("\nProcess %c won and is selected to run (length = %d)\n", current->name, current->length);
            print_list(my_list);
            current->length -= time_slice;
           
            // Decrease the global ticket count by 1
            current->tickets -= 1;
            gtickets -= 1;

            if (time_slice > current->length) {
                gtickets -= current->tickets;
                current->length = 0;
                remove_process(my_list, current); // process that is currently running is REMOVED from the queue
            }

            if (current->length == 0) {
                if (prev_process == NULL) { // process to be removed is the first one
                    my_list->first = current->next; // update the list's first pointer with the current one that is done's next
                } else {
                    prev_process->next = current->next;
                }
                free(current);
                my_list->count--;
            }
            return 0; // success (should it just be return 0; ???) CHECK THIS LATER
        } else {
            winning_ticket -= current->tickets;
            prev_process = current;
            current = current->next;
        }
    }

    return 0;
}


int main(int argc, char** argv) {
    list* my_list = create_list();
    add(my_list, create_process(1, 'a', 10), 100);
    add(my_list, create_process(2, 'b', 7), 50);
    add(my_list, create_process(3, 'c', 3), 200);



    while (my_list->count > 0) {
        
        int result = lottery(my_list, 5);
        if (result == -1) {
            printf("No processes in the list to schedule.\n");
            break;
        }

    }

    print_list(my_list);

    free(my_list);
    return 0;
}
