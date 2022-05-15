#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */

/* 1. Escreva um programa que crie um pipe anónimo e de seguida crie um processo filho. Relembre que as
associações dos descritores de ficheiros são preservadas na criação do processo filho. Experimente o pai
enviar uma linha de texto através do descritor de escrita do pipe, e o filho receber uma linha a partir do
respectivo descritor de leitura. Note que a informação foi transferida do pai para o filho. Experimente
de seguida provocar um atraso antes do pai enviar a linha de texto (p. ex., sleep(5)). Note agora que
a leitura do filho bloqueia enquanto o pai não realizar a operacão de escrita no pipe. Experimente agora
inverter os papeis de modo a informação ser transmitida do filho para o pai. */
  
int pai2filho(){
  int p[2];
  char line[] = "aqui\n";
  char buffer[20];
  int res;
  int statues;
  if (pipe(p) != 0){
    perror("Error");
    return -1;
  }
  switch(fork()){
    case -1:{
      perror("Fork");
      return -1;
    }
    case 0:{ //Filho
      //fechar filho
      close(p[1]);
      res = read(p[0],&buffer,sizeof(buffer));
      printf("Filho leu: %s",buffer);
      close(p[0]);
      _exit(0);
    }
    default:{ //Pai
      //fechar pai
      close(p[0]);
      //sleep(5);
      write(p[1], &line, sizeof(line));
      printf("Pai escreveu no pipe\n");
      close(p[1]);
      wait(&statues);
    }
  }
  return 0;
}

int filho2pai(){
  int p[2];
  char line[] = "aqui\n";
  char buffer[20];
  int res;
  int statues;
  if (pipe(p) != 0){
    perror("Error");
    return -1;
  }
  switch(fork()){
    case -1:{
      perror("Fork");
      return -1;
    }
    case 0:{ //Filho
      //fechar filho
      close(p[0]);
      write(p[1], &line, sizeof(line));
      printf("Filho escreveu para pipe\n");
      close(p[1]);
      _exit(0);
    }
    default:{ //Pai
      //fechar pai
      close(p[1]);
      res = read(p[0],&buffer,sizeof(buffer));
      printf("Pai leu: %s",buffer);
      close(p[0]);
      wait(&statues);
    }
  }
  return 0;
}

int main(int argc,char* argv[]){
    pai2filho();
    filho2pai()
    return 0;
}

/*2. Modifique o programa anterior de modo a leitura do pipe ser realizada enquanto não for detectada a
situação de end of file no descritor respectivo. Repare que esta situação acontece apenas quando nenhum
processo – neste caso, pai e filho – tem aberto o descritor de escrita do pipe.*/

int pai2filhoV2(){
  int p[2]; //p[0] -> ler; p[1] -> escrever;
  char line[] = "aqui\n";
  char buffer[20];
  int res;
  int statues;
  if (pipe(p) != 0){
    perror("Error");
    return -1;
  }
  switch(fork()){
    case -1:{
      perror("Fork");
      return -1;
    }
    case 0:{ //Filho
      //fechar filho
      close(p[1]);
      while(read(p[0],&buffer,1) > 0){
            printf("Filho leu: %s\n",buffer);
      }
      printf("Filho leu: %s",buffer);
      close(p[0]);
      _exit(0);
    }
    default:{ //Pai
      //fechar pai
      close(p[0]);
      //sleep(5);
      for(int i = 0;i < 1;i++){
          write(p[1], &line, sizeof(line));
          printf("Pai escreveu no pipe\n");
      }
      close(p[1]);
      wait(&statues);
    }
  }
  return 0;
}

int main(int argc,char* argv[]){
    pai2filhoV2();
    return 0;
}

/*3. Escreva um programa que execute o comando wc num processo filho. O processo pai deve enviar ao
filho através de um pipe anónimo uma sequência de linhas de texto introduzidas pelo utilizador no seu
standard input. Recorra à técnica de redireccionamento estudada no guião anterior de modo a associar
o standard input do processo filho ao descritor de leitura do pipe anónimo criado pelo pai. Recorde a
necessidade de fechar o(s) descritor(es) de escrita no pipe de modo a verificar-se a situação de end of file.*/

