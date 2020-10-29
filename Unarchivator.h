#ifndef UNARCHIVATOR_H
#define UNARCHIVATOR_H

#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dirent.h>
#include<stdlib.h>
#include<stdio.h>
#include<list>
#include<string.h>
#include<locale.h>

class Unarchivator
{
    public:
                        Unarchivator();
        virtual         ~Unarchivator();
        int             Do(char* pathInFile, char* pathOutDir, char mode);


    private:
        int             extract(char* pathInFile, char* pathOutDir);
        int             extractView(char* pathInFile, char* pathOutDir);
        bool            rightExtencion(char* extention);

};

#endif // UNARCHIVATOR_H
