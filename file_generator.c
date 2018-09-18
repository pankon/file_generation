/*******************************************************************************
 * file_generator.c
 * File Generation Template Tools
 * Pankonworks
 * Nathan Pankowsky
 * Writes to a .c file and a .h file
 * 6/6/2017
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>      /* toupper */
#include "file_generator.h"

void PrintEmptyLine(char *filename, char filetype);

/* TODO Test */
void PrintFile(file_gen_t *file_template)
{
    int i = 0;
    char buffer[1024] = "";

    printf("Printing to file %s\n", file_template->filename);

    PrintHeader(
        file_template->filename,
        file_template->header.summary,
        file_template->header.name,
        file_template->header.description,
        file_template->header.includes
    );

    PrintGuard(file_template->filename);
    
    PrintEmptyLine(file_template->filename, 'c');
    PrintEmptyLine(file_template->filename, 'h');

    for (i = 0; (NULL != file_template->lines[i].line); i++)
    {
        PrintLine(
            file_template->filename,
            file_template->lines[i].line,
            file_template->lines[i].file_type
        );
        if (NULL != file_template->lines[i + 1].line)
        {
            PrintEmptyLine(
                file_template->filename, 
                file_template->lines[i].file_type
            );
        }
    }

    for (
        i = 0;
        (NULL != file_template->signatures[i].signature) &&
        (NULL != file_template->signatures[i].precomment) && 
        (NULL != file_template->signatures[i].return_type) &&
        (NULL != file_template->signatures[i].func_name);
        ++i
    )
    {
        struct signature sig = file_template->signatures[i];
        if (NULL == strstr(sig.func_name, "main"))
        {
            sprintf(
                buffer,
                "%s%s%s(%s)",
                sig.return_type,
                file_template->prefix,
                sig.func_name,
                sig.signature
            );
        }
        else
        {
            sprintf(
                buffer,
                "%s%s(%s)",
                sig.return_type,
                sig.func_name,
                sig.signature
            );
        }
        PrintFunction(
            file_template->filename,
            sig.precomment,
            buffer,
            "",
            ""
        );
    }

    PrintFooterGuard(file_template->filename);
}

/* Print header to files
 * Writes in the following format:
 *
 *      filename.[c,h]
 *      summary
 *      name
 *      description
 *      date (generated)
 *
 *      (c file)#include "filename.h"
 * opens and closes the file, overwriting contents
 */
void PrintHeader(
    char *filename, char *summary, char *name, char *description,
    char *includes
)
{
    FILE *fp = NULL;
    int lines = 0;
    int line_count = 8;
    char *file_template = NULL;
    char *file_format = "%s.%c";
    char *filetypes = "hc";
    char *header[] = {
        "/**************************************",
        " * %s.%c",
        " * %s",
        " * Pankonworks",
        " * %s",
        " * %s",
        " * %s",
        "***************************************/"
    };
    char *fills[9] = {
        NULL
    };
    time_t now = {0};

    time(&now); /* set to current time */

    if (NULL == (file_template = malloc(strlen(filename) + 3)))
    {
        fputs("Error in malloc!\n", stderr);
        return;
    }

    fills[0] = " ";
    fills[1] = filename;
    fills[2] = summary;
    fills[3] = " ";
    fills[4] = name;
    fills[5] = description;
    fills[6] = asctime( localtime(&now) );
    fills[7] = " ";

    memset(fills[6] + strlen(fills[6]) - 1, '\0', 1);

    while ('\0' != *filetypes)
    {
        lines = 0;
        sprintf(file_template, file_format, filename, *filetypes);
        fp = fopen(file_template, "w");

        if (NULL == fp)
        {
            fputs("Error opening file!\n", stderr);
            return;
        }

        while (lines < line_count)
        {
            printf("%d\n", lines);
            if (' ' != *fills[lines])
            {
                printf("i: %d, %s\nlen: %lu\n", lines, header[lines], strlen(header[lines]));
                if (8 == strlen(header[lines]))
                {
                    printf(header[lines], fills[lines], *filetypes);
                    putchar('\n');
                    fprintf(fp, header[lines], fills[lines], *filetypes);
                }
                else
                {
                    printf(header[lines], fills[lines]);
                    if (6 != lines)
                    {
                        putchar('\n');
                    }
                    fprintf(fp, header[lines], fills[lines]);
                }
            }
            else
            {
                fprintf(fp, header[lines], fills[lines]);
            }
            fputc('\n', fp);
            ++lines;
        }

        if ('c' == *filetypes)
        {
            fputs("\n", fp);
            fputs(includes, fp);
            sprintf(file_template, file_format, filename, 'h');
            fprintf(fp, "#include \"%s\"\n", file_template);
        }

        if (fclose(fp))
        {
            fputs("Error closing file!\n", stderr);
        }
        ++filetypes;
    }

    free(file_template);
    file_template = NULL;
}

void PrintMultilineComment(FILE *fp, char *comment)
{
    char *current = comment;
    
    if (NULL == current)
    {
        return;
    }
    
    fputs("/* \n * ", fp);
    while ('\0' != *comment)
    {
        fputc(*comment, fp);
        if ('\n' == *comment)
        {
            fputs(" * ", fp);
        }
        ++comment;
    }
    fputs("\n */ \n", fp);
}

