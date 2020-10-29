#include"Archivator.h"
#include"Unarchivator.h"
#include"Modes.h"

char getMode(char* str)
{
    if(strcmp(str, "-c") == 0 || strcmp(str, "--CREAT") == 0)
        return MODE_CREAT;
    if(strcmp(str, "-cv") == 0 || strcmp(str, "--CREATVIEW") == 0)
        return MODE_CREAT_VIEW;

    if(strcmp(str, "-x") == 0 || strcmp(str, "--EXTRACT") == 0)
        return MODE_EXTRACT;
    if(strcmp(str, "-xv") == 0 || strcmp(str, "--EXTRACTVIEW") == 0)
        return MODE_EXTRACT_VIEW;
    return -1;
}

int main(int args, char* argv[])
{
    char mode = getMode(argv[1]);

    if(mode == MODE_CREAT || mode == MODE_CREAT_VIEW)
    {
        Archivator archivator;
        archivator.Do(argv[2], argv[3], mode);
    }
    else
    if(mode == MODE_EXTRACT || mode == MODE_EXTRACT_VIEW)
    {
        Unarchivator unarchivator;
        unarchivator.Do(argv[2], argv[3], mode);
    }
    else
    {
        printf("Error! : not found string mode. Check your parametres");
        return -1;
    }

    return 0;
}
