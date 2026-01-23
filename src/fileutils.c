#include <stdio.h>
#include <stdlib.h>
#include "../include/c-copy-headers.h"
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

void ExploreDir(const char *srcPath, const char *destPath, JobQueue *jq_ptr)
{
    DIR *dir_ptr = opendir(srcPath);
    if (dir_ptr == NULL)
    {
        return;
    }
    struct dirent *entry = NULL;

    while ((entry = readdir(dir_ptr)) != NULL)
    {
        const bool isSubdir = (entry->d_type == 4 && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0);
        const bool isFile = (entry->d_type == 8);

        if (strlen(srcPath) + 1 + strlen(entry->d_name) > PATH_MAX_LEN)
        {
			if (isSubdir)
        	{
				dirs_failed++;
			}
			else if (isFile)
			{
				files_failed++;
			}
            continue;
        }

        char srcFullPath[PATH_MAX_LEN] = "";
        char destFullPath[PATH_MAX_LEN] = "";

        // path construction approach assisted by ai
        snprintf(srcFullPath, PATH_MAX_LEN, "%s/%s", srcPath, entry->d_name);
        snprintf(destFullPath, PATH_MAX_LEN, "%s/%s", destPath, entry->d_name);

        if (isSubdir) // recurse
        {
            mkdir(destFullPath, 0755);
            ExploreDir(srcFullPath, destFullPath, jq_ptr);
        }
        else if (isFile) // create copy job
        {
            struct stat st;
            stat(srcFullPath, &st);
            bytes_total += st.st_size;
            if (!CreateJob(srcFullPath, destFullPath, st.st_size, jq_ptr))
            {
                bytes_failed += st.st_size;
				files_failed++;
				free(jq_ptr);
            }
        }
    }

    closedir(dir_ptr);
}

int CopyFile(CopyJob *job_ptr)
{
    //consulted with an ai to determine failure points and add code to check for them.
    
    if (job_ptr == NULL)
    {
        return -1;
    }

    FILE *src = fopen(job_ptr->srcPath, "rb");
    if (!src)
    {
        return -1;
    }

    FILE *dst = fopen(job_ptr->destPath, "wb");
    if (!dst) 
    {
        fclose(src);
        return -1;
    }

    char buffer[8192];
    size_t bytesRead;
    int totalBytes = 0;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), src)) > 0) 
    {
        size_t bytesWritten = fwrite(buffer, 1, bytesRead, dst);
        if (bytesWritten != bytesRead) 
        {
            fclose(src);
            fclose(dst);
            return -1;
        }
        totalBytes += (int) bytesWritten;
    }

    if (ferror(src)) 
    {
        fclose(src);
        fclose(dst);
        return -1;
    }

    fclose(src);
    fclose(dst);

    job_ptr->fileSize = totalBytes;
    return 0;
}
