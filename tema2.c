#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

void myFilesRecursive(char *sourcePath, char *destinationPath)
{
    char path1[1000], path2[1000];
    struct dirent *dp;
    DIR *dir = opendir(sourcePath);
    if (!dir) return;
    while((dp = readdir(dir)) != NULL)
    {
        // making the new path for source
        strcpy(path1, sourcePath);
        strcat(path1, "/");
        strcat(path1, dp->d_name);

        // making the new path for destination
        strcpy(path2, destinationPath);
        strcat(path2, "/");
        strcat(path2, dp->d_name);

        struct stat state;
        stat(path1, &state);
        if (S_ISDIR(state.st_mode)) // verify if the current path is directory
        {
            if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
            {
                printf("%s is directory. Rights: %o\n", path1, state.st_mode);
                mkdir(path2, state.st_mode);
                myFilesRecursive(path1, path2);
            }
        }
        else if (S_ISREG(state.st_mode)) // verify if the current path is a file
        {
            printf("%s is a file. Size: %jd   Rights: %o\n", path1, state.st_size, state.st_mode);
            if (state.st_size < 500) // if the file is smaller than 500 bytes
            {
                int readFile, writeFile, data;
                readFile = open(path1, O_RDONLY);
                if (readFile == -1) { printf("The source file could not be opened!\n"); break; }

                writeFile = creat(path2, state.st_mode);
                if (writeFile == -1) { printf("The file for write could not be opened!\n"); break; }

                char buffer[20];
                while(data = read(readFile, &buffer, 20)) // copying the actual data
                {
                    write(writeFile, &buffer, data);
                }

                close(writeFile);
                close(readFile);

                int rights = (state.st_mode & S_IRUSR) | (state.st_mode & S_IRGRP) | (state.st_mode & S_IROTH); // setting the special mode
                // example: -rwx-wxr-x  ---> -r-----r--
                chmod(path2, rights);

                struct stat state2;
                stat(path2, &state2);
                printf("\t%s is a file. Size: %jd   Rights: %o\n", path2, state2.st_size, state2.st_mode);

            }
            else 
            {
                link(path1, path2); // making a hardlink for files larger than 500 bytes
            }

        }
        else {
            printf("%s is not a directory/file. Rights: %o\n", path1, state.st_mode);
        }
    }
    closedir(dir);
}

int main(int argc, char** argv)
{
    DIR *dr;
    struct dirent *entryData;
    char cale[1000];

    printf("argv[0]   %s\n", argv[0]);
    printf("argv[1]   %s\n", argv[1]);
    printf("argv[2]   %s\n", argv[2]);

    struct stat state;
    struct stat state2;
    if (argc != 3) {
        printf("Only 2 arguments (source folder and destination)");
        return 1;
    }

    stat(argv[1], &state);
    if (!S_ISDIR(state.st_mode))
    {
        perror(argv[1]);
        return 1;
    }

    stat(argv[2], &state2);
    if (!S_ISDIR(state2.st_mode))
    {
        mkdir(argv[2], 0777);
        printf("Director creat!\n");
    }

    myFilesRecursive(argv[1], argv[2]);
}