/* Print a declaration to the filename.h file
 * and the body of the function to the filename.c file
 *
 * writes to the .h file:
 *      signature;
 * writes to the .c file:
 *      preprocessor
 *      signature (unless signature has main in it)
 *      {
 *          body
 *      }
 *      postbody
 * opens and closes the file, appending contents
 */
void PrintFunction(char *filename, char *precomment,
                   char *signature, char *body, char *postbody
                  )
{
    FILE *fp = NULL;
    char *file_template = NULL;
    char *file_format = "%s.%c";
    char *filetypes = "hc";

    if (NULL == (file_template = malloc(strlen(filename) + 3)))
    {
        fputs("Error in malloc!\n", stderr);
        return;
    }

    while ('\0' != *filetypes)
    {
        sprintf(file_template, file_format, filename, *filetypes);
        fp = fopen(file_template, "a");

        if (NULL == fp)
        {
            fputs("Error opening file!\n", stderr);
            return;
        }

        /* print signature */
        fputs("\n", fp);
        if (precomment)
        {
            PrintMultilineComment(fp, precomment);
        }

        if (('h' == *filetypes && NULL == strstr(signature, "main")) ||
                 ('h' != *filetypes)
                )
        {
            fputs(signature, fp);
        }

        if ('h' != *filetypes)
        {
            fputs("\n{\n", fp);
            fputs(body, fp);
            fputs("\n}\n", fp);
            if (postbody)
            {
                if ('\0' != *postbody)
                {
                    fputs("\n", fp);
                    fputs(postbody, fp);
                }
            }
        }
        else if (NULL == strstr(signature, "main"))
        {
            fputs(";\n", fp);
        }

        if (fclose(fp))
        {
            fputs("Error closing file!\n", stderr);
        }
        ++filetypes;
    }

    free(file_template);
    file_template = NULL;
}

/* Print if guard to .h file */
void PrintGuard(char *filename)
{
    FILE *fp = NULL;
    char *file_template = NULL;
    char *file_format = "%s.%c";
    char *filetypes = "hc";
    char *upper_file = NULL;
    char *filename_end = NULL;

    if (NULL == (file_template = malloc(strlen(filename) + 3)))
    {
        fputs("Error in malloc!\n", stderr);
        return;
    }

    if (NULL == (upper_file = malloc(strlen(filename) + 3)))
    {
        fputs("Error in malloc!\n", stderr);
        return;
    }

    sprintf(file_template, file_format, filename, *filetypes);
    fp = fopen(file_template, "a");

    if (NULL == fp)
    {
        fputs("Error opening file!\n", stderr);
        return;
    }

    strcpy(upper_file, filename);
    filename_end = upper_file + strlen(filename) + 1;
    filename = filename + (filename_end - upper_file);
    while (upper_file <= filename_end)
    {
        *filename_end = toupper(*filename);
        --filename_end;
        --filename;
    }

    fprintf(
        fp, "\n#ifndef __%s_H__\n#define __%s_H__\n",
        upper_file,
        upper_file
    );

    if (fclose(fp))
    {
        fputs("Error closing file!\n", stderr);
    }

    free(file_template);
    file_template = NULL;
    free(upper_file);
    upper_file = NULL;
}

/* Print endif guard with comment to .h file */
void PrintFooterGuard(char *filename)
{
    FILE *fp = NULL;
    char *file_template = NULL;
    char *file_format = "%s.%c";
    char *filetypes = "hc";
    char *upper_file = NULL;
    char *filename_end = NULL;

    if (NULL == (file_template = malloc(strlen(filename) + 3)))
    {
        fputs("Error in malloc!\n", stderr);
        return;
    }

    if (NULL == (upper_file = malloc(strlen(filename) + 2)))
    {
        fputs("Error in malloc!\n", stderr);
        return;
    }

    sprintf(file_template, file_format, filename, *filetypes);
    fp = fopen(file_template, "a");

    if (NULL == fp)
    {
        fputs("Error opening file!\n", stderr);
        return;
    }

    strcpy(upper_file, filename);
    filename_end = upper_file + strlen(filename) + 1;
    filename = filename + (filename_end - upper_file);
    while (upper_file <= filename_end)
    {
        *filename_end = toupper(*filename);
        --filename_end;
        --filename;
    }

    fprintf(
        fp, "\n#endif /* __%s_H__ */\n",
        upper_file
    );

    if (fclose(fp))
    {
        fputs("Error closing file!\n", stderr);
    }

    free(file_template);
    file_template = NULL;
    free(upper_file);
    upper_file = NULL;
}

/* Generic append to filename.filetype */
void PrintLine(char *filename, char *line, char file_type)
{
    FILE *fp = NULL;
    char *file_template = NULL;
    char *file_format = "%s.%c";

    if (NULL == (file_template = malloc(strlen(filename) + 3)))
    {
        fputs("Error in malloc!\n", stderr);
        return;
    }

    sprintf(file_template, file_format, filename, file_type);
    fp = fopen(file_template, "a");

    if (NULL == fp)
    {
        fputs("Error opening file!\n", stderr);
        free(file_template);
        file_template = NULL;
        return;
    }

    fputs(line, fp);
    fputc('\n', fp);

    if (fclose(fp))
    {
        fputs("Error closing file!\n", stderr);
    }

    free(file_template);
    file_template = NULL;
}

void PrintEmptyLine(char *filename, char filetype)
{
    PrintLine(filename, "", filetype);
}
