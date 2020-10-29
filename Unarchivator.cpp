#include "Unarchivator.h"
#include "Modes.h"
#include <math.h>

Unarchivator::Unarchivator()
{
    //ctor
}

Unarchivator::~Unarchivator()
{
    //dtor
}

int Unarchivator::extract(char* pathInFile, char* pathOutDir)
{
    int _fileInputFileDes = 0;

    if((_fileInputFileDes = open(pathInFile, O_RDONLY)) < 0)
    {
        printf("cannot open file: %s\n", pathInFile);
        return -1;
    }

    char buf = 0;
    int nread = 0;


    while((nread = read(_fileInputFileDes, &buf, 1)) != 0)
    {
        lseek(_fileInputFileDes, -1 , SEEK_CUR);

        DIR* dir;
        struct dirent* entry;
        struct stat statbuf;
        long int headDir;


        //Считать sizeof(short) байт - размер названия
        short _sizeNameDir = 0;
        read(_fileInputFileDes, &_sizeNameDir, sizeof(short));

        //Считать имя директории
        char _nameDir[1024] = {'\0'};
        read(_fileInputFileDes, _nameDir, _sizeNameDir);

        //Считать модификатор доступа файла
        __mode_t _mode;
        read(_fileInputFileDes, &_mode, sizeof(__mode_t));

        //Считать sizeof(long) байт - кол-во файлов в дирректории
        long _countFiles = 0;
        read(_fileInputFileDes, &_countFiles, sizeof(_countFiles));

        char path[1024] = {'\0'};
        strcat(path, pathOutDir);
        if(_sizeNameDir != 0)
        {
            strcat(path, "/");
            strcat(path, _nameDir);
        }

        mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO | S_IROTH | S_IXOTH);
        //mkdir(path, _mode);

        if((dir = opendir(path)) == NULL)
        {
            printf("cannot open directory: %s\n", path);
            return -2;
        }

        chdir(path);

        for(int i = 0; i < _countFiles; i ++)
        {
            //Считать длину названия
            short _sizeNameFile = 0;
            read(_fileInputFileDes, &_sizeNameFile, sizeof(_sizeNameFile));

            //Считать имя файла
            char _nameFile[1024] = {'\0'};
            read(_fileInputFileDes, _nameFile, _sizeNameFile);

            //Считать модификатор доступа файла
            __mode_t _modeFile;
            read(_fileInputFileDes, &_modeFile, sizeof(__mode_t));

            //Считать размер файла
            long int _sizeFile = 0;
            read(_fileInputFileDes, &_sizeFile, sizeof(_sizeFile));

            int fileOutDes = 0;
            if((fileOutDes = open(_nameFile, O_WRONLY|O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO | S_IROTH | S_IXOTH)) < 0)
            {
                printf("cannot open file: %s\n", _nameFile);
                return -3;
            }

            //Чтение данных из архива и запись их в файл
            char bufSymb[4096];
            int count = 0;

            while(count != _sizeFile)
            {
                int nread = read(_fileInputFileDes, &bufSymb, std::min(_sizeFile - count, (long int)4096));
                write(fileOutDes, &bufSymb, nread);
                count += nread;
            }

            close(fileOutDes);
        }

        closedir(dir);
        chdir(pathOutDir);


    }

    close(_fileInputFileDes);
    return 0;
}

int Unarchivator::extractView(char* pathInFile, char* pathOutDir)
{
    int _fileInputFileDes = 0;

    if((_fileInputFileDes = open(pathInFile, O_RDONLY)) < 0)
    {
        printf("cannot open file: %s\n", pathInFile);
        abort -1;
    }

    char buf = 0;
    int nread = 0;


    while((nread = read(_fileInputFileDes, &buf, 1)) != 0)
    {
        lseek(_fileInputFileDes, -1 , SEEK_CUR);

        DIR* dir;
        struct dirent* entry;
        struct stat statbuf;
        long int headDir;


        //Считать sizeof(short) байт - размер названия
        short _sizeNameDir = 0;
        read(_fileInputFileDes, &_sizeNameDir, sizeof(short));

        //Считать имя директории
        char _nameDir[1024] = {'\0'};
        read(_fileInputFileDes, _nameDir, _sizeNameDir);

        //Считать модификатор доступа файла
        __mode_t _mode;
        read(_fileInputFileDes, &_mode, sizeof(__mode_t));

        //Считать sizeof(long) байт - кол-во файлов в дирректории
        long _countFiles = 0;
        read(_fileInputFileDes, &_countFiles, sizeof(_countFiles));

        char path[1024] = {'\0'};
        strcat(path, pathOutDir);
        if(_sizeNameDir != 0)
        {
            strcat(path, "/");
            strcat(path, _nameDir);
        }

        mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO | S_IROTH | S_IXOTH);
        //mkdir(path, _mode);

        if((dir = opendir(path)) == NULL)
        {
            printf("cannot open directory: %s\n", path);
            return -2;
        }

        chdir(path);
        printf("openend directory: %s\n", path);

        for(int i = 0; i < _countFiles; i ++)
        {
            //Считать длину названия
            short _sizeNameFile = 0;
            read(_fileInputFileDes, &_sizeNameFile, sizeof(_sizeNameFile));

            //Считать имя файла
            char _nameFile[1024] = {'\0'};
            read(_fileInputFileDes, _nameFile, _sizeNameFile);

            //Считать модификатор доступа файла
            __mode_t _modeFile;
            read(_fileInputFileDes, &_modeFile, sizeof(__mode_t));

            //Считать размер файла
            long int _sizeFile = 0;
            read(_fileInputFileDes, &_sizeFile, sizeof(_sizeFile));

            int fileOutDes = 0;
            if((fileOutDes = open(_nameFile, O_WRONLY|O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO | S_IROTH | S_IXOTH)) < 0)
            {
                printf("cannot open file: %s\n", _nameFile);
                return -3;
            }

            //Чтение данных из архива и запись их в файл
            char bufSymb[4096];
            int count = 0;

            while(count != _sizeFile)
            {
                int nread = read(_fileInputFileDes, &bufSymb, std::min(_sizeFile - count, (long int)4096));
                write(fileOutDes, &bufSymb, nread);
                count += nread;
            }

            close(fileOutDes);
            printf("    written file %s %s%i %s\n", _nameFile, "size: ", _sizeFile, "bytes");
        }

        closedir(dir);
        chdir(pathOutDir);


    }

    close(_fileInputFileDes);
    return 0;
}

bool Unarchivator::rightExtencion(char* extention)
{
    int len = strlen(extention);
    char result[32] = {'\0'};
    char symb;
    int i = len-1;
    int k = 0;
    while(extention[i] != '.' && k < sizeof(result))
    {
        result[k] = extention[i];
        k++;
        i--;
    }

    if(strcmp(result, "iam") == 0)
        return true;
    return false;
}

int Unarchivator::Do(char* pathInFile, char* pathOutDir, char mode)
{
    if(rightExtencion(pathInFile))
    {
        if(mode == MODE_EXTRACT)
            extract(pathInFile, pathOutDir);
        if(mode == MODE_EXTRACT_VIEW)
            extractView(pathInFile, pathOutDir);
    }
    else
    {
        printf("not correct extention! Should be file of type *.mai");
        return -1;
    }



    return 0;
}
