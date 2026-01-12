#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>


#define PATH_MAX_LEN 4096


void CreateJob();	//declared for compilation

void ExploreDir(char *srcPath, char *destPath, Queue *q)
{
    DIR *dir;
    struct dirent *entry;
    struct stat st;

    char srcFullPath[PATH_MAX_LEN];
    char destFullPath[PATH_MAX_LEN];

    dir = opendir(srcPath);
    if(!dir){return;}		//could return -1

    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){continue;} //skip for obvious reasons

        snprintf(srcFullPath, PATH_MAX_LEN, "%s/%s", srcPath, entry->d_name);	//path construction
        snprintf(destFullPath, PATH_MAX_LEN, "%s/%s", destPath, entry->d_name);	//assisted by ai



        if(stat(srcFullPath, &st) == -1){continue;}	//if file doesnt exist (anymore?), skip it.


        if(S_ISREG(st.st_mode)) 
	{
            CreateJob();
        }
        else if(S_ISDIR(st.st_mode)) 
	{
            mkdir(destFullPath, 0755);			//create dir in destination
            ExploreDir(srcFullPath, destFullPath, q);	//copy dir's contents into destination/dir
        }
    }

    closedir(dir);
}
