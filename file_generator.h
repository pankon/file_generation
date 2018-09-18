/*******************************************************************************
 * file_generator.h
 * File Generation Template Tools
 * Infinity Labs R&D
 * Nathan Pankowsky
 * Writes to a .c file and a .h file
 * 6/6/2017
 ******************************************************************************/

#ifndef __FILE_GENERATOR_H__
#define __FILE_GENERATOR_H__

#include <stdio.h>

struct line
{
    char *line;
    char file_type;
};

struct header
{
    char *summary;
    char *name;
    char *description;
    char *includes;
};

/* TODO - add */
struct signature
{
    char *func_name;
    char *signature;
    char *return_type;
    char *precomment;
};

typedef struct
{
    char *filename;
    char *prefix;
    struct header header;
    struct line *lines;
    struct signature *signatures;
} file_gen_t;

void PrintFile(file_gen_t *file_template);

/* Print header to files
 * Writes in the following format:
 *
 *      filename.[c,h]
 *      summary
 *      name
 *      description
 *      date (generated)
 *
 *      (c file)includes
 *      (c file)#include "filename.h"
 * opens and closes the file, overwriting contents
 */
void PrintHeader(
    char *filename, char *summary, char *name, char *description,
    char *includes
);

/* Print a declaration to the filename.h file
 * and the body of the function to the filename.c file
 *
 * writes to the .h file:
 *      signature; (unless signature has main in it)
 * writes to the .c file:
 *      preprocessor
 *      signature
 *      {
 *          body
 *      }
 *      postbody
 * opens and closes the file, appending contents
 */
void PrintFunction(char *filename, char *precomment,
                   char *signature, char *body, char *postbody
                  );
                  
void PrintMultilineComment(FILE *fp, char *comment);

/* Print if guard to .h file */
void PrintGuard(char *filename);

/* Print endif guard with comment to .h file */
void PrintFooterGuard(char *filename);

/* Generic append to filename.filetype */
void PrintLine(char *filename, char *line, char file_type);


#endif /* __FILE_GENERATOR_H__ */
