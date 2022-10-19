# Signal basics

## Signals used in PA1?
signal name: SIGVTALRM 
signal value: 26, 26, 28
default action: terminate the process
comment: Virtual alarm clock

## What is signal handler?
Each signal may have a signal handler, 
which is a function that gets called 
when the process receives that signal. 

## Who will call the signal handler?
No where in you program you have code 
that calls this function directly.
Instead, the operating system "forces" current process to stop.
and call the signal handler function. 

## What happens after that signal handler function returns?
The process continues execution from wherever it happened
to be before the signal was received.
as if this interruption never occurred. 

## Diff between interrupts and signals?
The difference is that while interrupts are sent to the operating system by the hardware, 
Signals are sent to the process by the operating system, or by other processes. 
Note that signals have nothing to do with software interrupts,
which are still sent by the hardware (the CPU itself, in this case). 

# How to avoid signal racing?
``sigprocmask(int how, sigset_t signalsToBeMasked, sigset_t oldmask)``
for first param, namely, int how, there are only three opts:
SIG_BLOCK, SIG_UNBLOCK, SIG_SETMASK
for the second param, sigset_t, 
we always clear the old sigset first,
an example use case would be,:
~~~
    {
        /* define a new mask set */
        sigset_t mask_set;

        /* first clear the set (i.e. make it contain no signal numbers) */
        sigemptyset(&mask_set);

        /* lets add the TSTP and INT signals to our mask set */
        sigaddset(&mask_set, SIGTSTP);
        sigaddset(&mask_set, SIGINT);

        /* and just for fun, lets remove the TSTP signal from the set. */
        sigdelset(&mask_set, SIGTSTP);

        /* finally, lets check if the INT signal is defined in our set */

        if (sigismember(&mask_set, SIGINT)
        printf("signal INT is in our set\n");
        else
        printf("signal INT is not in our set - how strange...\n");

        /* finally, lets make the set contain ALL signals available on our system */
        sigfillset(&mask_set)
    }
~~~

# ``sigaction()``
## What is ``sigaction()`` in C?

It is a system call. 
It has a more complicated interface than  ``signal()``

```
    int sigaction
        (   int sig, 
            const struct sigaction *restrict act, 
            struct sigaction *restrict oact
        );
```
   Then, the question becomes, 

## What is ``struct sigaction``,  (the second param for ``sigaction()``)?

Those ``struct sigactions`` are "dispositions",
meaning they express what to do 
when the given signal (the first param in ``sigaction()``) is received. 

```
   struct sigaction {
    void (*sa_handler)(int); 
    // A pointer to the funtion
    // assigned to handle the signal.
    sigset_t sa_mask;    
    // The mask expresses which signals 
    // the process can receive while it is handling sig.          
    int  sa_flags;            
    };
```
## How to use sigaction() to catch and mask signals? 
Example code: ``signal_handle_with_sigaction.cpp``

## Why shall we use ``sigaction()``?
When we can catch signals with ``sigal(int sig, void (*sighandler_t)(int))``,
and,
mask signals with ``sigprocmask(how, sigset, oldsigset)``?

(Obviously, ``signal()`` and ``sigprocmask()`` is easier and more intuitive to use,

Because using ``sigprocmask()``
does not resolve ALL possible race conditions. 

Look into the count_ctrl_c example, 
it is possible that 
after we entered the signal handler,catch_int(),
but before we managed to call the sigprocmask(),
we receive another signal,
which WILL be called. 

## How to implemet signal() system call using sigaction() system call. 
 Exmaple code would be ``implement_signal_with_sigaction.cpp``

## Resources that I found to be useful: 
[setitimer example](http://www.cs.kent.edu/~ruttan/sysprog/lectures/signals/ticker_demo.c)
[intro to unix signal programming](http://www.cs.kent.edu/~ruttan/sysprog/lectures/signals.html)
[What is `sigaction` in C?](https://jameshfisher.com/2017/01/13/c-sigaction/)