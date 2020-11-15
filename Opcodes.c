#include "Vars.h"
#include "Opcodes.h"

extern int Max_Line;

int i, j, IC;

int ExecuteProc(struct PCB *Current)
{
	int Done = 0;
	PC = Current->PC;
	IC = Current->IC;

	while (!Done && IC > 0)
	/* This is implementing the Instruction Cycle */
	/*First, fetch instruction */
	{
		for (i = 0; i < 6; i++)
			IR[i] = memory[BaseRegister + PC][i]; //instruction fetch

		/*Second, Decode instruction*/

		opcode = ((int)(IR[0]) - 48) * 10;
		opcode += ((int)(IR[1]) - 48);

		/* Provide user information about Program Execution */
		printf("*********************************************************");
		printf("\nIn Program Execution Loop: New PC is %d OPCODE IS %d TIME SLICE (IC) is %d\n\n", PC, opcode, IC);
		/*Now execute instruction and increment PC (unless Branch) */
		switch (opcode){
		case 0:OP0(IR);PC++;break;
		case 1:OP1(IR);PC++;break;
		case 2:OP2(IR);PC++;break;
		case 3:OP3(IR);PC++;break;
		case 4:OP4(IR);PC++;break;
		case 5:OP5(IR);PC++;break;
		case 6:OP6(IR);PC++;break;
		case 7:OP7(IR);PC++;break;
		case 8:OP8(IR);PC++;break;
		case 9:OP9(IR);PC++;break;
		case 10:OP10(IR);PC++;break;
		case 11:OP11(IR);PC++;break;
		case 12:OP12(IR);PC++;break;
		case 13:OP13(IR);PC++;break;
		case 14:OP14(IR);PC++;break;
		case 15:OP15(IR);PC++;break;
		case 16:OP16(IR);PC++;break;
		case 17:OP17(IR);PC++;break;
		case 18:OP18(IR);PC++;break;
		case 19:OP19(IR);PC++;break;
		case 20:OP20(IR);PC++;break;
		case 21:OP21(IR);PC++;break;
		case 22:OP22(IR);PC++;break;
		case 23:OP23(IR);PC++;break;
		case 24:OP24(IR);PC++;break;
		case 25:OP25(IR);PC++;break;
		case 26:OP26(IR);PC++;break;
		case 27:OP27(IR);PC++;break;
		case 28:OP28(IR);PC++;break;
		case 29:OP29(IR);PC++;break;
		case 30:OP30(IR);PC++;break;
		case 31:OP31(IR);PC++;break;
		case 32:OP32(IR);PC++;break;
		case 33:OP33(IR, &PC);break;
		case 34:OP34(IR, &PC);break;
		case 35:OP35(IR, &PC);break;
		case 99:
			printf("ALL DONE\n");
			Done = 1;
			break;
		default:
			printf("Instruction %d not found!~\n", opcode);
			exit(0);
		}

		IC--;
	}
	return (Done);
}

/*These are some helper functions that do most of the 
	  in terms of parsing to obtain operands and to perform
	  memory access operations.
	*/

//This function returns the integer value of operand 1
//when this operand is an immediate two-byte integer.

int ParseOp1(char *IR)
{
	int VAL = (int)(IR[2] - 48) * 10 + (int)(IR[3] - 48);
	return (VAL);
}

// returns the integer value of operand 2 when this operand is a two-byte integer.

int ParseOp2(char *IR)
{
	int VAL = (int)(IR[4] - 48) * 10 + (int)(IR[5] - 48);
	return (VAL);
}

//returns the integer value of operands 1 and 2 combined to form a 4-byte integer.
int ParseOP1andOP2Imm(char *IR)
{
	int VAL = (int)(IR[2] - 48) * 1000 + (int)(IR[3] - 48) * 100 + (int)(IR[4] - 48) * 10 + (int)(IR[5] - 48);
	return (VAL);
}

// returns the register number of the register used as operand  1 of an instruction.
// Can be either Pointer or General-Purpose register.
int ParseOp1Reg(char *IR)
{
	int VAL;
	VAL = (int)(IR[3] - 48);
	return (VAL);
}

// returns the register number of a register used as operand  2 of an instruction.
// Can be either a Pointer or General-Purpose register.
int ParseOp2Reg(char *IR)
{
	int VAL;
	VAL = (int)(IR[5] - 48);
	return (VAL);
}

