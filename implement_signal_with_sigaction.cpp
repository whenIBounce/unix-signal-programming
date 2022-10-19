#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>


// allows for a signal to be caught, to be ignored, or to generate an interrupt
sig_t signal(int sig, sig_t handler) {

    // Construct the new disposition
    struct sigaction newDisp;
    newDisp.sa_handler = handler;
    sigemptyset(&newDisp.sa_mask);
    newDisp.sa_flags = SA_RESTART;

    // We'll put the old disposition here
    struct sigaction prevDisp;

    if (sigaction(sig, &newDisp, &prevDisp) == -1) {
      return SIG_ERR;
    } else {
      // User of signal doesn't care about the whole disposition; just the handler
      return prevDisp.sa_handler;
    }
}
