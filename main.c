/*
*  Ethan Trott
*  Professor Dickens
*  COS 331
*  Project 1
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Vars.h"

extern void printMEM(int);
void Create_PCBs();
void LoadPrograms();
void LoadProgram(int, struct PCB **);
void RestoreState(struct PCB *);
int ExecuteProc(struct PCB *);
void DeletePCB(struct PCB *);
void MvToTail(struct PCB *, struct PCB **);
void SaveState(struct PCB **);
void PrintQ(struct PCB *);
struct PCB *GetNextProcess(struct PCB **);

/*	The function ExecuteProc is external to this program (simply to keep the main program simple), and 
	executes a process for the number of instructions in its "time-slice", 
	which is held in the IC register of its PCB. It returns a 1 if the process has completed 
	its execution (i.e., it has terminated), or a 0 if it has not yet terminated.
*/
extern int ExecuteProc(struct PCB *);

int Max_Line = 0;

/*These are variables representing the VM itself*/

int program_line = 0; // For loading program into Memory

/*These variables are associated with the implementation of the VM*/
int fp;
int i;
int j, k;
char input_line[7];

int main(int argc, char *argv[])
{
        Create_PCBs();
        LoadPrograms();
        while (1)
        {
                Current = GetNextProcess(&RQ);
                RestoreState(Current);
                Current->IC = (rand() % 8) + 8; //using different random values as specified in proj. desc. (8,15) inclusive
                printf("CURRENT PID is %d, TIME SLICE (IC) is %d\n", Current->PID, Current->IC);
                int Completed = ExecuteProc(Current);
                if (Completed)
                {
                        printf("Process (PID) %d complete. It is to be terminated.\n", Current->PID);
                        DeletePCB(Current);

                        //the process is not moved to the tail, so it is effectively removed from RQ on GetNextProcess as its memory has been freed.
                }

                else
                {
                        SaveState(&Current);
                        printf("Process (PID) %d completed time slice. Placing at TAIL of RQ.\n", Current->PID);
                        MvToTail(Current, &RQT);
                        printf("RQT is %d\n", RQT->PID);
                        if (RQ == NULL)
                                RQ = RQT;
                }

                PrintQ(RQ);
                //sleep(1) ;
                if (RQ == NULL)
                        break;
        }

        printMEM(Max_Line + 1);
}

void Create_PCBs()
{
        RQ = (struct PCB *)malloc(sizeof(struct PCB));
        RQ->PID = 0;

        RQ->IC = (rand() % 8) + 8; //using different random values as specified in proj. desc.
        RQ->PC = 0;
        tmp = RQ;
        for (i = 1; i < 10; i++)
        {
                tmp->Next_PCB = (struct PCB *)malloc(sizeof(struct PCB));
                tmp->Next_PCB->PID = i;

                tmp->Next_PCB->IC = (rand() % 8) + 8; //using different random values as specified in proj. desc.
                tmp->Next_PCB->PC = 0;
                tmp->Next_PCB->Next_PCB = NULL;
                tmp = tmp->Next_PCB;
        }

        RQT = tmp;
        RQT->Next_PCB = NULL;
}

void LoadPrograms()
{
        struct PCB *tmp;
        tmp = RQ;
        for (i = 0; i < 10; i++)
        {
                LoadProgram(i, &tmp);
                printf("LimitReg = %d. IC = %d\n", tmp->LimitReg, tmp->IC);
                tmp = tmp->Next_PCB;
        }
}

void LoadProgram(int PID, struct PCB **tmp)
{
        int i, fp;
        int program_line = 100 * PID;

        (*tmp)->BaseReg = program_line;
        (*tmp)->LimitReg = program_line + 100;
        fp = open("Fib.PB", O_RDONLY); //always check the return value.
        printf("Open is %d\n", fp);

        if (fp < 0) //error in read
        {
                printf("Could not open file\n");
                exit(0);
        }

        int ret = read(fp, input_line, 7); //returns number of characters read`

        while (1)
        {
                if (ret <= 0)  //indicates end of file or error
                        break; //breaks out of infinite loop

                printf("Copying Program line %d into memory\n", program_line);
                for (i = 0; i < 6; i++)
                {
                        memory[program_line][i] = input_line[i];
                        printf("%c ", memory[program_line][i]);
                }
                printf("\n");

                ret = read(fp, input_line, 7);
                program_line++; //now at a new line in the prog
        }

        printf("Read in Code. Closing File\n");
        close(fp);
}

/*	This function returns the PCB at the head of the RQ and updates
	RQ to point to the next PCB in the list
*/

struct PCB *GetNextProcess(struct PCB **RQ)
{
        struct PCB *ReturnThis = *RQ; //store current process
        *RQ = (*RQ)->Next_PCB;        //set next process
        return ReturnThis;            //return current process
}

/*	Deletes the PCB (using free) */

void DeletePCB(struct PCB *Current)
{
        if (RQ == Current) RQ = NULL;           //remove head reference if this was it
        free(Current);                          //free memory from Current (this does not free Next_PCB)
}

/*	This function places the PCB pointed to by Current at the tail of the
	Ready Queue and updates the RQT pointer.
*/

void MvToTail(struct PCB *Current, struct PCB **RQT)
{
        if ((*RQT)!=Current){   //if we're not looking at the same process...
                Current->Next_PCB = NULL;   //remove pointer to next process, to avoid infinite looping
                (*RQT)->Next_PCB = Current; //link Current to the current tail
                *RQT = Current;             //set Current as new tail
        }
}

/*	Prints out the elements of a linked list */

void PrintQ(struct PCB *Head)
{
        printf("\nCURRENT READY QUEUE (head to tail)\n");
        if (Head == NULL)
                printf("The ready queue is empty. \n");
        while (Head != NULL)
        { //while there are more processes in the list..
                printf("PID: %d IC: %d\n", Head->PID, Head->IC);
                Head = Head->Next_PCB; //get next process in the list
        }
        printf("\n");
}

/*	This function restores the state of the process that is set to begin its
	execution
*/

void RestoreState(struct PCB *NextProc)
{
        //store PC, BaseRegister, LimitRegister, and ACC
        PC = NextProc->PC;
        BaseRegister = NextProc->BaseReg;
        LimitRegister = NextProc->LimitReg;
        ACC = NextProc->ACC;

        //store PSW
        for (int i = 0; i < 2; i++)
                PSW[i] = NextProc->PSW[i];

        //store PRegs and RRegs
        for (int i = 0; i < 4; i++)
        {
                PRegs[i] = NextProc->PRegs[i];
                RRegs[i] = NextProc->RRegs[i];
        }
}

/*	This function saves the state of the VM into the PCB of the process that
	just completed its "time slice"
*/

void SaveState(struct PCB **PrevProc)
{
        //store PC, BaseRegister, LimitRegister, and ACC
        (*PrevProc)->PC = PC;
        (*PrevProc)->BaseReg = BaseRegister;
        (*PrevProc)->LimitReg = LimitRegister;
        (*PrevProc)->ACC = ACC;

        //store PSW
        for (int i = 0; i < 2; i++)
                (*PrevProc)->PSW[i] = PSW[i];

        //store PRegs and RRegs
        for (int i = 0; i < 4; i++)
        {
                (*PrevProc)->PRegs[i] = PRegs[i];
                (*PrevProc)->RRegs[i] = RRegs[i];
        }
}
