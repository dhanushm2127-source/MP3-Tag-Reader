#include<stdio.h>
#include<string.h>
#include "type.h"
#include "edit.h"

Status get_tag_edit(char edit_tag, EditInfo * editInfo)
{
    switch(edit_tag)
    {
        // Song title
        case 't' :
            editInfo -> get_tag = "TIT2";
            break;
        // Artist name
        case 'a' :
            editInfo -> get_tag = "TPE1";
            break;
        // Album
        case 'A' :
            editInfo -> get_tag = "TALB";
            break;
        // Year
        case 'y' :
            editInfo -> get_tag = "TYER";
            break;
        // Content
        case 'm' :
            editInfo -> get_tag = "TCON";
            break;
        // Comment type
        case 'c' :
            editInfo -> get_tag = "COMM";
            break;
        //Invalid
        default :
            return e_failure;
    }
    return e_success;
}

Status read_and_validate_edit_args(char *argv[], EditInfo *editInfo)
{
    if(get_tag_edit(argv[2][1], editInfo) == e_failure)
    {
        return e_failure;
    }

    /*if(argv[4] == NULL)
    {
        printf("Invalid Input arguments\n");
        return e_failure;
    }*/

    char *dot = strrchr(argv[4], '.');
    if(dot == NULL || strcmp(dot, ".mp3") != 0)
    {
        printf("Error extension must be '.mp3' only\n");
        return e_failure;
    }

    editInfo -> new_data = argv[3];
    editInfo -> edit_mp3_fname = argv[4];
    editInfo -> temp_mp3_fname = "temp.mp3";

    if(open_edit_files(editInfo) == e_failure)
    {
        printf("Error....File doesnot opened\n");
        return e_failure;
    }

    char signature[4];
    if(fread(signature, 3, 1, editInfo -> fptr_edit_mp3) != 1)
    {
        return e_failure;
    }
    signature[3] = '\0';
    if(strcmp(signature, "ID3") != 0)
    {
        printf("Error... Signature of MP3 file not matched\n");
        return e_failure;
    }
    rewind(editInfo -> fptr_edit_mp3);
    return e_success;
}

Status open_edit_files(EditInfo *editInfo)
{
    //Open mp3 file in read 'rb' mode
    editInfo -> fptr_edit_mp3 = fopen(editInfo -> edit_mp3_fname, "rb");
    //Validate file is opening or not
    if(editInfo -> fptr_edit_mp3 == NULL)
    {
        printf("MP3 file is not opened.\n");
        return e_failure;
    } 

    //Open temp mp3 file in read 'wb' mode
    editInfo -> fptr_temp_mp3 = fopen(editInfo -> temp_mp3_fname, "wb");
    //Validate file is opening or not
    if(editInfo -> fptr_temp_mp3 == NULL)
    {
        printf("MP3 file is not opened.\n");
        return e_failure;
    } 
    printf("File is opened successfully\n");
    return e_success;
}

uint get_edit_size(unsigned char *size_buffer)
{
    //Convert Big endian to litte
    for(int i = 0; i < 2; i++)
    {
        unsigned char temp = size_buffer[i];
        size_buffer[i] = size_buffer[3 - i];
        size_buffer[3 - i] = temp;
    }

    //Get size
    uint size;
    unsigned char *ptr = (unsigned char *)&size;
    for(int i = 0; i < 4; i++)
    {
        ptr[i] = size_buffer[i];
    }
    return size;
}

void convert_little_to_big(int size, unsigned char *new_size)
{
    unsigned char *ptr = (unsigned char *)&size;
    for(int i = 0; i < 4; i++)
    {
        new_size[i] = ptr[3 - i];
    }
}

Status edit_operation(EditInfo *editInfo)
{
    char tag_buff[5];
    unsigned char size_buff[4];
    unsigned char new_size[4];
    uint size;
    
    //Copy header from edit file to temp file
    char header_buff[10];
    if(fread(header_buff, 10, 1, editInfo -> fptr_edit_mp3) != 1)
    {
        return e_failure;
    }
    if(fwrite(header_buff, 10, 1, editInfo -> fptr_temp_mp3) != 1)
    {
        return e_failure;
    }

    for(int i = 0; i < 6; i++)
    {
        //Read the tag of 4 byte
        if(fread(tag_buff, 4, 1, editInfo -> fptr_edit_mp3) != 1)
        {
            return e_failure;
        }
        //Store the tag to temp mp3 file
        if(fwrite(tag_buff, 4, 1, editInfo -> fptr_temp_mp3) != 1)
        {
            return e_failure;
        }
        tag_buff[4] = '\0';

        //Reading size as 4 bytes, convert to little to big
        if(fread(tag_buff, 4, 1, editInfo -> fptr_edit_mp3) != 1)
        {
            return e_failure;
        }
        size = get_edit_size(size_buff);
        convert_little_to_big(size, size_buff);

        //Check tags are same or not
        if(strcmp(tag_buff, editInfo -> get_tag) == 0)
        {
            //If not same, copy the size to temp mp3 file in big endian
            convert_little_to_big((strlen(editInfo -> new_data) + 1), new_size);
            if(fwrite(new_size, 1, 4, editInfo -> fptr_temp_mp3) != 1)
            {
                return e_failure;
            }
            //Copy the next 3 byte
            char flag_buffer[3];
            if(fread(flag_buffer, 3, 1, editInfo -> fptr_edit_mp3) != 1)
            {
                return e_failure;
            }
            if(fwrite(flag_buffer, 3, 1, editInfo -> fptr_temp_mp3) != 1)
            {
                return e_failure;
            }

            if(fwrite(editInfo -> new_data, strlen(editInfo -> new_data), 1, editInfo -> fptr_temp_mp3) != 1)
            {
                return e_failure;
            }
            fseek(editInfo -> fptr_edit_mp3, size - 1, SEEK_CUR);
            break;
        }

        if(fwrite(size_buff, 4, 1, editInfo -> fptr_edit_mp3) != 1)
        {
            return e_failure;
        }

        //Read 3 bytes(2 bytes -> flag, 1 byte -> NULL charater)
        char flag_buff[3];
        if(fread(flag_buff, 3, 1, editInfo -> fptr_edit_mp3) != 1)
        {
            return e_failure;
        }
        if(fwrite(flag_buff, 3, 1, editInfo -> fptr_temp_mp3) != 1)
        {
            return e_failure;
        }

        char buff[size];
        if(fread(buff, size - 1, 1, editInfo -> fptr_edit_mp3)  != 1)
        {
            return e_failure;
        }
        if(fwrite(buff, size - 1, 1, editInfo -> fptr_temp_mp3)  != 1)
        {
            return e_failure;
        }
    }
    char data;
    while(fread(&data, 1, 1, editInfo -> fptr_edit_mp3) == 1)
    {
        if(fwrite(&data, 1, 1, editInfo -> fptr_temp_mp3)  != 1)
        {
            return e_failure;
        }
    }
    printf("Edited Successfully\n");
    fclose(editInfo -> fptr_edit_mp3);
    fclose(editInfo -> fptr_temp_mp3);
    return e_success;
}