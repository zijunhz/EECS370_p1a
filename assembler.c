/**
 * Project 1
 * Assembler code fragment for LC-2K
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Every LC2K file will contain less than 1000 lines of assembly.
#define MAXLINELENGTH 1000

enum Operator {
    ADD,
    NOR,
    LW,
    SW,
    BEQ,
    JALR,
    HALT,
    NOOP,
    FILL,
    UNKNOWN
};

enum Operator c2o(const char* c);

int readAndParse(FILE*, char*, char*, char*, char*, char*);
static inline int isNumber(char*);
static inline int8_t checkReg(char* s);
int32_t everything2dec(const char* op, const char* arg0, const char* arg1, const char* arg2);

int main(int argc, char** argv) {
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

    char labels[MAXLINELENGTH][MAXLINELENGTH] = {0}, opcodes[MAXLINELENGTH][MAXLINELENGTH] = {0}, arg0s[MAXLINELENGTH][MAXLINELENGTH] = {0}, arg1s[MAXLINELENGTH][MAXLINELENGTH] = {0}, arg2s[MAXLINELENGTH][MAXLINELENGTH] = {0};

    // read in from file to arrs
    uint32_t lineCnt = 0;
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        uint32_t i = lineCnt;
        strcpy(labels[i], label);
        strcpy(opcodes[i], opcode);
        strcpy(arg0s[i], arg0);
        strcpy(arg1s[i], arg1);
        strcpy(arg2s[i], arg2);
        lineCnt++;
    }
    lineCnt--;

    // replace and check error

    for (uint32_t i = 0; i <= lineCnt; i++) {
        enum Operator opc = c2o(opcodes[i]);
        switch (opc) {
            case UNKNOWN:
                exit(1);
            case ADD:
            case NOR:
                if ((!checkReg(arg0s[i])) || (!checkReg(arg1s[i])) || (!checkReg(arg2s[i])))
                    exit(1);
                break;
            case LW:
            case SW:
            case BEQ:
                if ((!checkReg(arg0s[i])) || (!checkReg(arg1s[i])))
                    exit(1);
                break;
            case JALR:
                if ((!checkReg(arg0s[i])) || (!checkReg(arg1s[i])))
                    exit(1);
                strcpy(arg2s[i], "0");
                break;
            case NOOP:
            case HALT:
                strcpy(arg0s[i], "0");
                strcpy(arg1s[i], "0");
                strcpy(arg2s[i], "0");
                break;
            case FILL:
                strcpy(arg1s[i], "0");
                strcpy(arg2s[i], "0");
                break;
            default:
                break;
        }
    }
    for (uint32_t i = 0; i <= lineCnt; i++) {
        enum Operator opc = c2o(opcodes[i]);
        if ((opc == LW || opc == SW || opc == BEQ) && (!isNumber(arg2s[i]))) {
            int32_t targetLine = -1;
            for (int k = 0; k <= lineCnt; k++)
                if (!strcmp(labels[k], arg2s[i])) {
                    if (targetLine != -1)
                        exit(1);
                    targetLine = k;
                }
            if (targetLine == -1)
                exit(1);
            sprintf(arg2s[i], "%d", opc == BEQ ? targetLine - i - 1 : targetLine);
        } else {
            if (opc == FILL && (!isNumber(arg0s[i]))) {
                int32_t targetLine = -1;
                for (int k = 0; k <= lineCnt; k++)
                    if (!strcmp(labels[k], arg0s[i])) {
                        if (targetLine != -1)
                            exit(1);
                        targetLine = k;
                    }
                if (targetLine == -1)
                    exit(1);
                sprintf(arg0s[i], "%d", targetLine);
            }
        }
        if ((opc == LW || opc == SW || opc == BEQ) && (atoi(arg2s[i]) < -32768 || atoi(arg2s[i]) > 32767)) {
            printf("Error: unsupported offsets %s\n", arg2s[i]);
            exit(1);
        }
    }

    // for (uint32_t i = 0; i <= lineCnt; i++)
    //     fprintf(outFilePtr, "%s %s %s %s %s\n", labels[i], opcodes[i], arg0s[i], arg1s[i], arg2s[i]);

    for (uint32_t i = 0; i <= lineCnt; i++)
        fprintf(outFilePtr, "%d\n", everything2dec(opcodes[i], arg0s[i], arg1s[i], arg2s[i]));

    // /* here is an example for how to use readAndParse to read a line from
    //     inFilePtr */
    // if (!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
    //     /* reached end of file */
    // }

    // /* this is how to rewind the file ptr so that you start reading from the
    //     beginning of the file */
    // rewind(inFilePtr);

    // /* after doing a readAndParse, you may want to do the following to test the
    //     opcode */
    // if (!strcmp(opcode, "add")) {
    //     /* do whatever you need to do for opcode "add" */
    // }
    return (0);
}

/*
 * NOTE: The code defined below is not to be modifed as it is implimented correctly.
 */

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
int readAndParse(FILE* inFilePtr, char* label, char* opcode, char* arg0, char* arg1, char* arg2) {
    char line[MAXLINELENGTH];
    char* ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
        /* reached end of file */
        return (0);
    }

    /* check for line too long */
    if (strlen(line) == MAXLINELENGTH - 1) {
        printf("error: line too long\n");
        exit(1);
    }

    // Treat a blank line as end of file.
    // Arguably, we could just ignore and continue, but that could
    // get messy in terms of label line numbers etc.
    char whitespace[4] = {'\t', '\n', '\r', ' '};
    int nonempty_line = 0;
    for (size_t line_idx = 0; line_idx < strlen(line); ++line_idx) {
        int line_char_is_whitespace = 0;
        for (int whitespace_idx = 0; whitespace_idx < 4; ++whitespace_idx) {
            if (line[line_idx] == whitespace[whitespace_idx]) {
                ++line_char_is_whitespace;
                break;
            }
        }
        if (!line_char_is_whitespace) {
            ++nonempty_line;
            break;
        }
    }
    if (nonempty_line == 0) {
        return 0;
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
        /* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
           opcode, arg0, arg1, arg2);

    return (1);
}

static inline int
isNumber(char* string) {
    int num;
    char c;
    return ((sscanf(string, "%d%c", &num, &c)) == 1);
}

enum Operator c2o(const char* c) {
    char s[9][9] = {"add", "nor", "lw", "sw", "beq", "jalr", "halt", "noop", ".fill"};
    enum Operator ops[9] = {ADD, NOR, LW, SW, BEQ, JALR, HALT, NOOP, FILL};
    for (uint8_t i = 0; i < 9; i++) {
        if (!strcmp(s[i], c))
            return ops[i];
    }
    return UNKNOWN;
}

static inline int8_t checkReg(char* s) {
    if (!isNumber(s))
        exit(1);
    int32_t num = atoi(s);
    return num >= 0 && num <= 7;
}

int32_t everything2dec(const char* op, const char* arg0, const char* arg1, const char* arg2) {
    // const uint8_t SHIFT[5] = {0, 22, 19, 16, 0};
    // opcode, arg0, arg1, arg2
    enum Operator opp = c2o(op);
    if (opp == FILL) {
        return atoi(arg0);
    }
    return ((opp << 22) & ((1 << 25) - (1 << 22))) + ((atoi(arg0) << 19) & ((1 << 22) - (1 << 19))) + ((atoi(arg1) << 16) & ((1 << 19) - (1 << 16))) + ((atoi(arg2)) & ((1 << 16) - 1));
}