// returns the data stored at memory location Memory_Location
int FetchData(int Memory_Location)
{
	Memory_Location += BaseRegister; //access from the process's subsection of memory

	// quit if illegal memory location being addressed
	if (Memory_Location < BaseRegister || Memory_Location >= LimitRegister){
		printf("Error detected. Memory Location %d is attempting to be accessed with BaseRegister %d and LimitRegister %d", Memory_Location, BaseRegister, LimitRegister);
		exit(1);
	}

	int VAL;
	VAL = (int)(memory[Memory_Location][2] - 48) * 1000;
	VAL += (int)(memory[Memory_Location][3] - 48) * 100;
	VAL += (int)(memory[Memory_Location][4] - 48) * 10;
	VAL += (int)(memory[Memory_Location][5] - 48);

	return (VAL);
}

//Prints out the contents of the IR on the same line.

void PrintIR(char *IR)
{
	printf("IR: %s\n", IR);
}

void PrintLocation(int Address)
{
	Address += BaseRegister; //access from the process's subsection of memory

	// quit if illegal memory location being addressed
	if (Address < BaseRegister || Address >= LimitRegister){
		printf("Error detected. Memory Location %d is attempting to be accessed with BaseRegister %d and LimitRegister %d", Address, BaseRegister, LimitRegister);
		exit(1);
	}
	
	int i;
	printf("Memory Location[%d]: ", Address);
	for (i = 0; i < 6; i++)
		printf(" %c ", memory[Address][i]);
	printf("\n");
}
/*Converts Value from an int to a character string and stores 
	it in memory[Memory_Location]
	*/

void StoreData(int Memory_Location, int Value)
{
	Memory_Location += BaseRegister; //access from the process's subsection of memory

	// quit if illegal memory location being addressed
	if (Memory_Location < BaseRegister || Memory_Location >= LimitRegister){
		printf("Error detected. Memory Location %d is attempting to be accessed with BaseRegister %d and LimitRegister %d", Memory_Location, BaseRegister, LimitRegister);
		exit(1);
	}

	if (Memory_Location > Max_Line)
		Max_Line = Memory_Location;

	int start = 2;
	char temp[6];
	temp[0] = 'Z';
	temp[1] = 'Z';
	if (Value < 1000)
	{
		start++;
		temp[2] = '0';
	}
	if (Value < 100)
	{
		start++;
		temp[3] = '0';
	}

	if (Value < 10)
	{
		start++;
		temp[4] = '0';
	}

	sprintf(&temp[start], "%d", Value);
	//sleep(1);
	for (int i = 0; i < 6; i++)
		memory[Memory_Location][i] = temp[i];
}

/*Prints out all memory locations used in program*/

/*Prints out the value of the Program Reisters*/
void PrintRegs()
{
	int i, j;
	printf("\n");
	printf("PID: %d BaseRegister: %d LimitRegister: %d\n", BaseRegister/100, BaseRegister, LimitRegister);
	printf("Instructions Remaining in Time Slice (IC): %d\n", IC-1);	//don't include this instruction in count, since it's already completed
	printf("ACC %d P0 %d P1 %d P2 %d P3 %d\n", ACC, PRegs[0], PRegs[1], PRegs[2], PRegs[3]);
	printf("R0 %d R1 %d R2 %d R3 %d\n", RRegs[0], RRegs[1], RRegs[2], RRegs[3]);
	printf("PC is %d\n", PC);
}

void printMEM(int upto)
{
	int i;
	for (i = 0; i < upto; i++)
	{
		printf("Memory[%d]: ", i);
		for (j = 0; j < 6; j++)
			printf("%c ", memory[i][j]);
		printf("\n");
	}
}

/*Simulate Execution of Opcodes*/

void OP0(char *IR)
{
	int PREG, VAL;
	printf("Opcode = 0. Load Pointer Immediate\n");
	PrintIR(IR);
	PREG = ParseOp1Reg(IR);
	VAL = ParseOp2(IR);
	PRegs[PREG] = VAL;
	PrintRegs();
	printf("*************************************************\n\n");
}

void OP1(char *IR)
{
	int PREG, VAL;
	printf("Opcode = 1. ADD Pointer Immediate\n");
	PrintIR(IR);
	PREG = ParseOp1Reg(IR);
	VAL = ParseOp2(IR);
	PRegs[PREG] += VAL;
	PrintRegs();
	printf("************************************************\n\n");
}

