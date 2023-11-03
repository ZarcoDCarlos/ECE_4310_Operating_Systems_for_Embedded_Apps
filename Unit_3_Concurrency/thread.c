#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int glob = 0; //global variable shared by each thread
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; //initially unlocked because no processes are doing anything yet 

//void* = indicates that thread_function returns a pointer to data of unspecified type
// (void *args)= argument of the function, the argument can be pointer to data of any type
static void* thread_function(void *args)
{
    int loops = *((int *) args); //(int *)args = args is now an integer pointer
                                 // *((int *)args) = dereference the pointer, access the value that the integer pointer args points t
    int ix, ret;
    for(ix = 0; ix < loops; ++ix)
    {
        ret = pthread_mutex_lock(&mtx); // Locking the door (only current process can access the global variable)
        if (ret != 0) return NULL; //if there is some sort of error do not access glob, finish the function here
        ++glob; //preincrement because it is faster

        ret = pthread_mutex_unlock(&mtx); //unlocking the door
        if (ret != 0) return NULL;
    }

    return NULL;
}

int main(int argc, char*argv[]) //char*argv[] is an array of strings
{
    pthread_t t1, t2; //unsigned long (integer that takes 8-bytes)
    int loops, ret; //local variables: ret = return value
    loops = (argc > 1) ? atoi(argv[1]) : 1000000; 
    //Command line argument:
                                                  // if there is an argument in the terminal (arc >1), 
                                                  // convert first argument we receive to an integer (atoi)
                                                  // else, no command line argument is provided, use default value of 1000000

    ret = pthread_create(&t1, NULL, thread_function, &loops); // arguments: (address of a variable first, second argument is null, 
                                                              // function we want for the thread, argument we want to provide thread_function)
    ret = pthread_create(&t2, NULL, thread_function, &loops);

    pthread_join (t1, NULL); // wait for t1 to finish
    pthread_join(t2, NULL); //wait for t2 to finish

    printf("total = %d\n", glob);

    return 0;

}

