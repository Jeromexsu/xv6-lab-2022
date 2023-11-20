#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

char* canonicalName(char *path) {
    static char buf[512], *p;
    for(p = path + strlen(path); p >= path; p--) {
        if(*p == '/') break;
    }
    p++;
    strcpy(buf,p);
    return buf;
}

void find(char *path, const char *name) {
    int fd;
    struct stat st;
    
    if((fd = open(path,O_RDONLY)) < 0) {
        fprintf(2,"find : open error\n");
        return;
    }

    if (fstat(fd,&st) < 0)
    {
        fprintf(2,"find: fstat error\n");
        close(fd);
        return;   
    }

    if(st.type == T_FILE) {
        if(!strcmp(name,canonicalName(path))) {
            printf("%s\n",path);
        }
    } else if (st.type == T_DIR) {
        char buf[512], *p;
        
        // copy path into buf, init pointer at the end of path
        strcpy(buf,path);
        p = buf + strlen(path);
        *p++ = '/';

        struct dirent de; // read dir entries
        while((read(fd, &de, sizeof de)) != 0) { 
            if(de.inum == 0) { // skip empty nodes
                continue;
            }    
            if(!strcmp(de.name, ".") || !strcmp(de.name, "..")) {
                continue;
            }

            strcpy(p,de.name); // concat file name to dir path
            p[strlen(de.name)] = 0;

            find(buf,name);
        }
    }
    
    close(fd);

}

int main(int argc, char *argv[]) {
    if(argc <= 1) {
        printf("find directory file\n");
    } else if(argc == 2) {
        find(".",argv[1]);
    } else {
        find(argv[1],argv[2]);
    }
    exit(0);
}