void OP2(char *IR)
{
	int PREG, VAL;
	printf("Opcode = 2. Subtract Pointer Immediate\n");
	PrintIR(IR);
	PREG = ParseOp1Reg(IR);
	VAL = ParseOp2(IR);
	PRegs[PREG] -= VAL;
	PrintRegs();
	printf("***********************************************\n\n");
}

void OP3(char *IR)
{
	int PREG, VAL;
	printf("Opcode = 3. Load Accumulator Immediate\n");
	PrintIR(IR);
	VAL = ParseOP1andOP2Imm(IR);
	ACC = VAL;
	PrintRegs();
	printf("************************************************\n\n");
}

void OP4(char *IR)
{
	int PREG, Value, Address;
	printf("Opcode 4: Load ACC Register Addressing\n");
	PrintIR(IR);
	PREG = ParseOp1Reg(IR);
	Address = PRegs[PREG];
	Value = FetchData(Address);
	ACC = Value;
	PrintRegs();
	printf("************************************************\n\n");
}

void OP5(char *IR)
{
	int PREG, Value, Address;
	printf("Opcode 5: Load ACC Direct Addressing\n");
	PrintIR(IR);
	Address = ParseOp1Reg(IR);
	Value = FetchData(Address);
	ACC = Value;
	PrintRegs();
	printf("**********************************************\n\n");
}

void OP6(char *IR)
{
	int PREG, Value, Address;
	printf("Opcode 6: Store ACC Register Addressing\n");
	PrintIR(IR);
	PREG = ParseOp1Reg(IR);
	Address = PRegs[PREG];
	StoreData(Address, ACC);
	PrintRegs();
	PrintLocation(Address);
	printf("********************************************\n\n");
}

void OP7(char *IR)
{
	int PREG, Value, Address;
	printf("Opcode 7: Store ACC Direct Addressing\n");
	PrintIR(IR);
	Address = ParseOP1andOP2Imm(IR);
	StoreData(Address, ACC);
	PrintLocation(Address);
	printf("*********************************************\n\n");
}

void OP8(char *IR)
{
	int PREG, RREG, Value, Address;
	printf("Opcode 8: Store Register to Memory:  Register Addressing\n");
	PrintIR(IR);
	RREG = ParseOp1Reg(IR);
	PREG = ParseOp2Reg(IR);
	Address = PRegs[PREG];
	Value = RRegs[RREG];
	StoreData(Address, Value);
	PrintLocation(Address);
	printf("***********************************************\n\n");
}

void OP9(char *IR)
{
	int RREG, Value, Address;
	printf("Opcode 9: Store Register to Memory: Direct Addressing\n");
	PrintIR(IR);
	RREG = ParseOp1Reg(IR);
	Address = ParseOp2(IR);
	printf("Storing Reg %d Value %d to Memory Address %d\n", RREG, RRegs[RREG], Address);
	StoreData(Address, RRegs[RREG]);
	PrintLocation(Address);
	printf("**********************************************\n\n");
}

void OP10(char *IR)
{
	int RREG, PREG, Value, Address;
	printf("Opcode 10: Load Register From Memory: Register Addressing\n");
	PrintIR(IR);
	RREG = ParseOp1Reg(IR);
	PREG = ParseOp2Reg(IR);
	Address = PRegs[PREG];
	Value = FetchData(Address);
	RRegs[RREG] = Value;
	PrintRegs();
	printf("************************************************\n\n");
}

void OP11(char *IR)
{
	int RREG, PREG, Value, Address;
	printf("Opcode 11: Load Register From Memory: Direct Addressing\n");
	PrintIR(IR);
	RREG = ParseOp1Reg(IR);
	Address = ParseOp2(IR);
	Value = FetchData(Address);
	RRegs[RREG] = Value;
	PrintRegs();
	printf("*****************************************************\n\n");
}

void OP12(char *IR)
{
	int RREG, VAL;
	printf("Opcode = 12. Load Register R0 Immediate\n");
	PrintIR(IR);
	PrintIR(IR);

	VAL = ParseOP1andOP2Imm(IR);
	printf("P1&2Imm returned %d\n", VAL);
	RRegs[0] = VAL;
	PrintRegs();
	printf("******************************************************\n\n");
}

