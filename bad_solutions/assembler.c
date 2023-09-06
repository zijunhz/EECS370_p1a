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

int readAndParse(FILE*, char*, char*, char*, char*, char*);
static inline int isNumber(char*);

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

// TODO:

enum Operator c2o(const char* c);
int8_t isInt(const char s[]);

int32_t everything2dec(const char op[], const char arg0[], const char arg1[], const char arg2[]);

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

    /* here is an example for how to use readAndParse to read a line from
        inFilePtr */
    char l[MAXLINELENGTH + 10][5][MAXLINELENGTH] = {'\0'};
    uint32_t lCnt = 0;
    const uint32_t LABEL = 0, OPCODE = 1, ARG0 = 2, ARG1 = 3, ARG2 = 4;
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        // fprintf(outFilePtr, "%s %s %s %s %s\n", label, opcode, arg0, arg1, arg2);
        strcpy(l[lCnt][LABEL], label);
        strcpy(l[lCnt][OPCODE], opcode);
        strcpy(l[lCnt][ARG0], arg0);
        strcpy(l[lCnt][ARG1], arg1);
        strcpy(l[lCnt][ARG2], arg2);
        lCnt++;
    }
    lCnt--;
    for (uint16_t i = 0; i <= lCnt; i++) {
        switch (c2o(l[i][1])) {
            case UNKNOWN:
                exit(1);
            case JALR:
                strcpy(l[i][ARG2], "0");
                break;
            case FILL:
                strcpy(l[i][ARG2], "0");
                strcpy(l[i][ARG1], "0");
                break;
            case NOOP:
            case HALT:
                strcpy(l[i][ARG2], "0");
                strcpy(l[i][ARG1], "0");
                strcpy(l[i][ARG0], "0");
                break;
            default:
        }
        // fprintf(outFilePtr, "%s %s %s %s %s\n", l[i][0], l[i][1], l[i][2], l[i][3], l[i][4]);
        for (uint8_t j = 2; j <= 4; j++) {
            if (!isNumber(l[i][j])) {
                if (strlen(l[i][j]) == 0) {
                    strcpy(l[i][j], "0");
                } else {
                    int8_t foundLabel = false;
                    for (uint16_t k = 0; k <= lCnt; k++) {
                        if (!strcmp(l[k][0], l[i][j])) {
                            if (foundLabel) {
                                exit(1);
                            }
                            foundLabel = true;
                            switch (c2o(l[i][1])) {
                                case BEQ:
                                    sprintf(l[i][j], "%d", k - i - 1);
                                    break;
                                default:
                                    sprintf(l[i][j], "%d", k);
                            }
                        }
                    }
                    if (!foundLabel) {
                        // printf("!!!%s", l[i][j]);
                        exit(1);
                    }
                }
            }
        }
        fprintf(outFilePtr, "%d\n", everything2dec(l[i][1], l[i][2], l[i][3], l[i][4]));
    }

    // ///
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

enum Operator c2o(const char c[]) {
    char s[9][9] = {"add", "nor", "lw", "sw", "beq", "jalr", "halt", "noop", ".fill"};
    enum Operator ops[9] = {ADD, NOR, LW, SW, BEQ, JALR, HALT, NOOP, FILL};
    for (uint8_t i = 0; i < 9; i++) {
        if (!strcmp(s[i], c))
            return ops[i];
    }
    return UNKNOWN;
}

int8_t isInt(const char s[]) {
    char s1[10] = {'\0'};
    sprintf(s1, "%d", atoi(s));
    if (strcmp(s1, s)) {
        printf("%s\n%s\n", s1, s);
    }
    return !strcmp(s1, s);
}

int8_t isOut(const char s[]) {
    return atoi(s) < 0 || atoi(s) > 7;
}

int32_t everything2dec(const char op[], const char arg0[], const char arg1[], const char arg2[]) {
    // const uint8_t SHIFT[5] = {0, 22, 19, 16, 0};
    // opcode, arg0, arg1, arg2
    enum Operator opp = c2o(op);
    if (opp == FILL) {
        return atoi(arg0);
    }
    if (!(isInt(arg0) && isInt(arg1) && isInt(arg2)))
        exit(1);
    if (isOut(arg0) || isOut(arg1))
        exit(1);
    if ((opp == ADD || opp == NOR) && isOut(arg2))
        exit(0);
    if ((opp == LW || opp == SW || opp == BEQ) && (atoi(arg2) > (1 << 15) - 1 || atoi(arg2) < -(1 << 15))) {
        exit(1);
    }
    return ((opp << 22) & ((1 << 25) - (1 << 22))) + ((atoi(arg0) << 19) & ((1 << 22) - (1 << 19))) + ((atoi(arg1) << 16) & ((1 << 19) - (1 << 16))) + ((atoi(arg2)) & ((1 << 16) - 1));
}
