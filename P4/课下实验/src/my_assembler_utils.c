/*****************************************************************
 * BUAA Fall 2022 Fundamentals of Computer Hardware
 * Project4 Assembler and Linker
 *****************************************************************
 * my_assembler_utils.c
 * Assembler Submission
 *****************************************************************/
#include <string.h>
#include <stdlib.h>

#include "my_assembler_utils.h"
#include "assembler-src/assembler_utils.h"
#include "lib/translate_utils.h"

/*
 * This function reads .data symbol from INPUT and add them to the SYMTBL
 * Note that in order to distinguish in the symbol table whether a symbol
 * comes from the .data segment or the .text segment, we append a % to the
 * symbol name when adding the .data segment symbol. Since only underscores and
 * letters will appear in legal symbols, distinguishing them by adding % will
 * not cause a conflict between the new symbol and the symbol in the assembly file.
 *
 * Return value:
 *  Return the number of bytes in the .data segment.
 * 
 * Hint:
 *  Read pass_one() in assembler.c first.
 */
int read_data_segment(FILE *input, SymbolTable *symtbl) {
    char str[1024];
    char *token;
    int data_sz=0;
    fgets(str,1024,input);//read a line:".data"
    while(fgets(str,1024,input))
    {
        if(str[0]=='\n'||((str[0]=='\r')&&(str[1]=='\n'))) break;//结束空的那一行
        skip_comment(str);//去掉注释
        token=strtok(str,ASSEMBLER_IGNORE_CHARS);
        if(token==NULL) continue;
        token[strlen(token)-1]='\0';
        if(is_valid_label(token))
        {
            char sym_name[10]="%";
            strcat(sym_name,token);
            add_to_table(symtbl,sym_name,data_sz);
            token= strtok(NULL,ASSEMBLER_IGNORE_CHARS);
            token=strtok(NULL,ASSEMBLER_IGNORE_CHARS);
            long int add_data_sz;
            translate_num(&add_data_sz,token,INT16_MIN,INT16_MAX);
            data_sz+=add_data_sz;
        }
    }
    return data_sz;
}

/* Reads STR and determines whether it is a label (ends in ':'), and if so,
 * whether it is a valid label, and then tries to add it to the symbol table,
 * remerber to replace ':' with '\0'.
 *
 * Four scenarios can happen:
 *  1. STR is not a label (does not end in ':'). Returns 0.
 *  2. STR ends in ':', but is not a valid label. Returns -1.
 *  3a. STR ends in ':' and is a valid label. Addition to symbol table fails.
 *      Returns -1.
 *  3b. STR ends in ':' and is a valid label. Addition to symbol table succeeds.
 *      Returns 1.
 * 
 * Hint:
 *  Use is_valid_label(), add_to_table() and raise_label_error().
 */
int add_if_label(uint32_t input_line, char *str, uint32_t addr, SymbolTable *symtbl) {
    int len = strlen(str);
    if(str[len-1]!=':') return 0;
    str[len-1]='\0';
    if(!is_valid_label(str)) return -1;
    if(add_to_table(symtbl,str,addr)!=0) 
    {
        raise_label_error(input_line,str);
        return -1;
    }
    return 1;
}

/*
 * Convert memory instructions(including lw, lb, sw, sb, lbu) to machine code.
 * Output the instruction to the **OUTPUT**.(Consider using write_inst_hex()).
 * 
 * Arguments:
 *  opcode:     op segment in MIPS machine code
 *  args:       args[0] is the $rt register, and args[2] is the $rs register.
 *             The other cases are illegal and need not be considered
 *  num_args:   length of args array
 *  addr:       Address offset of the current instruction in the file
 * 
 * Hint:
 *  Use translate_reg(), translate_num() and write_inst_hex().
 */
int write_mem(uint8_t opcode, FILE *output, char **args, size_t num_args) {
    uint32_t rt=translate_reg(args[0]);
    //printf("%x\n",rt);
    uint32_t rs=translate_reg(args[2]);
    //printf("%x\n",rs);
    long int offset;
    if(translate_num(&offset,args[1],INT64_MIN,INT64_MAX)==-1) return -1;
    //printf("%lx\n",offset);
    //printf("opcode:%x\n",opcode&0x3f);
    //printf("rs:%x\n",rs&0x1f);
    //printf("rt:%x\n",rt&0x1f);
    //printf("offset:%lx\n",offset&0xffff);
    uint32_t instruction=(opcode<<26)|(rs<<21)|(rt<<16)|(offset&0xffff);
    //printf("%x\n",instruction);
    write_inst_hex(output,instruction);
    return 0;
}
