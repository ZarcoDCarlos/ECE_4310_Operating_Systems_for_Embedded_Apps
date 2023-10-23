// This program is a round robin scheduler in C
#include <stdlib.h> // for malloc function
//#include <math.h> // to calculate the execution time
#include <stdio.h>



//typedef is alias for a structure
// says replace everything in the brackets for the word at the end
// it is like a class
typedef struct _process {
    int pid;
    char name;
    int length; //how big is the process

                            // star is a pointer to an adress to the next thing in the queue I think?
    struct _process * next; //each node of queue has link to next process in the queue

} process;

// for a queue the new element is ALWAYS added at the end
// we only keep track of the first and the last
typedef struct _queue { 
    int count; // how many elements are in the queue
    process * first; // someone in front of the queue
    process * last; // someone in the back of the queue
} queue;

//this function returns a pointer to the queue structure
queue * create_queue() {
    // malloc usually returns a pointer to void, so we cast it to a pointer
    //sizeof returns the size of the structure 'queue' in bytes
    queue* result = (queue *)malloc(sizeof(queue)); // (malloc = memory allocation) allocate enough bytes to fit a queue structure 
    result->count = 0; //initialize the count to  0 (brand new queue)

    //empty queue so everything is null for the queue
    result->first = NULL;
    result->last = NULL; //null = 0, it is a 0 converted to a pointer to void

    return result; //result is a pointer of structure queue to memory location of  a newly created queue
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

//add to queue
//new elements get added to the end
int add(queue* my_queue, process* new_process) {
    //there is a special case where the count is 0; and a point (don't have a queue at all)
    if (my_queue == NULL) //queue does not exist (point)
        return-1;   //error

    ++my_queue->count; 


    if (my_queue->first == NULL) //no new processes in the queue?
    {
        my_queue->first = new_process;
        my_queue->last = new_process;
    } else { 
        my_queue->last->next = new_process; //to add a new process, assign the current last process to point to the new last process
        my_queue->last = new_process; // new last is assigned to the newly added process
    }
 
    return 0;
}

//removes the first element of a queue(FIFO)
process* remove_process(queue* my_queue) {
    //there is a special case where the count is 0; and a point (don't have a queue at all)
    if (my_queue == NULL) //queue does not exist (point)
        return NULL;   //error (need to return a pointer so return NULL)

    if (my_queue->first == NULL) //queue empty
        return NULL;   //error (need to return a pointer so return NULL)

    --my_queue->count;  //decrement the count

    process* result = my_queue->first; // return = pointer to the removed element

    my_queue->first = result->next;

    

    if (my_queue->count == 0)
        my_queue->last = NULL; // if you are deleting the only element in the queue, get rid of last arrow pointing to element to be deleted
    
    result->next = NULL; //now that it is off the list, make sure it doesn't point to the list
    return result;  
}

//ROUND ROBIN Function:
// how many units of time is each time slice
// returns total execution time
int rr(queue* my_queue, int time_slice) {
    int total = 0; // total execution time
    if(my_queue == NULL)
        return -1; //error there is no queue
        
    while (my_queue->count > 0)
    {
        process* run_process = remove_process(my_queue); // process that is currently running is REMOVED from the queue
        total +=1; // +1 =The time it takes to SWITCH PROCESSES, total = execution time
        
        /*...*/
        printf("\nprocess %c is selected to run (length = %d)", run_process->name, run_process-> length);
        /*...*/

        if (time_slice > run_process->length)
            total +=  run_process->length;
        else
            total += time_slice;
        
        run_process->length -= time_slice; // after the process is run take away units of time process took

        if(run_process->length > 0)
            add(my_queue, run_process); //if the process length is bigger than 1, it is not done, add it back to the queue
    }
    
    return 0; 
}

int main(int argc, char** argv) {

    queue* my_q = create_queue();
    add(my_q, create_process(1, 'a', 30));
    add(my_q, create_process(2, 'b', 70));
    add(my_q, create_process(3, 'c', 10));

    // How many time units do I give per run of execution per each process?
    rr(my_q, 5);
    return 0;
}
