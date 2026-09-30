#include <stdio.h>
#include "type.h"
#include "view.h"

int main(int argc, char *argv[])
{
    ViewInfo viewInfo;
    if(argc < 2)
    {
        printf("ERORR: ./a.out : INVALID ARGUMENTS\nUSAGE\n");
        printf("To view please pass like : ./a.out -v mp3file_name\n");
        printf("To edit please pass like : ./a.out -e -t/-a/-A/-M/-y/-c mp3file_name\n");
        return 0;
    }
    if(check_operation_type(argv[1][1]) == e_view)
    {
        if(argc < 3)
        {
            printf("ERORR: ./a.out : INVALID ARGUMENTS\nUSAGE\n");
            printf("To view please pass like : ./a.out -v mp3file_name\n");
            printf("To edit please pass like : ./a.out -e -t/-a/-A/-M/-y/-c 'New data' mp3file_name\n");
            return 0;
        }

        if(read_and_validate_view_args(argv, &viewInfo) == e_failure)
        {
            printf("Invalid\n");
            printf("To view please pass like : ./a.out -v mp3file_name\n");
            return 0;
        }
        view_operation(&viewInfo);
    }
    else
    {
        printf("Invalid operation...\n");
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