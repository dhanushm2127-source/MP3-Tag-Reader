#include<stdio.h>
#include<string.h>
#include "type.h"
#include "view.h"

int num = 1;

Status read_and_validate_view_args(char *argv[], ViewInfo *viewInfo)
{
    if(argv[2] == NULL)
    {
        printf("Invalid Input arguments\n");
        return e_failure;
    }
    char *dot = strrchr(argv[2], '.');
    if(dot == NULL || strcmp(dot, ".mp3") != 0)
    {
        printf("Error extension must be '.mp3' only\n");
        return e_failure;
    }

    viewInfo -> view_mp3_fname = argv[2];

    if(open_files(viewInfo) == e_failure)
    {
        printf("Error....File doesnot opened\n");
        return e_failure;
    }

    char signature[4];
    fread(signature, 3, 1, viewInfo -> fptr_view_mp3);
    signature[3] = '\0';
    if(strcmp(signature, "ID3") != 0)
    {
        printf("Error... Signature of MP3 file not matched\n");
        return e_failure;
    }

    //Move the offset to 10th position
    fseek(viewInfo -> fptr_view_mp3, 10, SEEK_SET);
    return e_success;
}

Status open_files(ViewInfo *viewInfo)
{
    //Open mp3 file in read 'r' mode
    viewInfo -> fptr_view_mp3 = fopen(viewInfo -> view_mp3_fname, "rb");
    //Validate file is opening or not
    if(viewInfo -> fptr_view_mp3 == NULL)
    {
        printf("MP3 file is not opened.\n");
        return e_failure;
    } 

    printf("File is opened successfully\n");
    return e_success;
}

uint get_size(unsigned char *size_buffer)
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

Status view_operation(ViewInfo *viewInfo)
{
    char tag_buff[5];
    char size_buff[4];
    uint size;
    printf("%-8s | %-10s | %s\n","SI. No", "Tag", "Tag Info");
    for(int i = 0; i < 6; i++)
    {
        //Read 4 bytes of tag
        fread(tag_buff, 4, 1, viewInfo -> fptr_view_mp3);
        tag_buff[4] = '\0';

        //Read 4 bytes of size
        fread(size_buff, 4, 1, viewInfo -> fptr_view_mp3);
        //To get size in little endian
        size = get_size(size_buff);

        //Skip 3 byte (2 bytes for flag and 1 bytes for '\0)
        fseek(viewInfo -> fptr_view_mp3, 3, SEEK_CUR);
        char buffer[size];
        //Read size - 1 bytes of meta data
        fread(buffer, size - 1, 1, viewInfo -> fptr_view_mp3);
        buffer[size - 1] = '\0';
        
        //Check tag is of "TIT2 / TPE1 / TALB / TYER / TCON / TCOM"
        if(strcmp(tag_buff, "TIT2") == 0)
        {
            printf("%-8d | %-10s | %s\n",num++, tag_buff, buffer);
        }
        else if(strcmp(tag_buff, "TPE1") == 0)
        {
            printf("%-8d | %-10s | %s\n",num++, tag_buff, buffer );
        }
        else if(strcmp(tag_buff, "TALB") == 0)
        {
            printf("%-8d | %-10s | %s\n",num++, tag_buff, buffer);
        }
        else if(strcmp(tag_buff, "TYER") == 0)
        {
            printf("%-8d | %-10s | %s\n",num++, tag_buff, buffer);
        }
        else if(strcmp(tag_buff, "TCON") == 0)
        {
            printf("%-8d | %-10s | %s\n",num++, tag_buff, buffer);
        }
        else if(strcmp(tag_buff, "TCOM") == 0)
        {
            printf("%-8d | %-10s | %s\n",num++, tag_buff, buffer);
        }
    }
    fclose(viewInfo -> fptr_view_mp3);
    return e_success;
}