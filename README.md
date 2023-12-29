# SIGACTION-Signals

Purpose: To increase your understanding of system calls; especially those involved with
signal processing.

Requirements:
For this assignment you will write a program (the controller) that will synchronize the
actions of other compute intensive processes (compute processes). Signals will be used
to cause this synchronization. The synchronization will be used to obtain an estimate of
the elapsed time required for the compute intensive processes to perform their
computations. This measurement is to be made by the controller. This measurement
cannot be started until the compute processes have initialized and have reported that they
are ready. The controller then tells the compute processes to compute. As each
compute process completes its processing it reports back to the controller that it is done.
The controller then tells the compute processes to print out their results. The exchange
of messages is shown in the diagram below (for simplicity, only one instance of each
command is depicted).
Controller process Compute processes
DT
ready
compute
done
print
Time

Additional requirements are detailed below:

1. This assignment is an individual assignment: You are expected to produce your
own solution to the assignment.

2. Number of compute processes (MAXCP): The value of MAXCP must be
specified via a #define statement.

3. header file: Write and use a header (.h) file. Include in this header file your
#define statements and other appropriate items. The program should be tested
with MAXCP set to 2.

4. Makefile: Write a makefile to control the compilation of your programs.

5. Use of Signals: the ready and done commands are to be indicated by SIGUSR2
signals sent by each compute process to the controller. The compute and print
commands are indicated by SIGUSR1 signals sent by the controller to each
compute process.

6. Controller: the controller is to produce MAXCP child processes; these will exec
the compute program (described below). The controller must control the
execution of the computation as described above and in the figure. It must
measure the length of the time interval of the computation (DT) – see figure. Note
that the interval starts right before the first compute command is issued and ends
with the receipt of the last done command.

7. Compute Processes: The compute program must participate in the exchange of
signals as described above and in the figure.

The computation to be performed is as follows:

• Each process will receive an id value such as 0, 1 or 2.

• For integer c in the interval [0, (id+1)*INC) it is to determine the
maximum number of times that the following relation is true:
where a and b are integers in the interval (0, c). INC should initially be set
to 1000; however, it can be made larger in increments of 1000 if the
computation time (DT) is smaller than 60 seconds.

8. Messages: Output messages must clearly show the following items:
• Sending and receiving of SIGUSR1 and SIGUSR2 signals; the messages
must indicate which process is sending or receiving these signals.
• Measured elapsed time of the computation interval.
• Exit status of the compute processes.
• Termination of the controller.
• Values of the computed counts and related interval to which it is applicable.
c2 = a2 + b2

Other items should be added to this list in order to clearly demonstrate the
processes are working correctly.

------------------------------------------------------------------------------------------
Partial sample output for MAXCP = 2 appears below; measured elapsed time not shown:

compute(152): Sending SIGUSR2 to parent process.

compute(153): Sending SIGUSR2 to parent process.

controller: SIGUSR2 received.

controller: SIGUSR2 received.

controller: Sending SIGUSR1 to child process (152).

controller: Sending SIGUSR1 to child process (153).

compute(152): SIGUSR1 received.

compute(153): SIGUSR1 received.
.
.
.

compute(152): For interval [0,100000): count = nnnn

compute(153): For interval [100000,200000): count = mmmm

compute(152): Terminating normally.

controller: child (152) terminated with exit status: 0

compute(153): Terminating normally.

controller: child (153) terminated with exit status: 0

controller: Complete.
