#ifndef ARCHIVATOR_H
#define ARCHIVATOR_H

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

    struct DirDM;
    struct FileDM
    {

        __mode_t                 mode;
        char                    name[1024] = {'\0'};
        long int                size;
        struct DirDM*           root;

                                FileDM(struct DirDM& _root, char* _name, long int _size, __mode_t _mode);
        void                    serialize(int fileDes);

    };


    struct DirDM
    {
        __mode_t                mode;
        char                    name[1024] = {'\0'};
        char                    localname[1024] = {'\0'};
        std::list<FileDM>*      listFiles;
        long                    countFiles;

                                DirDM(char* _name, char* _localname, __mode_t _mode);
        void                    serialize(int fileDes);

    };



class Archivator
{
public:
                                Archivator();
    virtual                     ~Archivator();
    int                         Do(char* pathInput, char* pathOutput, char mode);


private:
    std::list<DirDM>*           getFilesData(char* pathDirectory, char* localPath ,std::list<DirDM>* listDirectories);
    void                        addFile(std::list<DirDM>* list, char* directoryName, char* fileName, long int size, __mode_t _mode);
    int                         archivate(char* pathInput, char* pathOutput);
    int                         archivateView(char* pathInput, char* pathOutput);

};

#endif // ARCHIVATOR_H




