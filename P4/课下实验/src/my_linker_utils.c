/*****************************************************************
 * BUAA Fall 2022 Fundamentals of Computer Hardware
 * Project4 Assembler and Linker
 *****************************************************************
 * my_linker_utils.c
 * Linker Submission
 *****************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "lib/tables.h"
#include "linker-src/linker_utils.h"

/*
 * Detect whether the given instruction needs relocation.
 *
 * Return value:
 *  1 if the instruction needs relocation, 0 otherwise.
 *
 * Arguments:
 *  reltbl: the relocation table.
 *  offset: the address of an instruction in the file.
 * 
 * Hint:
 *  Use get_symbol_for_addr().
 */
int inst_needs_relocation(SymbolTable *reltbl, uint32_t offset) {
    if (get_symbol_for_addr(reltbl, offset) != NULL)
        return 1;
    return 0;
}

/*
 * Builds the symbol table and relocation data for a single file.
 * Read the .data, .text, .symbol, .relocation segments in that order.
 * The size of the .data and .text segments are read and saved in the
 * relocation table of the current file. For the .symbol and .relocation
 * segments, save the symbols in them in the corresponding locations.
 *
 * Return value:
 * 0 if no errors, -1 if error.
 *
 * Arguments:
 * input:            file pointer.
 * symtbl:           symbol table.
 * reldt:            pointer to a Relocdata struct.
 * base_text_offset: base text offset.
 * base_data_offset: base data offset.
 * 
 * Hint:
 *  Use calc_text_size(), calc_data_size(), add_to_symbol_table();
 */
int fill_data(FILE *input, SymbolTable *symtbl, RelocData *reldt, uint32_t base_text_offset, uint32_t base_data_offset) {
    char str[1024];
    char *token;
    while (fgets(str, 1024,input))
    {
        token = strtok(str, LINKER_IGNORE_CHARS);
        if (!strcmp(token, ".data"))
            reldt->data_size = calc_data_size(input);
        else if (!strcmp(token, ".text"))
            reldt->text_size = calc_text_size(input);
        else if (!strcmp(token, ".symbol"))
            add_to_symbol_table(input, symtbl, base_text_offset, base_data_offset);
        else if (!strcmp(token, ".relocation"))
            add_to_symbol_table(input, reldt->table, 0, 0);
    }
    return 0;
}
