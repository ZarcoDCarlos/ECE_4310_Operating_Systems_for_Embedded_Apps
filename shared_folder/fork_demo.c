#include <stdio.h> 
#include <sys/types.h> //libraries for fork (getpid) also wait
#include <unistd.h> //fork as
#include <unistd.h> // for exec: execute a file
#include <sys/wait.h> // for wait: function waits for a specific process and reports back the status when it is completed


//#include <errno.h> // if u want to knwo why a fork failed

int main (int argc, char **argv) {
    //argc = counts how many elements there is an a vector (it can never be zero), argv is a vector
    // by default if you don't assign argc it is 1

    printf("\n%s\n", argv[0]);

    int pid_from_fork = -1;
    int my_pid = 0;
    int err = 0; //variable for error
    //pid_t is a typedef, exactly the same as typing int to the compiler
    int wait_pid = 0;
    pid_from_fork = fork(); //fork happens

    //according to manpages results tab, 3 results are possible -1 for failure, 0, for success and random number
    switch(pid_from_fork) {
        case(0):    // 0 is returned in the child, fork succeeded
            my_pid = getpid();
            printf("\nHi from the child: my_pid = %d\n", my_pid);
            err = execl("/usr/bin/ls", "/usr/bin/ls", "/", "-all", (char *) NULL); //type which ls in terminal to see path
                                                                                    // "/" = the rooot folder
            break;
        case(-1):   // -1 returned in the parent (inside the parent), no child created and the call for fork failed
            my_pid = getpid();
            printf("\nHi from the parent (FORK failed): my_pid = %d\n", my_pid);
            break;
        default:    // when the process ID for the child is not 0, the PID for the child can only be seen in the parent 
            my_pid = getpid();
            //goal: get parent to print after the child is done
            int state = 0;
            wait_pid = wait(&state); //sends the address of the local variable
            printf("\nHi from the parent(my_pid = %d, child pid = %d)\n", my_pid, pid_from_fork);
            printf("\nChild process finished: wait_pid = %d, state = %d\n", wait_pid, state);
            break; 
    }
               
   return 0; 
}
