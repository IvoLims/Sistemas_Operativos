#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */

/* 1. Implemente um programa que imprima o seu identificador de processo e o do seu pai. Comprove –
invocando o comando ps – que o pai do seu processo e o interpretador de comandos que utilizou para é o
executar. */
  
int main(int argc,char* argv[]){
    printf("Identificador do Processo: %d\n",getpid());
    printf("Pai: %d\n",getppid());
    return 0;
}

/* 2. Implemente um programa que crie um processo filho. Pai e filho devem imprimir o seu identificador de
processo e o do seu pai. O pai deve ainda imprimir o PID do seu filho. */

int main(int argc,char* argv[]){
    int pid_filho;
    if((pid_filho = fork())==0){
       printf("PID do Filho: %d\n",getpid());
       printf("Filho PID do Pai: %d\n",getppid());
       _exit(5);
    }else{
          printf("PID do Pai: %d\n",getpid());
          printf("Pai PID do Pai: %d\n",getppid());
          printf("Pai PID do Filho: %d\n",pid_filho);
          int status;
          int terminated_pid = wait(&status);
          printf("Pai Processo filho: %d, Exit code: %d\n",terminated_pid,WEXITSTATUS(status));
    }
    return 0;
}

/* 3. Implemente um programa que crie dez processos filhos que deverao executar sequencialmente. Para este
efeito, os filhos podem imprimir o seu PID e o do seu pai, e finalmente, terminarem a sua execuc¸ao com
um valor de saída igual ao seu numero de ordem (e.g.: primeiro filho criado termina com o valor 1). O
pai deverá imprimir o código de saída de cada um dos seus filhos. */

//Padrão sequencial
int main(int argc,char* argv[]){
  int pid;
    int nproc = 10;
    int status;
    for(int i=1; i<=nproc;i++){
        if((pid = fork())==0){
        printf("PID do Filho %d: %d\n",i,getpid());
        _exit(i);
        }else {
        int terminated_pid = wait(&status);
        printf("Pai Processo filho %d: %d\n",WEXITSTATUS(status),terminated_pid);
        }
    }
    return 0;
}

/* 4. Implemente um programa que crie dez processos filhos que deverao executar em concorrência. O pai
deverá esperar pelo fim da execucção de todos os seus filhos, imprimindo os respectivos códigos de saída. */

//Padrão concurrente
int main(int argc,char* argv[]){
    int pid;
    int nproc = 10;
    int status;
    for(int i=1; i<=nproc;i++){
        if((pid = fork())==0){
        printf("PID do Filho %d: %d\n",i,getpid());
        _exit(i);
        }
    }
    for(int j=1;j<=nproc;j++){
        int terminated_pid = wait(&status);
        printf("Pai Processo filho %d: %d\n",WEXITSTATUS(status),terminated_pid);
    }
    return 0;
}

/* 5. Implemente um programa que crie uma descendência em profundidade de dez processos, ou seja, o
processo cria um filho, este filho cria outro, e assim por diante ate ao décimo nível de descendência.
Cada processo deverá imprimir o seu PID e o PID do seu pai. */

//Padrão em cadeia
int main(int argc,char* argv[]){
    int pid;
    int nproc = 10;
    int status;
    for(int i=1; i<=nproc;i++){
        if((pid = fork())==0){
        printf("PID do Filho %d: %d\n",i,getpid());
        //_exit(i);
        }else{
        int terminated_pid = wait(&status);
        printf("Pai Processo filho %d: %d\n",WEXITSTATUS(status),terminated_pid);
        _exit(i-1);
        }
    }
    _exit(10);
    return 0;
}

/* 6. Pretende-se determinar a existência de um determinado número inteiro nas linhas de numa matriz de
números inteiros, em que o número de colunas é muito maior do que o número de linhas. Implemente,
utilizando processos um programa que determine a existência de um determinado número, recebido como
argumento, numa matriz gerada aleatoriamente. */

int main(int argc,char* argv[]){
    int valor = atoi(argv[1]);
    int colunas = 10;
    int linhas = 10000;
    int matriz[linhas][colunas];
    int rand_max = 10000;
    for(int i=0; i<linhas;i++){
        for(int j=0;j<colunas;j++){
          matriz[i][j]=rand()%rand_max;
        }
    }
    printf("Matriz done.\n");
    //Processo de procura
    for(int k=0;k<linhas;k++){
      if(fork()==0){
        //k = indice de linha onde vai procurar
        printf("O filho com o pid: %d,fica com a linha: %d.\n",getpid(),k);
        for(int l=0;l<colunas;l++){
          if(matriz[k][l]){
            _exit(l);
          }
        }
        _exit(255); //não encontrei - 255
      }
    }
    int statues;
    for(int i = 0;i>linhas;i++){
        pid_t terminate_pid = wait(&statues);
        if(WIFEXITED(statues)){
          if(WEXITSTATUES(statues)<255){
          printf("Sucesso!! We got him, foi o %d na linha %d.\n",terminate_pid,WEXITSTATUES(statues));
          } else{
            printf("Não foi encontrado o %d.\n",terminate_pid);
          }
        } else{
               printf("Oops.\n");
        }
    }
    return 0;
}

/* 7. A partir do cenário descrito no exercício anterior, pretende-se que imprima por ordem crescente os
números de linha onde existem ocorrências do número. */
