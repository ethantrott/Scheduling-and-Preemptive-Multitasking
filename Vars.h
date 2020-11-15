#ifndef VARS_H_
#define VARS_H_
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct PCB
        { struct PCB *Next_PCB ;
          int PID ;
          short int PC, PRegs[4] ;
          int IC ; //number of instructions before preemption (i.e., time slice)
          int RRegs[4] ;
          char PSW[2] ;
          int ACC ;
          int BaseReg, LimitReg ;
          /* Note: You cannot store the process' memory in PCB */

        } ;

struct PCB *RQ, *tmp, *RQT, *Current ;

/*These are variables representing the VM itself*/

char IR[6] ;
short int PC ;

short int P0 ; //these are the pointer registers
short int P1 ;
short int P2 ;
short int P3 ;

short int PRegs[4] ;
short int RRegs[4] ;

int R0 ; //GP regs
int R1 ;
int R2 ;
int R3 ;

int BaseRegister, LimitRegister ;

int ACC ;
char PSW[2] ;
char memory [1000][6]  ; 	//this is the program memory for all programs
short int opcode ;            

#endif

