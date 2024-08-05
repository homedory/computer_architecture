/* Assembler code fragment for LC-2K */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000
#define MAXSIZE 55

struct KeyValue;

typedef struct KeyValue {
	char key[10];
	int value;
	struct KeyValue *next;
} KeyValue; 

KeyValue *labelList;

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
void addLabel(KeyValue *, char *, int);
int getLabelValue(KeyValue *, char *);
void concatRegCode(char *, char *);
void concatOffset(char *, char *);
void concatBranchOffset(char *, char *, int);
void concatFill(char *, char *);
int binaryToDecimal(char *);

int main(int argc, char *argv[]) 
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], 
			 arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
				argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}
  
	
	/* here is an example for how to use readAndParse to read a line from
		 inFilePtr */

	/* TODO: Phase-1 label calculation */
	labelList = (KeyValue*)malloc(sizeof(KeyValue));

	int lineNum = 0;
	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		// no labels	
		if (label[0] == '\0') {
			lineNum++;
			continue;
		}
		addLabel(labelList, label, lineNum);
		lineNum++;
	}
	/* this is how to rewind the file ptr so that you start reading from the
		 beginning of the file */
	rewind(inFilePtr);
	
	lineNum = 0;

	/* TODO: Phase-2 generate machine codes to outfile */
	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)	) {
		char code[33] = "0000000";
		if (!strcmp(opcode, "add")) {
			if (!strcmp(arg2, "0")) {
				printf("error: 0 register write\n");
				exit(1);
			}
			strcat(code, "000");
			concatRegCode(code, arg0);
			concatRegCode(code, arg1);
			strcat(code, "0000000000000");
			concatRegCode(code, arg2);
		} else if (!strcmp(opcode, "nor")) {
			if (!strcmp(arg2, "0")) {
				printf("error: 0 register write\n");
				exit(1);
			}
			strcat(code, "001");
			concatRegCode(code, arg0);
			concatRegCode(code, arg1);
			strcat(code, "0000000000000");
			concatRegCode(code, arg2);
		} else if (!strcmp(opcode, "lw")) {
			if (!strcmp(arg1, "0")) {
				printf("error: 0 register write\n");
				exit(1);
			}
			strcat(code, "010");
			concatRegCode(code, arg0);
			concatRegCode(code, arg1);
			concatOffset(code, arg2);
		} else if (!strcmp(opcode, "sw")) {
			strcat(code, "011");
			concatRegCode(code, arg0);
			concatRegCode(code, arg1);
			concatOffset(code, arg2);
		} else if (!strcmp(opcode, "beq")) {
			strcat(code, "100");
			concatRegCode(code, arg0);
			concatRegCode(code, arg1);
			concatBranchOffset(code, arg2, lineNum + 1);
		} else if (!strcmp(opcode, "jalr")) {
			if (!strcmp(arg1, "0")) {
				printf("error: 0 register write\n");
				exit(1);
			}
			strcat(code, "101");
			concatRegCode(code, arg0);
			concatRegCode(code, arg1);
		} else if (!strcmp(opcode, "halt")) {
			strcat(code, "110");
			strcat(code, "0000000000000000000000");
		} else if (!strcmp(opcode, "noop")) {
			strcat(code, "111");
			strcat(code, "0000000000000000000000");
		} else if (!strcmp(opcode, ".fill")) {
			code[0] = '\0';
			concatFill(code, arg0);
		} else {
			printf("error: wrong instruction\n");
			exit(1);
		}
		fprintf(outFilePtr, "%d\n", binaryToDecimal(code));
		lineNum++;
	}
	/* after doing a readAndParse, you may want to do the following to test the
		 opcode */

	if (inFilePtr) {
		fclose(inFilePtr);
	}
	if (outFilePtr) {
		fclose(outFilePtr);
	}
	return(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
		char *arg1, char *arg2)
{
	char line[MAXLINELENGTH];
	char *ptr = line;

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		/* reached end of file */
		return(0);
	}

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL) {
		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label)) {
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
	}

	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 */
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%"
			"[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
	return(1);
}

int isNumber(char *string)
{
	// return 1 if string is a number
	int i;
	return( (sscanf(string, "%d", &i)) == 1);
}

