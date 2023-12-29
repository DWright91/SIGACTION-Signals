////////////////////////////////////////////////////////////////////////////////
// controller.c                                                               //
// CSCI 312 Project4                                                          //
// Created by Dalton Wright                                                   //
////////////////////////////////////////////////////////////////////////////////
/*
    Controller.c produces MAXCP child processes and controls the execution of
    the computation as desired.
*/
#include "sig.h"

// Global variables.
pid_t cid[MAXCP];
volatile int ccount;

// Handlers function.
void sHandler1(int sig)
{
    ccount--;
    printf("controller: SIGUSR2 recieved.\n");
}

int main(int argc, char **argv)
{
    // Variable Initialization.
    int err;
    int status;
    int i;
    pid_t result;
    char buf[BUFLEN];
    ccount = MAXCP;

    // Set Signal Handler.
    memset(&sig1Action, 0, sizeof(sig1Action));
    sig1Action.sa_handler = (void (*)(int))sHandler1;
    sigemptyset(&sig1Action.sa_mask);
    sig1Action.sa_flags = 0;

    err = sigaction(SIGNAL2, &sig1Action, NULL); // Bind the SIGACTION to SIGNAL2
    if (err == -1)
    {
        printf("controller2: Signal action Error %d\n", errno);
        exit(2);
    }

    // MAXCP Child Process
    for (i = 0; i < MAXCP; i++)
    {
        cid[i] = fork();

        if (cid[i] == -1)
        {
            SYS_ERR("controller: Error Creating New Child Process\n");
        }

        if (cid[i] == 0)
        {
            // Starts Child Process
            sprintf(buf, "%d", i);
            err = execl("./compute", "compute", buf, (char *)NULL);
            if (err == -1)
            {
                SYS_ERR("CONTROLLER: Error Occured.\n");
            }

            exit(0);
        }
    }

    /* Start Parent Process */

    // Delay process.
    while (ccount)
    {
        usleep(1000);       // Suspends the execution for 1 second.
    }

    ccount = MAXCP;     // Reset the wait process.

    // Send the first signal to start computation to the child process.
    for (int i = 0; i < MAXCP; i++)
    {
        printf("controller: Sending SIGUSR1 to child process (%d)\n", cid[i]);

        // Sends the signal to the child.
        err = kill(cid[i], SIGNAL1);
        if (err == -1)
        {
            printf("controller: Error occured, couldn't send Signal to Process: %d\n", errno);
            exit(4);
        }
    }

    // Delays process.
    while (ccount)
    {
        sleep(1);
    }

    // Sends the first signal to start PRINTING to the child process.
    for (int i = 0; i < MAXCP; i++)
    {

        printf("controller: Sending SIGUSR1 to Child Process (%d)\n", cid[i]);

        // Sends the signal to the child.
        err = kill(cid[i], SIGNAL1);
        if (err == -1)
        {
            printf("controller: Error occured, couldn't send Signal to Process: %d\n", errno);
            exit(5);
        }
    }

    // Waiting for all of the child process to be completed.
    sleep(MAXCP*2);

    // Wait for all the child process and terminate all the process.
    for (int i = 0; i < MAXCP; i++)
    {
        /* Wait for termination */
        result = waitpid(-1, &status, 0);
        if (result == -1)
        {
            SYS_ERR("controller: There is No Child Process at the moment\n");
        }
        printf("controller: child (%d) terminated with exit status: 0\n", cid[i]);
    }

    printf("controller: Complete.\n");

    return (0);
}
