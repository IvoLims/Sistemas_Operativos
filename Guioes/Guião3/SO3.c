#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/* 1. Implemente um programa que execute o comando ls -l. Note que no caso da execução ser bem
sucedida, mais nenhuma outra instrução é executada do programa original. */

int main(int argc,char* argv[]){
    
    //Poderei utilizar um ou outro
    
    //execl's
    execl("/bin/ls", "ls","-l",NULL); //ou execl("/bin/ls","/bin/ls","-l",NULL);
    execlp("ls","ls","-l",NULL)
    
    //execv's (vetores)
    char* comandos[]={"/bin/ls","-l",NULL};
    
    execv("/bin/ls",comandos);
    execvp("ls",comandos);
    
    //Nada daqui para baixo será printado mas pronto, é só para ilustrar melhor
    perror("Error:");
    printf("Exec\n");
    return 0;
}

/* 2. Implemente um programa semelhante ao anterior que execute o mesmo comando mas agora no contexto
de um processo filho. */

int main(int argc,char* argv[]){
    printf("Antes do Exec.\n");
    if(fork()==0){
      //Só a imagem do filho será substituída
      execl("/bin/ls", "ls","-l",NULL);
      perror("Exec");
      _exit(1);
    }
    printf("Filho criado com Exec\n");
    int status;
    pid_t terminated_pid=wait(&status);
    printf("Filho %d terminou.\n",terminated_pid);
    return 0;
}

// 3. Implemente um programa que imprima a lista de argumentos recebidos na sua linha de comando.

int main(int argc,char* argv[]){
    for(int i=0;argv[i];i++){
        printf("argv[%d]: %s;\n",i,argv[i]);
    }
    return 0;
}

/* 4. Implemente um programa que execute o programa da questao anterior com uma qualquer lista de argumentos. 
Mantendo o nome do ficheiro que corresponde o programa executável, experimente alterar o primeiro elemento 
da lista de argumentos (índice zero do argv). */

int main(int argc,char* argv[]){
    char* comandos[] = {"cenaQualquer","comando1","comando2",NULL};
    execv("ex3_printArgs",comandos);
    return 0;
}

//Nota: Funciona tudo bem embora seja convençam passar o nome do programa no argv[0].

/* 5. Implemente um programa que execute concorrentemente uma lista de executaveis especificados como
argumentos da linha de comando. Considere os executaveis sem quaisquer argumentos próprios. O
programa deverá esperar pelo fim da execuçáo de todos processos por si criados. */

int main(int argc,char* argv[]){
    for(int i=0;i<argc;i++){
        if(fork()==0){
           execlp(argv[0],argv[1],NULL); //execlp("ls","ls",NULL)
           _exit(i);
        }
    }
    for(int j=0;j<argc;j++){
        int status;
        wait(&status);
    }
    return 0;
}

/* 6. Implemente uma versão simplificada da função system(). Ao contrário da função original, não tente
suportar qualquer tipo de redireccionamento, ou composição/encadeamento de programas executáveis.
O único argumento deverá ser uma string que específica um programa executável e uma eventual lista de
argumentos. Procure que o comportamento e valor de retorno da sua funçao sejam compatíveis com a
original. */
