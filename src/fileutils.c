#include <stdio.h>
#include <stdlib.h>
#include "../include/c-copy-headers.h"
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

void ExploreDir(const char *src_path, const char *dest_path, JobQueue *jq_ptr)
{
    DIR *dir_ptr = opendir(src_path);
    if (dir_ptr == NULL)
    {
        return;
    }
    struct dirent *entry = NULL;

    while ((entry = readdir(dir_ptr)) != NULL)
    {
        const bool is_subdir = (entry->d_type == 4 && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0);
        const bool is_file = (entry->d_type == 8);

        if (strlen(src_path) + 1 + strlen(entry->d_name) > PATH_MAX_LEN)
        {
			if (is_subdir)
        	{
				dirs_failed++;
			}
			else if (is_file)
			{
				files_failed++;
			}
            continue;
        }

        char src_path_full[PATH_MAX_LEN] = "";
        char dest_path_full[PATH_MAX_LEN] = "";

        // path construction approach assisted by ai
        snprintf(src_path_full, PATH_MAX_LEN, "%s/%s", src_path, entry->d_name);
        snprintf(dest_path_full, PATH_MAX_LEN, "%s/%s", dest_path, entry->d_name);

        if (is_subdir) // recurse
        {
            mkdir(dest_path_full, 0755);
            ExploreDir(src_path_full, dest_path_full, jq_ptr);
        }
        else if (is_file) // create copy job
        {
            struct stat st;
            stat(src_path_full, &st);
            bytes_total += st.st_size;
            if (!CreateJob(src_path_full, dest_path_full, st.st_size, jq_ptr))
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

    FILE *src_file = fopen(job_ptr->src_path, "rb");
    if (!src_file)
    {
        return -1;
    }

    FILE *dst_file = fopen(job_ptr->dest_path, "wb");
    if (!dst_file) 
    {
        fclose(src_file);
        return -1;
    }

    char buffer[8192];
    size_t bytes_read;
    int bytes_total = 0;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src_file)) > 0) 
    {
        size_t bytes_written = fwrite(buffer, 1, bytes_read, dst_file);
        if (bytes_written != bytes_read) 
        {
            fclose(src_file);
            fclose(dst_file);
            return -1;
        }
        bytes_total += (int) bytes_written;
    }

    if (ferror(src_file)) 
    {
        fclose(src_file);
        fclose(dst_file);
        return -1;
    }

    fclose(src_file);
    fclose(dst_file);

    job_ptr->file_size = bytes_total;
    return 0;
}