ssize_t readln(int fd, char *line, size_t size){
        int next_pos=0;
        int read_bytes =0;
        while(next_pos < size && read(fd,line+next_pos,1)>0){
          read_bytes++;
          if(line[next_pos] == '\n'){
            break;
          }
          next_pos++;
        }
        return read_bytes;
}

int main(int argc,char* argv[]){
    int res; 
    int p[2];
    int pid;
    char buffer[20];
    if (pipe(p) != 0){
        perror("Error");
        return -1;
    }switch(pid=fork()){
            case -1:{
                     perror("Fork");
                     return -1;
            }
            case 0:{
                    close(p[1]);
                    dup2(p[0],0);
                    close(p[0]);
                    res = execlp("wc","wc",NULL);
                    _exit(1);

            }
            default:{
                     int status;
                     close(p[0]);
                     printf("Pai %d, Filho %d",getpid(),pid);
                     while((res = readln(0,buffer,sizeof(buffer))) > 0){
                        write(p[1], buffer, res);
                     }
                     close(p[1]);
                     wait(&status);
                     if(WIFEXITED(status)){
                        printf("Pai terminou com codigo %d",WEXITSTATUS(status));
                     }
            }
  }
    return 0;
}

/*4. Escreva um programa que emule o funcionamento do interpretador de comandos na execução encadeada
de ls /etc | wc -l.*/

int main(int argc,char* argv[]){
    int p[2];
    int status[2];
    if (pipe(p) != 0){
        perror("Error");
        return -1;
    }switch(fork()){
            case -1:{
                     perror("Fork");
                     return -1;
            }
            case 0:{
                    close(p[0]);
                    dup2(p[1],1);
                    close(p[1]);
                    execlp("ls", "ls", "/etc",NULL);
                    _exit(1);

            }
            default:{
                     close(p[1]);
                     dup2(p[0],0);
                     close(p[0]);
                     execlp("wc", "wc", "-l", NULL);
            }
  }
    return 0;
}

int exec_command(char* arg){
    char* exec_args[10];
    char* string;
    int exec_ret = 0;
    int i = 0;
    char* command = strdup(arg);
    string = strtok(command," ");
    while(string != NULL){
          exec_args[1] = string;
          string = strtok(NULL," ");
          i++;
    }
    exec_args[i] = NULL;
    exec_ret = execvp(exec_args[0], exec_args);
    return exec_ret;
}

/*5. Escreva um programa que emule o funcionamento do interpretador de comandos na execução encadeada
de grep -v ˆ# /etc/passwd | cut -f7 -d: | uniq | wc -l. */

int main(int argc,char* argv[]){
    char* commands[] = {"grep -v ˆ# /etc/passwd" , "cut -f7 -d:" , "uniq" , "wc -l"};
    int ncommands = 4;
    int pipes[ncommands-1][2];
    int status[ncommands];
    for(int i = 0; i < ncommands; i++){
        if (i == 0){
            if (pipe(pipes[i]) != 0){
                perror("Error");
                return -1;
            }switch(fork()){
                    case -1:{
                      perror("fork");
                      return -1;
                    }
                    case 0:{
                            close(pipes[i][0]);
                            dup2(pipes[i][1],1);
                            close(pipes[i][1]);
                            exec_command(commands[i]);
                            _exit(0);
                    }
                    default:{
                             close(pipes[i][1]);
                    }
             }
        }else if(i == ncommands -1){
                 switch(fork()){
                    case -1:{
                      perror("fork");
                      return -1;
                    }
                    case 0:{
                            dup2(pipes[i-1][0],0);
                            close(pipes[i-1][0]);
                            exec_command(commands[i]);
                            _exit(0);
                    }
                    default:{
                             close(pipes[i][1]);
                    }
                 }
        }else{
              if (pipe(pipes[i]) != 0){
                  perror("Error");
                  return -1;
              }switch(fork()){
                    case -1:{
                      perror("fork");
                      return -1;
                    }
                    case 0:{
                            close(pipes[i][0]);
                            dup2(pipes[i][1],1);
                            close(pipes[i][1]);
                            dup2(pipes[i-1][0],0);
                            close(pipes[i-1][0]);
                            exec_command(commands[i]);
                            _exit(0);
                    }
                    default:{
                             close(pipes[i][1]);
                             close(pipes[i-1][0]);
                    }

              }
        }
    }for(int j = 0; j<ncommands ;j++){
      wait(&status[j]);
    }
    return 0;
}
