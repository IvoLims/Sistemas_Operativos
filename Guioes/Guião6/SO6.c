#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char* argv[]){
    if(mkfifo("fifo",0666) == -1){
       perror("mkfifo");
    }
unlink("fifo");
return 0;
}

int main(int argc, char* argv[]){
    int fd;
    int bytes_read;
    char buffer[10];
    if((fd = open("fifo",O_RDONLY)) == -1){
       perror("Can't open");
       return -1;
    }
    while((bytes_read = read(fd,&buffer,10)) > 0){
           write(STDOUT_FILENO, &buffer, bytes_read);
    }
    unlink("fifo");
    close(fd);
return 0;
}

int main(int argc, char* argv[]){
    int fd;
    int bytes_read;
    char buffer[10];
    fd = open("fifo",O_WRONLY);
    while((bytes_read = read(STDIN_FILENO,&buffer,10)) > 0){
           write(fd, &buffer, bytes_read);
    }
    unlink("fifo");
    close(fd);
return 0;
}
