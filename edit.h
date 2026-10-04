#ifndef EDIT_H
#define EDIT_H

#include "type.h" // Contains user defined types


typedef struct _EditInfo
{
    /* Edit info */
    char *edit_mp3_fname;
    FILE *fptr_edit_mp3;

    /* Temp MP3 file*/
    char *temp_mp3_fname;
    FILE *fptr_temp_mp3;

    char *get_tag;
    char *new_data;

} EditInfo;


/* Edit function prototype */

/* Check operation type */
OperationType check_operation_type(char opt);

/* Read and validate args from argv */
Status read_and_validate_edit_args(char *argv[], EditInfo *editInfo);

/* Get tag to edit */
Status get_tag_edit(char edit_tag, EditInfo * editInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EditInfo *editInfo);

/*Do edit operation*/
Status edit_operation(EditInfo *editInfo);

/* Convert size to MP# frame-size format */
void convert_little_to_big(int size, unsigned char *new_size);

/*To get frame size*/
uint get_edit_size(unsigned char *size_buffer);

#endif