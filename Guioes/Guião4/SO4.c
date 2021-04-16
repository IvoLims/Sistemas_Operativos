#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */

/* 1. Escreva um programa que redireccione o descritor associado ao seu standard input para o ficheiro
/etc/passwd, e o standard output e error respectivamente para saida.txt e erros.txt. */

//Simplesmente redireccione o descritor associado
int main(int argc,char* argv[]){
    int fdp = open("/etc/passwd",O_RDONLY);
    int fds = open("saida.txt",O_CREAT | O_TRUNC | O_WRONLY,0644);
    int fde = open("erros.txt",O_CREAT | O_TRUNC | O_WRONLY,0644);
    //stdin -> "/etc/passwd"
    dup2(fdp, 0);
    close(fdp);
    //stdout -> "saida.txt"
    dup2(fds, 1);
    close(fds);
    //stderr -> "erros.txt"
    dup2(fde, 2);
    close(fde);
    return 0;
}

//Redireccionar o descritor associado e escrever nos ficheiros embora escreva o mesmo em ambos
int main(int argc,char* argv[]){
    int fdp = open("/etc/passwd",O_RDONLY);
    int fds = open("saida.txt",O_CREAT | O_TRUNC | O_WRONLY,0644);
    int fde = open("erros.txt",O_CREAT | O_TRUNC | O_WRONLY,0644);
    //stdin -> "/etc/passwd"
    dup2(fdp, 0);
    close(fdp);
    //stdout -> "saida.txt"
    dup2(fds, 1);
    close(fds);
    //stderr -> "erros.txt"
    dup2(fde, 2);
    close(fde);
    char* c;
    while(read(0,&c,1) > 0){
          //saida.txt
          write(1, &c, 1);
          //error.txt
          write(2, &c, 1);
    }
    return 0;
}

/* 2. Modifique o programa anterior de modo a que, depois de realizar os redireccionamentos, seja criado um
novo processo que realize operações de leitura e escrita. Observe o conteúdo dos ficheiros. Repare que o
processo filho "nasce" com as mesmas associações de descritores de ficheiros do processo pai. */

int main(int argc,char* argv[]){
    int fdp = open("/etc/passwd",O_RDONLY);
    int fds = open("saida.txt",O_CREAT | O_TRUNC | O_WRONLY,0644);
    int fde = open("erros.txt",O_CREAT | O_TRUNC | O_WRONLY,0644);
    //stdin -> "/etc/passwd"
    dup2(fdp, 0);
    close(fdp);
    //stdout -> "saida.txt"
    dup2(fds, 1);
    close(fds);
    //stderr -> "erros.txt"
    dup2(fde, 2);
    close(fde);
    char* c;
    if(fork() == 0){
       while(read(0,&c,1) > 0){
             //saida.txt
             write(1, &c, 1);
             //error.txt
             write(2, &c, 1);
       }
       _exit(0);
    } else {
            wait(NULL);
            /*or
            int status;
            wait(&status);*/
    }
    return 0;
}
