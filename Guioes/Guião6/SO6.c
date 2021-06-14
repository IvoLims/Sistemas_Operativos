#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

//1.

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

//2

// Tem de entrar um cliente. O terminal de escrita nunca fecha;

#define MAXBUFFER 1024

int main(int argc,char* argv[]){
  char buf[MAXBUFFER];
  int bytes_read;
  int logfile,fd,fd_write;
  if((fd = open("fifo",O_RDONLY)) ==-1){
      perror("Couldn't open");
      return -1;
  } else{
    printf("Opened fifo for read\n");
  }
  if((fd_write = open("fifo",O_WRONLY)) == -1){
      perror("Couldn't open");
      return -1;
  } else {
    printf("Opened for write\n");
  }
  while((bytes_read == read(fd,buf,MAXBUFFER)) > 0){
         write(logfile,buf,bytes_read);
         printf("Wrote %s to file\n",buf);
  }
  if(bytes_read == 0){
     printf("EOF\n");
  } else{
    perror("Couldn't read\n");
  }
  close(logfile);
  close(fd);
  close(fd_write);
  return 0;
}

//  Bloqueia no 1º open se nenhum cliente escrever fica lá preso; Se o cliente escrever fica preso no ciclo se leitura. Depois de escrever volta ao início e fica a aguardar novas intruções.

int main(int argc,char* argv[]){
    char buf[MAXBUFFER];
    int bytes_read;
    int logfile,fd,fd_write;
    if((logfile = open("log.txt",O_CREAT | O_TRUNC | O_WRONLY,0666)) == -1){
       perror("Couldn't open");
       return 1;
    }
    while(1){
          if((fd = open("fifo",O_RDONLY)) == -1){
              perror("Couldn't open");
              return 1;
          } else{
            printf("Open fifo for read\n");
          }
          while((bytes_read=read(fd,buf,MAXBUFFER)) > 0){
                write(logfile,buf,bytes_read);
                printf("Wrote %s to file\n",buf);
          }
          if(bytes_read == 0){
             printf("EOF");
          } else{
            perror("Couldn't read");
            }
    close(fd);
    }
    close(logfile);
    close(fd_write);
    return 0;
}
