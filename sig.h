////////////////////////////////////////////////////////////////////////////////
// sig.h                                                                      //
// CSCI 312 Project4                                                          //
// Created by Dalton Wright                                                   //
////////////////////////////////////////////////////////////////////////////////
/*
    SIGNALS:The header file contains all the required descriptors
            and libraries for Controller and Compute Pgms.
*/

#ifndef SIG_H
#define SIG_H

#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define INC 1000
#define MAXCP 2
#define SIGNAL1 SIGUSR1
#define SIGNAL2 SIGUSR2
#define BUFLEN 100
#define TRUE 1
#define FALSE 0

struct sigaction sig1Action;
struct sigaction sig2Action;

void SYS_ERR(char* error)
{
    perror(error);
    exit(1);
}

#endif