void OP13(char *IR)
{
	int RREG, PREG, Value, Address;
	printf("Opcode 13: Register to Register Transfer\n");
	PrintIR(IR);
	RREG = ParseOp1Reg(IR);
	PREG = ParseOp2Reg(IR);
	RRegs[RREG] = RRegs[PREG];
	PrintRegs();
	printf("******************************************************\n\n");
}

void OP14(char *IR)
{
	int RREG, PREG, Value, Address;
	printf("Opcode 14: Load Accumulator From Register\n");
	PrintIR(IR);
	RREG = ParseOp1Reg(IR);
	ACC = RRegs[RREG];
	PrintRegs();
	printf("***************************************************\n\n");
}

void OP15(char *IR)
{
	int RREG, PREG, Value, Address;
	printf("Opcode 15: Load Register From Accumulator \n");
	PrintIR(IR);
	RREG = ParseOp1Reg(IR);
	RRegs[RREG] = ACC;
	PrintRegs();
	printf("*************************************************\n\n");
}

void OP16(char *IR)
{
	int PREG, VAL;
	printf("Opcode = 16. Add Accumulator Immediate\n");
	PrintIR(IR);
	VAL = ParseOP1andOP2Imm(IR);
	ACC += VAL;
	printf("ADDED %d to ACC.\n", VAL);
	PrintRegs();
	printf("**************************************************\n\n");
}

void OP17(char *IR)
{
	int PREG, VAL;
	printf("Opcode = 17. Substract Accumulator Immediate\n");
	PrintIR(IR);
	VAL = ParseOP1andOP2Imm(IR);
	ACC -= VAL;
	PrintRegs();
	printf("**************************************************\n\n");
}

void OP18(char *IR)
{
	int RREG, VAL;
	printf("Opcode = 18. Add contents of Register to  Accumulator \n");
	PrintIR(IR);
	RREG = ParseOp1Reg(IR);
	ACC += RRegs[RREG];
	PrintRegs();
	printf("****************************************************\n\n");
}

void OP19(char *IR)
{
	int RREG, VAL;
	printf("Opcode = 19. Subtract contents of Register From  Accumulator \n");
	PrintIR(IR);
	RREG = ParseOp1Reg(IR);
	ACC -= RRegs[RREG];
	PrintRegs();
	printf("*****************************************************\n\n");
}

void OP20(char *IR)
{
	int PREG, VAL;
	printf("Opcode = 20. Add to Accumulator Register Addressing\n");
	PrintIR(IR);
	PREG = ParseOp1Reg(IR);
	VAL = FetchData(PRegs[PREG]);
	ACC += VAL;
	PrintRegs();
	printf("**********************************************************\n\n");
}

void OP21(char *IR)
{
	int Address, PREG, VAL;
	printf("Opcode = 21. Add to Accumulator Direct Addressing\n");
	PrintIR(IR);
	Address = ParseOp1(IR);
	VAL = FetchData(Address);
	ACC += VAL;
	PrintRegs();
	printf("*******************************************************\n\n");
}

void OP22(char *IR)
{
	int PREG, VAL;
	printf("Opcode = 22. Subtract From Accumulator Register Addressing\n");
	PrintIR(IR);
	PREG = ParseOp1Reg(IR);
	VAL = FetchData(PRegs[PREG]);
	ACC -= VAL;
	PrintRegs();
	printf("********************************************************\n\n");
}

void OP23(char *IR)
{
	int Address, PREG, VAL;
	printf("Opcode = 23. Subtract From Accumulator Direct Addressing\n");
	PrintIR(IR);
	Address = ParseOp1(IR);
	VAL = FetchData(Address);
	ACC -= VAL;
	PrintRegs();
	printf("*******************************************************\n\n");
}

void OP24(char *IR)
{
	int PREG, VAL;
	printf("Opcode = 24. Compare Equal Register Addressing\n");
	PrintIR(IR);
	PREG = ParseOp1Reg(IR);
	VAL = FetchData(PRegs[PREG]);
	if (ACC == VAL)
		PSW[0] = 'T';
	else
		PSW[0] = 'F';
	printf("PSW[0] = %c\n", PSW[0]);
	printf("*********************************************************\n\n");
}

