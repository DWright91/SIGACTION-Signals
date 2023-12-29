////////////////////////////////////////////////////////////////////////////////
// compute.c                                                                  //
// CSCI 312 Project4                                                          //
// Created by Dalton Wright                                                   //
////////////////////////////////////////////////////////////////////////////////
/*
 SIGNALS: The compute.c file is to compute the number of pythagoreans
          between the interval (Id * INC, (Id+1)*INC) (Here INC: 1000).
*/
#include "sig.h"


void signalHandler(int sig)
{
    char buf[BUFLEN];
    sprintf(buf, "compute(%d): SIGUSR1 recieved.\n", getpid());
    printf("%s", buf);
}

// Invoke power function.
int power(int base, int power);
// Invoke the pythagoreanOrNot functions: pythagoreanOrNot as a global variable.
int pythagoreanOrNot(int);

int power(int base, int power)
{
    int result = 1;
    for(int i = 0; i < power ; i++)
    {
        result *= base;
    }
    return  result;
}

// Check if number pythagorean or not.
int pythagoreanOrNot(int num)
{
  int a, b;
  for(a = 0; a < num-1 ; a++)
  {
    for(b = 0; b < num; b++)
    {
      if ((a < b) && (b < num - a - b))
      {
        if ((power(a, 2) + power(b, 2)) == (power(num - a - b, 2)))
        {
          //printf("%d^2 + %d^2 = %d^2 \n", a, b, num-a-b); // Display triplets.
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// Main Function
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    pid_t cpID = getpid();  // ID for Child Process
    pid_t ppID = getppid(); // ID for Parent Process
    int err, ID, i;         // Error, ID of  CHILD Process, and INDEX
    int count = 0;          // Counts pythagorean or relation number
    double timetaken;       // for computation
    clock_t time;           // Sets clock time

    if (argc < 2)
    {
        perror("compute: Invalid Arguments.\n");
        exit(1);
    }
    ID = atoi(argv[1]); // Sets the ID prov by the Parent

    /* Setting Signal Handler */
    memset(&sig2Action, 0, sizeof(sig2Action));
    sig2Action.sa_handler = signalHandler;   // Sets the Signal Handler Function
    sigemptyset(&sig2Action.sa_mask);     // Empty the structure
    sig2Action.sa_flags = 0; // Sets any flags for error

    err = sigaction(SIGNAL1, &sig2Action, NULL); // Binds to SIGNAL1
    if (err == -1)
    {
        printf("compute(%d): Sigaction Error %d\n", cpID, errno);
        exit(2);
    }

    // Send the First Signal to the Parent.
    printf("compute(%d): Sending SIGUSR2 to the parent process.\n", cpID);
    sleep(ID+1);

    // When ready Send the signal SIGUSR2 to the Controller.
    err = kill(ppID, SIGNAL2);
    if (err == EINTR)
    {
        printf("compute(%d): Error occured, couldn't send Signal(SIGUSR2) to Controller: %d\n", cpID, errno);
        exit(6);
    }

    /* Wait for Signal from Parent to begin Computing */
    err = pause();
    if (err == EINTR)
    {
        perror("compute: Error Pause.");
        exit(7);
    }

    time = clock(); // Gets the Start Clock Time.

    // Pythagorean numbers in the interval [id * INC, ((id + 1) * INC)]
    for (i = ID * INC; i < ((ID + 1) * INC); i++)
    {
        if (pythagoreanOrNot(i))
        {
            count++;
        }
    }

    time = clock() - time; // Gets the END Time.

    // Elapsed Time.
    timetaken = ((double)time) / CLOCKS_PER_SEC;

    printf("compute(%d): Sending SIGUSR2 to the parent process.\n", cpID);

    // When done Send the signal SIGUSR2 to the Controller.
    err = kill(ppID, SIGNAL2);
    if (err == -1)
    {
        printf("controller: Error occured, couldn't send Signal(SIGUSR2) to Controller: %d\n", errno);
        exit(8);
    }

    // Wait to Print the Computation.
    err = pause();
    if (err != -1)
    {
        perror("compute: Error Pausing.");
        exit(9);
    }
    sleep(ID +1);
    printf("compute(%d): For interval [%d,%d): count = %d | Time Taken: %f\n", cpID, ID * INC, (ID + 1) * INC, count, timetaken);

    printf("compute(%d): Terminating normally.\n", cpID);

    exit (0);
}

//[rc2282qr@csci4 ~]$ make clean
//rm -f compute
//rm -f controller
//[rc2282qr@csci4 ~]$ make
//gcc -o compute -std=gnu99 compute.c
//gcc -o controller -std=gnu99 controller.c
//[rc2282qr@csci4 ~]$ ./controller
//compute(52123): Sending SIGUSR2 to the parent process.
//compute(52122): Sending SIGUSR2 to the parent process.
//controller: SIGUSR2 recieved.
//controller: SIGUSR2 recieved.
//controller: Sending SIGUSR1 to child process (52122)
//controller: Sending SIGUSR1 to child process (52123)
//compute(52122): SIGUSR1 recieved.
//compute(52123): SIGUSR1 recieved.
//compute(52122): Sending SIGUSR2 to the parent process.
//controller: SIGUSR2 recieved.
//compute(52123): Sending SIGUSR2 to the parent process.
//controller: SIGUSR2 recieved.
//controller: Sending SIGUSR1 to Child Process (52122)
//controller: Sending SIGUSR1 to Child Process (52123)
//compute(52122): SIGUSR1 recieved.
//compute(52123): SIGUSR1 recieved.
//compute(52122): For interval [0,1000): count = 189 | Time Taken: 1.810000
//compute(52122): Terminating normally.
//compute(52123): For interval [1000,2000): count = 202 | Time Taken: 12.630000
//compute(52123): Terminating normally.
//controller: child (52122) terminated with exit status: 0
//controller: child (52123) terminated with exit status: 0
//controller: Complete.
