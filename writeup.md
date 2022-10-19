# Design Choices for Programming Assignment 1
## What (additional) assumptions did you make?
1. Uthread Library does not provide deadlock-prevention facilities, and it remains the responsibility of users to ensure
that they design deadlock free programs. 

    More specifcally, the program should be designed in a way that threads in READY queue will eventually
get the resources it needs, instead of never again changing state. 

    We implmented a round-robin scheduler, so that every thread is allotted the same time slice to run. The length of the time slice is set in ``uthread_init(time_slice)``. 
    
    The user should choose the length of time slice wisely. For example, in our testcases, there are no system call that blocks, so longer time slice is preferred for less context switching time. 

2. ``disableInterrupts()`` only block SIGVTALRM signal. 
        
    ```
        // Block signals from firing timer interrupt
        static void disableInterrupts()
        {
            /* define a new mask set */
            sigset_t mask_set;

            /* first clear the set (i.e. make it contain no signal numbers) */
            sigemptyset(&mask_set);

            /* add the SIGVTALRM signals to our mask set */
            sigaddset(&mask_set, SIGVTALRM);

            sigprocmask(SIG_BLOCK, &mask_set, NULL);
        }
    ```
    So, when we call  ``disableInterrupts()`` after entering uthread library functions that modify critical data,
we are able to disable timer interrupts (SIGVTALRM), but not other OS interrupts. 

3. This user program should follow the thread create/join strategy, which means that the parent thread will wait for child threads to terminate by calling the pthread_join() function. 
   
   For example, in our testcases, we use a **for** loop to wait on several threads using ``uthread_join()`` function. 

4. Maximum number of threads (MAX_THREAD_NUM) and stack size (STACK_SIZE) per thread are predefine in uthead library.
    The user program should not attempt to exceed the limits. 
##  Descriptions of testcases your provide and how to use them to test your implementation,
e.g. what’s the purpose of each testcase, what specific functionality does each testcase focus
on, what is the expected output of each testcase, etc.
## How did your library pass input/output parameters to a thread entry function? What must ``makecontext`` do “under the hood” to invoke the new thread execution?