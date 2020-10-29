#include"Archivator.h"
#include"Modes.h"

Archivator::Archivator()
{

}

Archivator::~Archivator()
{

}


FileDM::FileDM(struct DirDM& _root, char* _name, long int _size, __mode_t _mode)
{
    //mode = S_IRWXU | S_IRWXG | S_IRWXO | S_IROTH | S_IXOTH;
    mode = _mode;
    strcpy(name,_name);
    size = _size;
    root = &_root;

}

void FileDM::serialize(int fileDes)
{
    //Длина названия
    short _sizeName = strlen(name);
    write(fileDes, &_sizeName, sizeof(short));

    //Имя файла
    write(fileDes, name, strlen(name));

    //Модификатор доступа файла
    write(fileDes, &mode, sizeof(__mode_t));

    //Размер файла
    write(fileDes, &size, sizeof(long int));

    //Записать данные файла fileInputDes
    int fileInputDes = 0;
    char buff[1024] = {'\0'};
    strcat(buff, root->name);
    strcat(buff, "/");
    strcat(buff, name);

    if((fileInputDes = open(buff, O_RDONLY)) < 0)
    {
        printf("cannot open file: %s\n", buff);
        abort -1;
    }

    char bufread[1024] = {'\0'};
    long int nread = 0;
    long int allread = 0;
    while((nread = read(fileInputDes, bufread, sizeof(buff))) != 0)
    {
        allread += nread;
        write(fileDes, bufread, nread);
    }

    if(allread != size)
    {
        printf("file has not correct size: %s\n", buff);
        abort -2;
    }

    close(fileInputDes);
}


DirDM::DirDM(char* _name, char* _localname , __mode_t _mode)
{
    //mode = S_IRWXU | S_IRWXG | S_IRWXO | S_IROTH | S_IXOTH;
    mode = _mode;
    strcpy(name,_name);
    strcpy(localname,_localname);
    listFiles = new std::list<FileDM>();
    countFiles = 0;
}

void DirDM::serialize(int fileDes)
{
    //Записать sizeof(short) байт - размер названия
    short _sizeName = strlen(localname);
    write(fileDes, &_sizeName, sizeof(short));

    //Записать имя директории
    write(fileDes, localname, _sizeName);

    //Модификатор доступа директории
    write(fileDes, &mode, sizeof(__mode_t));

    //Записать sizeof(long) байт - кол-во файлов в дирректории
    long _countFiles = countFiles;
    write(fileDes, &_countFiles, sizeof(long));
}

int Archivator::archivate(char* pathInput, char* pathOutput)
{
    int fileOutDes = 0;

    if((fileOutDes = open(pathOutput, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO | S_IROTH | S_IXOTH)) < 0)
    {
        printf("cannot open file: %s\n", pathOutput);
        return -1;

    }

    std::list<DirDM> listDirectories;
    getFilesData(pathInput, "", &listDirectories);

    for(auto it = listDirectories.begin(); it != listDirectories.end(); it++)
    {
        it->serialize(fileOutDes);
        for(auto jt = it->listFiles->begin() ; jt != it->listFiles->end(); jt++)
            jt->serialize(fileOutDes);
    }

    close(fileOutDes);

    return 0;

}

int Archivator::archivateView(char* pathInput, char* pathOutput)
{
    int fileOutDes = 0;

    if((fileOutDes = open(pathOutput, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO | S_IROTH | S_IXOTH)) < 0)
    {
        printf("cannot open file: %s\n", pathOutput);
        return -1;

    }

    std::list<DirDM> listDirectories;
    getFilesData(pathInput, "", &listDirectories);

    for(auto it = listDirectories.begin(); it != listDirectories.end(); it++)
    {
        it->serialize(fileOutDes);
        printf("written data of directory: %s\n", it->name);
        for(auto jt = it->listFiles->begin() ; jt != it->listFiles->end(); jt++)
        {
            jt->serialize(fileOutDes);
            printf("    archivated file: %s\n", jt->name);
        }

    }

    close(fileOutDes);

    return 0;
}



int Archivator::Do(char* pathInput, char* pathOutput, char mode)
{
    if(mode == MODE_CREAT)
        archivate(pathInput, pathOutput);
    if(mode == MODE_CREAT_VIEW)
        archivateView(pathInput, pathOutput);

    return 0;
}


void Archivator::addFile(std::list<DirDM>* list, char* directoryName, char* fileName, long int size, __mode_t _mode)
{
    for(auto it = list->begin(); it != list->end(); it++)
    {
        if(strcmp(it->localname, directoryName) == 0)
        {
            it->listFiles->push_back(FileDM(*it, fileName, size, _mode));
            it->countFiles++;
            break;
        }
    }

}

std::list<DirDM>* Archivator::getFilesData(char* pathDirectory, char* localPath ,std::list<DirDM>* listDirectories)
{
    DIR* dir;
    struct dirent* entry;
    struct stat statbuf;
    long int headDir;

    if((dir = opendir(pathDirectory)) == NULL)
    {
        printf("cannot open directory: %s\n", pathDirectory);
        return NULL;
    }

    chdir(pathDirectory);
    headDir = telldir(dir);

    while((entry = readdir(dir)) != NULL)
    {
        lstat(entry->d_name, &statbuf);
        if(!S_ISDIR(statbuf.st_mode))
        {
            if(listDirectories->empty())
                listDirectories->push_back(DirDM(pathDirectory,localPath, statbuf.st_mode));
            addFile(listDirectories, localPath, entry->d_name, statbuf.st_size, statbuf.st_mode);

        }
    }

    seekdir(dir,headDir);

    while((entry = readdir(dir)) != NULL)
    {
        lstat(entry->d_name, &statbuf);
        if(S_ISDIR(statbuf.st_mode))
        {
            if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0 )
                continue;

            char path[1024] = {'\0'};
            strcat(path, pathDirectory);
            strcat(path, "/");
            strcat(path, entry->d_name);


            char _localpath[1024] = {'\0'};
            strcat(_localpath, localPath);
            strcat(_localpath, "/");
            strcat(_localpath, entry->d_name);

            listDirectories->push_back(DirDM(path, _localpath, statbuf.st_mode));
            getFilesData(path, _localpath, listDirectories);
        }

    }


    chdir("..");
    closedir(dir);
    return listDirectories;

}