void addLabel(KeyValue *start, char *label, int value) 
{
	KeyValue *p = start;
	while(p->next != NULL) {
		p = p->next;
		if (strcmp(p->key, label) == 0) {
			printf("error: same label name\n");
			exit(1);
		}
	}
	
	KeyValue *np = (KeyValue*) malloc(sizeof(KeyValue));
	strcpy(np->key, label);
	np->value = value;
	p->next = np;
}

int getLabelValue(KeyValue *start, char *label)
{
	KeyValue *p = start->next;

	while(p != NULL) {
		if (strcmp(p->key, label) == 0)
			return p->value;
		p = p->next;
	}

	printf("error: no label found\n");
	exit(1);
}

void concatRegCode(char *code, char *reg) 
{
	if (!strcmp(reg, "0"))
		strcat(code, "000");
	else if (!strcmp(reg, "1"))
		strcat(code, "001"); 
	else if (!strcmp(reg, "2"))
		strcat(code, "010"); 
	else if (!strcmp(reg, "3"))
		strcat(code, "011"); 
	else if (!strcmp(reg, "4"))
		strcat(code, "100"); 
	else if (!strcmp(reg, "5"))
		strcat(code, "101"); 
	else if (!strcmp(reg, "6"))
		strcat(code, "110"); 
	else if (!strcmp(reg, "7"))
		strcat(code, "111"); 
	else {
		printf("error: wrong register input\n");
		exit(1);
	}
}

void concatOffset(char *code, char *offset) 
{
	int value = 0;
	if (isNumber(offset))
		value = atoi(offset);
	else {
		value = getLabelValue(labelList, offset);
	}	
	if (value > 32767 || value < -32768) {
		printf("error: offset out of range\n");
		exit(1);
	}

	// convert the value into a binary number
	char binary[17];

	int isNegative = value < 0;

	if (isNegative)
		value = -value;

	for (int i = 15; i >= 0; i--) {
		binary[i] = (value & 1) ? '1' : '0';
		value = value >> 1;
	}
	
	// negative number
	if (isNegative) {
		// flip bits
		for (int i = 0; i <= 15; i++) {
			binary[i] = (binary[i] == '0') ? '1' : '0';
		}
		// add 1
		for (int i = 15; i >= 0; i--) {
			if (binary[i] == '1') {
				binary[i] = '0';
			} else {
				binary[i] = '1';
				break;
			}
		}
	}
	binary[16] = '\0';
	strcat(code, binary);
}

void concatBranchOffset(char *code, char *offset, int pc) 
{
	int value = 0;
	if (isNumber(offset))
		value = atoi(offset);
	else {
		value = getLabelValue(labelList, offset) - pc;
	}	
	if (value > 32767 || value < -32768) {
		printf("error: offset out of range\n");
		exit(1);
	}

	// convert the value into a binary number
	char binary[17];
	int isNegative = value < 0;

	if (isNegative)
		value = -value;

	for (int i = 15; i >= 0; i--) {
		binary[i] = (value & 1) ? '1' : '0';
		value = value >> 1;
	}

	if (isNegative) {
		for (int i = 0; i <= 15; i++) {
			binary[i] = (binary[i] == '0') ? '1' : '0';
		}
		for (int i = 15; i >= 0; i--) {
			if (binary[i] == '1') {
				binary[i] = '0';
			} else {
				binary[i] = '1';
				break;
			}
		}
	}
	binary[16] = '\0';
	strcat(code, binary);
}

void concatFill(char *code, char *field) 
{
	int value = 0;
	if (isNumber(field))
		value = atoi(field);
	else
		value = getLabelValue(labelList, field);

	char binary[33];
	
	for (int i = 31; i >= 0; i--) {
		binary[i] = (value & 1) ? '1' : '0';
		value = value >> 1;
	}
	binary[32] = '\0';
	strcat(code, binary);
}

int binaryToDecimal(char *binNum) 
{
	int isNegative = 0;
	if (binNum[0] == '1')
		isNegative = 1;
	
	int decimal = 0;
	
	for (int i = 31; i >= 0; i --) {
		if (binNum[i] == '1') 
			decimal |= (1 << (31 - i));
	}
  
	if (isNegative) {
		decimal = -(~decimal + 1);
	}
  
	return decimal;
}
