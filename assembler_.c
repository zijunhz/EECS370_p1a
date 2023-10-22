/**
 * Project 1
 * Assembler code fragment for LC-2K
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Every LC2K file will contain less than 1000 lines of assembly.
#define MAXLINELENGTH 1000

struct labels {
    char letters[100];
    int addre;
};

int readAndParse(FILE*, char*, char*, char*, char*, char*);
static inline int isNumber(char*);

int judge_valid_reg(char* arg) {
    int reg = -1;
    reg = atoi(arg);
    if (reg < 0 || reg > 7) {
        return 0;
    }
    while (*arg != '\0') {
        if ((*arg) < '0' || (*arg) > '7') {
            return 0;
        }
        arg++;
    }
    return 1;
}

int judge_valid(int num) {
    if (num < -32768 || num > 32767) {
        return 0;
    }
    return 1;
}

// int add(char *arg0, char *arg1, char *arg2){
//     int val1=0;
//     int val2=0;
//     int val3=0;
//     int result = 0;

//     if(judge_valid_reg(arg0)==0 || judge_valid_reg(arg1)==0 || judge_valid_reg(arg2)==0){
//         printf("Error:Register not integer");
//         exit(1);
//     }

//     val1 = atoi(arg0);
//     val2 = atoi(arg1);
//     val3 = atoi(arg2);
//     result = ((((val1 << 3) + val2) << 16) + val3);
//     return result;
// }

// int nor(char *arg0, char *arg1, char *arg2){
//     int val1=0;
//     int val2=0;
//     int val3=0;
//     int result = 0;

//     if(judge_valid_reg(arg0)==0 || judge_valid_reg(arg1)==0 || judge_valid_reg(arg2)==0){
//         printf("Error:Register not integer");
//         exit(1);
//     }
//     val1 = atoi(arg0);
//     val2 = atoi(arg1);
//     val3 = atoi(arg2);
//     result = ((((((1 << 3) + val1) << 3) + val2) << 16) + val3);
//     return result;
// }

int find(char* arg2, struct labels label_list[], int lab_num, int cur_address) {
    int offset = 0;
    int record_address = -1;
    for (int i = 0; i < lab_num; i++) {
        if (!strcmp(label_list[i].letters, arg2)) {
            record_address = label_list[i].addre;
            offset = record_address - cur_address - 1;
            break;
        }
    }
    if (record_address == -1) {
        printf("Error:address not found");
        exit(1);
    }
    return offset;
}

// int lw(char *arg0, char *arg1, char *arg2, struct labels label_list[], int lab_num){
//     int val1=0;
//     int val2=0;
//     int val3=0;
//     int result = 0;
//     if(judge_valid_reg(arg0)==0 || judge_valid_reg(arg1)==0){
//         printf("Error:Register not integer");
//         exit(1);
//     }
//     val1 = atoi(arg0);
//     val2 = atoi(arg1);
//     result = (0<<31);
//     result |= (1<<23);
//     result |= (val1<<19);
//     result |= (val2<<16);

//     if(isNumber(arg2)){
//         val3 = atoi(arg2);
//         if(judge_valid(val3)==0){
//             printf("Error:Invalid offset");
//             exit(1);
//         }
//         result |= (val3 & 0xFFFF);
//     }
//     else {
//         result |= ((find_lsw(arg2, label_list, lab_num)) & 0xFFFF);
//     }
//     return result;
// }

// int sw(char *arg0, char *arg1, char *arg2, struct labels label_list[], int lab_num){
//     int val1=0;
//     int val2=0;
//     int val3=0;
//     int result = 0;

//     val1 = atoi(arg0);
//     val2 = atoi(arg1);
//     result = (0<<31);
//     result |= (3<<22);
//     result |= (val1<<19);
//     result |= (val2<<16);
//     if(judge_valid_reg(arg0)==0 || judge_valid_reg(arg1)==0){
//         printf("Error:Register not integer");
//         exit(1);
//     }
//     if(isNumber(arg2)){
//         val3 = atoi(arg2);
//         if(judge_valid(val3)==0){
//             printf("Error:Invalid offset");
//             exit(1);
//         }
//         result |= (val3 & 0xFFFF);
//     }
//     else {
//         result |= ((find_lsw(arg2, label_list, lab_num)) & 0xFFFF);
//     }
//     return result;
// }

// int beq(char *arg0, char *arg1, char *arg2, struct labels label_list[], int lab_num, int curr_address){
//     int val1=0;
//     int val2=0;
//     int val3=0;
//     int result = 0;

//     val1 = atoi(arg0);
//     val2 = atoi(arg1);
//     result = (0<<31);
//     result |= (4<<22);
//     result |= (val1<<19);
//     result |= (val2<<16);
//     if(judge_valid_reg(arg0)==0 || judge_valid_reg(arg1)==0){
//         printf("Error:Register not integer");
//         exit(1);
//     }
//     if(isNumber(arg2)){
//         val3 = atoi(arg2);
//         if(judge_valid(val3)==0){
//             printf("Error:Invalid offset");
//             exit(1);
//         }
//         result |= (val3 & 0xFFFF);
//     }
//     else {
//         result |= ((find(arg2, label_list, lab_num, curr_address)) & 0xFFFF);
//     }
//     return result;
// }

// int jalr(char *arg0, char *arg1, char *arg2){
//     int val1=0;
//     int val2=0;
//     int result = 0;

//     if(judge_valid_reg(arg0)==0 || judge_valid_reg(arg1)==0){
//         printf("Error:Register not integer");
//         exit(1);
//     }
//     val1 = atoi(arg0);
//     val2 = atoi(arg1);
//     result = (((((0b101 << 3) + val1) << 3) + val2) << 16);
//     return result;
// }

// int halt(){
//     int result = 0;
//     result = (0b110 << 22);
//     return result;
// }

// int noop(){
//     int result = 0;
//     result = (0b111 << 22);
//     return result;
// }

int find_lsw(char* arg2, struct labels label_list[], int lab_num) {
    int record_address = -1;
    for (int i = 0; i < lab_num; i++) {
        if (!strcmp(label_list[i].letters, arg2)) {
            record_address = label_list[i].addre;
            break;
        }
    }
    if (record_address == -1) {
        printf("Error:address not found");
        exit(1);
    }
    return record_address;
}

int fill(char* arg0, char* arg1, char* arg2, struct labels label_list[], int lab_num) {
    int result = 0;
    if (isNumber(arg0)) {
        result = atoi(arg0);
        if (judge_valid(result) == 0) {
            printf("Error: Invalid number");
            exit(0);
        }
    } else {
        result = find_lsw(arg0, label_list, lab_num);
    }
    return result;
}

void findduplen(struct labels label_list[], int lab_num) {
    size_t length;
    for (int i = 0; i < lab_num; i++) {
        length = strlen(label_list[i].letters);
        if (length > 7) {
            printf("Error: Too long");
            exit(1);
        }
        for (int j = i + 1; j < lab_num; j++) {
            if (strcmp(label_list[i].letters, label_list[j].letters) == 0) {
                printf("Error:Duplicate labels");
                exit(1);
            }
        }
    }
}

int calculate_mc(int i, char* arg0, char* arg1, char* arg2, int addres, struct labels label_list[], int lab_num) {
    int result = 0;
    if (i == 0 || i == 1) {
        if (judge_valid_reg(arg0) == 0 || judge_valid_reg(arg1) == 0 || judge_valid_reg(arg2) == 0) {
            printf("Error:Register not integer");
            exit(1);
        }
        result = ((((((i << 3) + atoi(arg0)) << 3) + atoi(arg1)) << 16) + atoi(arg2));
    } else if (i == 2 || i == 3 || i == 4) {
        if (judge_valid_reg(arg0) == 0 || judge_valid_reg(arg1) == 0) {
            printf("Error:Register not integer");
            exit(1);
        }
        result = ((((((i << 3) + atoi(arg0)) << 3) + atoi(arg1)) << 16));
        if (isNumber(arg2)) {
            if (judge_valid(atoi(arg2)) == 0) {
                printf("Error:Invalid offset");
                exit(1);
            }
            result |= (atoi(arg2) & 0xFFFF);
        } else {
            if (i == 4) {
                result += ((find(arg2, label_list, lab_num, addres)) & 0xFFFF);
            } else {
                result += ((find_lsw(arg2, label_list, lab_num)) & 0xFFFF);
            }
        }
    } else if (i == 5) {
        if (judge_valid_reg(arg0) == 0 || judge_valid_reg(arg1) == 0) {
            printf("Error:Register not integer");
            exit(1);
        }
        result = ((((((i << 3) + atoi(arg0)) << 3) + atoi(arg1)) << 16));

    } else if (i == 6 || i == 7) {
        result = ((((((i << 3) + atoi(arg0)) << 3) + atoi(arg1)) << 16));
    }
    return result;
}

int main(int argc, char** argv) {
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
        arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
    struct labels label_list[MAXLINELENGTH];
    int lab_num = 0;
    char operand[20][10] = {"add", "nor", "lw", "sw", "beq", "jalr", "halt", "noop"};

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
    int addres = 0;
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        if (label[0] != '\0') {
            strcpy(label_list[lab_num].letters, label);
            label_list[lab_num].addre = addres;
            lab_num++;
        }
        addres++;
    }

    findduplen(label_list, lab_num);
    // int length = sizeof(label)/sizeof(label[0]);
    //     for (int i = 0; i < length; i++) {
    //     printf("%c ", label[i]);
    // }

    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    rewind(inFilePtr);

    /* after doing a readAndParse, you may want to do the following to test the
        opcode */
    int result = 0;
    bool flag = false;
    addres = 0;
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        for (int i = 0; i < 8; i++) {
            if (!strcmp(operand[i], opcode)) {
                result = calculate_mc(i, arg0, arg1, arg2, addres, label_list, lab_num);
                flag = true;
            }
        }
        if (!strcmp(".fill", opcode)) {
            flag = true;
            if (isNumber(arg0)) {
                result = atoi(arg0);
                if (judge_valid(result) == 0) {
                    printf("Error: Invalid number");
                    exit(0);
                }
            } else {
                result = find_lsw(arg0, label_list, lab_num);
            }
        }
        if (flag == false) {
            printf("Error: Wrong opcode\n");
            return 1;
        }
        addres++;
        flag = false;
        fprintf(outFilePtr, "%d\n", result);
    }
    // for(int i=0;i<lab_num;i++){
    //     fprintf(outFilePtr, "%s%d\n",label_list[i].letters,label_list[i].addre);
    // }
    fclose(inFilePtr);
    fclose(outFilePtr);
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