void OP25(char *IR)
{
	int PREG, VAL;
	printf("Opcode = 25. Compare Less Register Addressing\n");
	PrintIR(IR);
	PREG = ParseOp1Reg(IR);
	VAL = FetchData(PRegs[PREG]);
	if (ACC < VAL)
		PSW[0] = 'T';
	else
		PSW[0] = 'F';
	printf("PSW[0] = %c\n", PSW[0]);
	printf("********************************************************\n\n");
}

void OP26(char *IR)
{
	int PREG, VAL;
	printf("Opcode = 26. Compare Greater Register Addressing\n");
	PrintIR(IR);
	PREG = ParseOp1Reg(IR);
	VAL = FetchData(PRegs[PREG]);
	printf("Pointer Register = P%d. Memory at Location %d is: \n", PREG, PRegs[PREG]);
	PrintLocation(PRegs[PREG]);
	printf("Comparing ACC = %d > %d\n", ACC, VAL);
	if (ACC > VAL)
		PSW[0] = 'T';
	else
		PSW[0] = 'F';
	printf("PSW[0] set to %c\n", PSW[0]);
	printf("***********************************************************\n\n");
}

void OP27(char *IR)
{
	int PREG, VAL;
	printf("Opcode = 27. Compare Greater Immediate\n");
	PrintIR(IR);
	VAL = ParseOP1andOP2Imm(IR);
	if (ACC > VAL)
		PSW[0] = 'T';
	else
		PSW[0] = 'F';
	printf("PSW[0] set to %c\n", PSW[0]);
	printf("************************************************\n\n");
}

void OP28(char *IR)
{
	int PREG, VAL;
	printf("Opcode = 28. Compare Equal Immediate\n");
	PrintIR(IR);

	VAL = ParseOP1andOP2Imm(IR);
	if (ACC == VAL)
		PSW[0] = 'T';
	else
		PSW[0] = 'F';
	printf("PSW[0] set to %c\n", PSW[0]);
	printf("**********************************************\n\n");
}

void OP29(char *IR)
{
	int PREG, VAL;
	printf("Opcode = 29. Compare Less Immediate\n");
	PrintIR(IR);

	VAL = ParseOP1andOP2Imm(IR);
	if (ACC < VAL)
		PSW[0] = 'T';
	else
		PSW[0] = 'F';
	printf("PSW[0] set to %c\n", PSW[0]);
	printf("***********************************************\n\n");
}

void OP30(char *IR)
{
	int RREG, VAL;
	printf("Opcode = 30. Compare Register Equal\n");
	PrintIR(IR);
	RREG = ParseOp1Reg(IR);

	if (ACC == RRegs[RREG])
		PSW[0] = 'T';
	else
		PSW[0] = 'F';
	printf("PSW[0] set to %c\n", PSW[0]);
	printf("***********************************************\n\n");
}

void OP31(char *IR)
{
	int RREG, VAL;
	printf("Opcode = 31. Compare Register Less\n");
	PrintIR(IR);
	RREG = ParseOp1Reg(IR);

	if (ACC < RRegs[RREG])
		PSW[0] = 'T';
	else
		PSW[0] = 'F';
	printf("PSW[0] set to %c\n", PSW[0]);
	printf("*********************************************\n\n");
}

void OP32(char *IR)
{
	int RREG, VAL;
	printf("Opcode = 32. Compare Register Greater\n");
	PrintIR(IR);
	RREG = ParseOp1Reg(IR);

	if (ACC > RRegs[RREG])
		PSW[0] = 'T';
	else
		PSW[0] = 'F';
	printf("PSW[0] set to %c\n", PSW[0]);
	printf("*********************************************\n\n");
}

void OP33(char *IR, short int *PC)
{
	printf("Branch Conditional True\n");
	if (PSW[0] == 'T')
		*PC = ParseOp1(IR);
	else
		*PC = ++(*PC);
	printf("New PC is %d\n", *PC);
	printf("********************************************\n\n");
}

void OP34(char *IR, short int *PC)
{
	printf("Branch Conditional False\n");
	if (PSW[0] == 'F')
		*PC = ParseOp1(IR);
	else
		*PC = ++(*PC);
	printf("New PC is %d\n", (*PC));
	printf("******************************************\n\n");
}

void OP35(char *IR, short int *PC)
{
	printf("Branch Unconditional \n");
	*PC = ParseOp1(IR);
	printf("New PC is %d\n", *PC);
	printf("*****************************************\n\n");
}
