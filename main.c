#include <stdio.h>
#include "type.h"
#include "view.h"
#include "edit.h"

int main(int argc, char *argv[])
{
    ViewInfo viewInfo;
    EditInfo editInfo;
    if(argc < 2)
    {
        printf("ERORR: ./a.out : INVALID ARGUMENTS\nUSAGE\n");
        printf("To view please pass like : ./a.out -v mp3file_name\n");
        printf("To edit please pass like : ./a.out -e -t/-a/-A/-M/-y/-c mp3file_name\n");
        return 0;
    }
    if(check_operation_type(argv[1][1]) == e_view)
    {
        if(argc == 3 && read_and_validate_view_args(argv, &viewInfo) == e_failure)
        {
            printf("ERORR: ./a.out : INVALID ARGUMENTS\nUSAGE\n");
            printf("To view please pass like : ./a.out -v mp3file_name\n");
            printf("To edit please pass like : ./a.out -e -t/-a/-A/-M/-y/-c 'New data' mp3file_name\n");
            return 0;
        }
        view_operation(&viewInfo);
    }
    else if(check_operation_type(argv[1][1]) == e_edit)
    {
        if(argc == 5 && read_and_validate_view_args(argv, &editInfo) == e_failure)
        {
            printf("ERORR: ./a.out : INVALID ARGUMENTS\nUSAGE\n");
            printf("To view please pass like : ./a.out -v mp3file_name\n");
            printf("To edit please pass like : ./a.out -e (-t/-a/-A/-M/-y/-c) 'New data' mp3file_name\n");
            return 0;
        }
        edit_operation(&editInfo);
    }
    else if(check_operation_type(argv[1][1]) == '-' && check_operation_type(argv[1][2]) == e_help)
    {
        printf("1. -v  -> to view mp3 file contents\n");
        printf("2. -e  -> to edit mp3 file contents\n");
        printf("\t 2.1. -t  -> to edit song title\n");
        printf("\t 2.1. -a  -> to edit artist name\n");
        printf("\t 2.1. -A  -> to edit album name\n");
        printf("\t 2.1. -y  -> to edit year\n");
        printf("\t 2.1. -m  -> to edit content\n");
        printf("\t 2.1. -c  -> to edit comment\n");
        return 0;
    }
    else
    {
        printf("Invalid operation...\n");
        printf("ERORR: ./a.out : INVALID ARGUMENTS\nUSAGE\n");
        printf("To view please pass like : ./a.out -v mp3file_name\n");
        printf("To edit please pass like : ./a.out -e -t/-a/-A/-M/-y/-c mp3file_name\n");
        return 0;
    }
    return 0;
}
    
OperationType check_operation_type(char opt)
{
    if(opt == 'v')
    {       
        return e_view;
    }
    else if(opt == 'e')
    {
        return e_edit;
    }
    else if(opt == 'h')
    {
        return e_help;
    }
    else
    {
        return e_unsupported;
    }
}