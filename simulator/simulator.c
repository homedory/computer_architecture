/* LC-2K Instruction-level simulator */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000 

#define OPCODE(X) (((X) >> 22) & 7)
#define REG_A(X) (((X) >> 19) & 7)
#define REG_B(X) (((X) >> 16) & 7)
#define REG_DEST(X) ((X) & 7)
#define OFFSET(X) ((X) & 65535)

typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int num);
void checkMemoryBoundary(int, int);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
            state.numMemory++) {

        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

		/* TODO: */

	/* initialize registers */	
	for (int i = 0; i < 8; i++) {
		state.reg[i] = 0;
	}

	int isRunning = 1;	
	int instCnt = 0;

	while(isRunning) {
		printState(&state);
		instCnt++;
		int inst = state.mem[state.pc];
		state.pc++;

		int opcode = OPCODE(inst);
		int address = 0;
		switch (opcode) {
		// add
		case 0:
			state.reg[REG_DEST(inst)] = state.reg[REG_A(inst)] + state.reg[REG_B(inst)];
			break;
		//nor
		case 1:
			state.reg[REG_DEST(inst)] = ~(state.reg[REG_A(inst)] | state.reg[REG_B(inst)]);
			break;
		// lw
		case 2:
			address = state.reg[REG_A(inst)] + convertNum(OFFSET(inst));
			checkMemoryBoundary(address, state.numMemory);
			state.reg[REG_B(inst)] = state.mem[address];
			break; 
		// sw
		case 3:
			address = state.reg[REG_A(inst)] + convertNum(OFFSET(inst));
			checkMemoryBoundary(address, state.numMemory);
			state.mem[address] = state.reg[REG_B(inst)];
			break;
		// beq
		case 4:
			if (state.reg[REG_A(inst)] == state.reg[REG_B(inst)]) {
				checkMemoryBoundary(state.pc + convertNum(OFFSET(inst)), state.numMemory);
				state.pc = state.pc + convertNum(OFFSET(inst));
			}
			break;
		// jalr
		case 5:
			state.reg[REG_B(inst)] = state.pc;
			checkMemoryBoundary(state.reg[REG_A(inst)], state.numMemory);
			state.pc = state.reg[REG_A(inst)];
			break;
		// halt
		case 6:
			isRunning = 0;
			printf("machine halted\n");
			break;
		// noop
		case 7:
			break;
		default:
			printf("error: wrong instruction format\n");
			exit(1);
			break;
		};
	}

	printf("total of %d instructions executed\n", instCnt);
	printf("final state of machine:\n");
	printState(&state);

    return(0);
}



void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
	/* convert a 16-bit number into a 32-bit Linux integer */
	if (num & (1 << 15)) {
		num -= (1 << 16);
	}
	return (num);
}

void checkMemoryBoundary(int address, int numMemory)
{
	if (address < 0 || address >= numMemory) {
		printf("error: memory access out of bound\n");
	  	exit(1);
  	}
}
