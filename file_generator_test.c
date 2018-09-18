/*******************************************************************************
 * file_generator_test.c
 * File Generation Template Tools
 * Infinity Labs R&D
 * Nathan Pankowsky
 * Writes to a .c file and a .h file
 * 6/6/2017
 ******************************************************************************/

#include <stdio.h>          /* sprintf */
#include <ctype.h>          /* toupper */
#include "file_generator.h"
extern char *filename;
extern char *prefix;
extern struct header header;
extern struct signature signatures[];
extern struct line lines[];
int main(void)
{
    file_gen_t file = { "" };

    file.filename   = filename;
    file.prefix     = prefix;
    file.header     = header;
    file.signatures = signatures;
    file.lines      = lines;

    PrintFile(&file);

    return (0);
}
