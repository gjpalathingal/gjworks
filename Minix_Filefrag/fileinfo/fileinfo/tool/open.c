#include <stdio.h>
#include <unistd.h>

int main()
{
    FILE *fp = fopen("/home/rekha/project_3/b1", "a");
    sleep(1000);
    fclose(fp);
}
