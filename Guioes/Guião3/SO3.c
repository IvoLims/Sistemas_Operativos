#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/* 1. Implemente um programa que execute o comando ls -l. Note que no caso da execução ser bem
sucedida, mais nenhuma outra instrução é executada do programa original. */

int main(int argc,char* argv[]){
    execl("/bin/ls", "ls","-l",NULL);
    //ou execl("/bin/ls","/bin/ls","-l",NULL);
    
    //Nada daqui para baixo será printado mas pronto, é só para ilustrar melhor
    perror("Exec");
    printf("Exec\n");
    return 0;
}

/* 2. Implemente um programa semelhante ao anterior que execute o mesmo comando mas agora no contexto
de um processo filho. */

// 3. Implemente um programa que imprima a lista de argumentos recebidos na sua linha de comando.

/* 4. Implemente um programa que execute o programa da questao anterior com uma qualquer lista de argumentos. 
Mantendo o nome do ficheiro que corresponde o programa executável, experimente alterar o primeiro elemento 
da lista de argumentos (índice zero do argv). */

/* 5. Implemente um programa que execute concorrentemente uma lista de executaveis especificados como
argumentos da linha de comando. Considere os executaveis sem quaisquer argumentos próprios. O
programa deverá esperar pelo fim da execuçáo de todos processos por si criados. */

/* 6. Implemente uma versao simplificada da função system(). Ao contrário da função original, não tente
suportar qualquer tipo de redireccionamento, ou composição/encadeamento de programas executáveis.
O único argumento deverá ser uma string que específica um programa executável e uma eventual lista de
argumentos. Procure que o comportamento e valor de retorno da sua funçao sejam compatíveis com a
original. */
