#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    FILE *opt, *sym, *inter, *leng, *asml, *objc;

    
    opt = fopen("optab.asm", "r");
    sym = fopen("symtab.asm", "r");
    inter = fopen("intermediate.asm", "r");
    leng = fopen("length.asm", "r");
    asml = fopen("asml.asm", "w");
    objc = fopen("objc.asm", "w");

    char addr[10], label[10], opcode[10], operand[10], mnemonic[10], value[10], symbol[10], symaddr[10];
    int start, proglen;

    fscanf(leng, "%X", &proglen);


    fscanf(inter, "%s%s%s%s", addr, label, opcode, operand);
    if(strcmp(opcode,"START") == 0){
        start = (int)strtol(operand, NULL, 16);
    }
    else{
        start = (int)strtol("0000", NULL, 16);
    }

    fprintf(asml, "%-4s\t%-4s\t%-4s\t%-4s\t%-4s\n", addr, label, opcode, operand, "~");
    fprintf(objc, "H^%-4s^%06X^%06X\n", label, start, proglen);

    fprintf(objc, "T^%06X^%06X", start, proglen);
    fscanf(inter, "%s%s%s%s", addr, label, opcode, operand);

    while(strcmp(opcode, "END") != 0){

        int found  = 0;
        rewind(opt);
        while(fscanf(opt, "%s%s", mnemonic, value) != EOF){
            if(strcmp(opcode,mnemonic) == 0){
                found = 1;

                int sym_found = 0;
                rewind(sym);
                while(fscanf(sym, "%s%s", symbol, symaddr) != EOF){
                    if(strcmp(symbol, operand) == 0){
                        sym_found = 1;
                        fprintf(asml, "%-4s\t%-4s\t%-4s\t%-4s\t%s%s\n", addr, label, opcode, operand, value, symaddr);
                        fprintf(objc, "^%s%s", value, symaddr);
                        break;
                    }
                }

                if(!sym_found){
                    fprintf(asml, "%-4s\t%-4s\t%-4s\t%-4s\t%s%s\n", addr, label, opcode, operand, value, "0000");
                    fprintf(objc, "^%s%s", value, "0000");
                }
                break;
            }
            
        }

        if(!found){
            if(strcmp(opcode,"WORD") == 0){
                int temp = (int)strtol(operand, NULL, 10);
                fprintf(asml, "%-4s\t%-4s\t%-4s\t%-4s\t%06X\n", addr, label, opcode, operand, temp);
                fprintf(objc, "^%06X", temp);

            }
            else if(strcmp(opcode, "BYTE") == 0){
                fprintf(asml, "%-4s\t%-4s\t%-4s\t%-4s\t", addr, label, opcode, operand);
                fprintf(objc, "^");
                if(operand[0] == 'C'){
                    for(int i = 2; i < strlen(operand) - 1; i++){
                        fprintf(asml, "%X", operand[i]);
                        fprintf(objc, "%X", operand[i]);
                    }
                }
                else if(operand[0] == 'X'){
                    for(int i = 2; i < strlen(operand) - 1; i++){
                        fprintf(asml, "%c", operand[i]);
                        fprintf(objc, "%c", operand[i]);
                    }
                }
                fprintf(asml, "\n");
            }
            else{
                fprintf(asml, "%-4s\t%-4s\t%-4s\t%-4s\t%-4s\n", addr, label, opcode, operand, "~");
            }
        }
        fscanf(inter, "%s%s%s%s", addr, label, opcode, operand);
    }

    fprintf(asml, "%-4s\t%-4s\t%-4s\t%-4s\t%-4s\n", addr, label, opcode, operand, "~");
    fprintf(objc, "\nE^%06X", start);

    
    fclose(opt);
    fclose(sym);
    fclose(inter);
    fclose(leng);
    fclose(asml);
    fclose(objc);
}