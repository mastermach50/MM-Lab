#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    FILE *inp, *opt, *sym, *inter, *leng;

    inp = fopen("input.asm", "r");
    opt = fopen("optab.asm", "r");
    sym = fopen("symtab.asm", "w");
    inter = fopen("intermediate.asm", "w");
    leng = fopen("length.asm", "w");

    char label[10], opcode[10], operand[10], mnemonic[10], value[10];
    int start,locctr;

    fscanf(inp, "%s%s%s", label, opcode, operand);

    if(strcmp(opcode, "START") == 0){
        locctr = (int)strtol(operand, NULL, 16);
    }
    else{
        locctr = (int)strtol("0000", NULL, 16);
    }

    start = locctr;


    fprintf(inter,"%-4s\t%-4s\t%-4s\t%-4s\n", "~", label, opcode, operand);
    fscanf(inp, "%s%s%s", label, opcode, operand);

    while(strcmp(opcode,"END") != 0){
        fprintf(inter, "%04X\t", locctr);

        if(strcmp(label,"~") != 0){
            fprintf(sym, "%-4s\t%04X\n", label, locctr);
        }

        int found = 0;
        rewind(opt);
        while(fscanf(opt, "%s%s", mnemonic, value) != EOF){
            if(strcmp(mnemonic, opcode) == 0){
                found = 1;
                locctr += 3;
                break;
            }
        }

        if(!found){
            if(strcmp(opcode, "WORD") == 0){
                locctr += 3;
            }
            else if(strcmp(opcode, "BYTE") == 0){
                if(operand[0] == 'C'){
                    locctr += strlen(operand) - 3;
                }
                else if(operand[0] == 'X'){
                    locctr += (strlen(operand) - 3) / 2;
                }
            }
            else if(strcmp(opcode, "RESW") == 0){
                locctr += 3*atoi(operand);
            }
            else if(strcmp(opcode, "RESB") == 0){
                locctr += atoi(operand);
            }
        }

        fprintf(inter,"%-4s\t%-4s\t%-4s\n", label, opcode, operand);
        fscanf(inp, "%s%s%s", label, opcode, operand);
    }
    fprintf(inter,"%04X\t%-4s\t%-4s\t%-4s\n", locctr, label, opcode, operand);

    int proglen = locctr - start;
    fprintf(leng, "%X", proglen);

    fclose(inp);
    fclose(opt);
    fclose(sym);
    fclose(inter);
    fclose(leng);
}