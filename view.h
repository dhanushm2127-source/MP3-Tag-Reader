#ifndef VIEW_H
#define VIEW_H

#include "type.h" // Contains user defined types


typedef struct _ViewInfo
{
    /* View info */
    char *view_mp3_fname;
    FILE *fptr_view_mp3;

} ViewInfo;


/* View function prototype */

/* Check operation type */
OperationType check_operation_type(char *opt);

/* Read and validate args from argv */
Status read_and_validate_view_args(char *argv[], ViewInfo *viewInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(ViewInfo *viewInfo);

/*View operation*/
Status view_operation(ViewInfo *viewInfo);

/*To get file size*/
uint get_size(unsigned char *size_buffer);

#endif