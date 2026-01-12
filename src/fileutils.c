#include <stdio.h>
#include <stdlib.h>
#include <c-copy-headers.h>

int CopyFile(CopyJob *job)
{
    //consulted with an ai to determine failiure points and add code to check for them.
    
    if(job == NULL){return -1;}

    FILE *src = fopen(job->srcPath, "rb");
    if(!src){return -1;}

    FILE *dst = fopen(job->destPath, "wb");
    if (!dst) 
    {
        fclose(src);
        return -1;
    }

    char buffer[8192];
    size_t bytesRead;
    int totalBytes = 0;

    while((bytesRead = fread(buffer, 1, sizeof(buffer), src)) > 0) 
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

    if(ferror(src)) 
    {
        fclose(src);
        fclose(dst);
        return -1;
    }

    fclose(src);
    fclose(dst);

    job->fileSize = totalBytes;
    return 0;
}
