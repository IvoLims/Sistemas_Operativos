#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */


/* 1. Implemente em C um programa mycp com funcionalidade similar ao comando cp. Varie o tamanho do
buffer usado e meca o tempo necessario para copiar um ficheiro de grande dimensão. */

int main(int argc, char *argv[]) {
    if (argc >= 3) {
        int fileToRead = open(argv[1], O_RDONLY);
        int fileToWrite = open(argv[2], O_CREAT | O_RDWR, 0600);
        ssize_t res = 1;
        char buffer[1];
    if(fileToRead == -1){
        perror("open");
        return -1;
    }
        while(res != 0) {
            res = read(fileToRead, &buffer, 1);
            if (res == 0) break;
            res = write(fileToWrite, &buffer, 1);
        }
        close(fileToWrite);
        close(fileToRead);
    }
    return 0;
}

/* 2. Implemente em C um programa mycat com funcionalidade similar ao comando cat, que lê do seu 
stdin e escreve para o seu stdout. */

#define MAX_BUFFER 10

int main(int argc, char *argv[]) {
    char buffer = MAX_BUFFER;
    ssize_t bitsLidos=0;
    bitsLidos = read(0, &buffer, MAX_BUFFER);
    while(bitsLidos != 0){
         write(STDOUT_FILENO,&buffer,bitsLidos);
    }
  return 0;
}

/* 3. Implemente a leitura de uma linha (i.e. sequência terminada por \n) numa função readln:

ssize_t readln(int fd, char *line, size_t size); */

ssize_t readln(int fd, char *line, size_t size){
        char c;
        ssize_t i=0;
        ssize_t res;
        if (fd == -1) {
        perror("Error opening file");
        return -1;
        }
        while(c != '\n'){
          res = read(fd,&c,1);
          if (res == 0) break;
          line[i++]+=c;
        }
        line[i] = '\0';
          return i;
}

int main(int argc, char *argv[]) {
    char line [100];
    size_t size;
    int file = open("Test.txt", O_RDONLY);
    size = readln(file, line, 100);
    write(1, &line, size);
    putchar('\n');
    close(file);
    return 0;
    }

/* 4. Melhore a implementação da função readln evitando ler um carater de cada vez. */

/* 5. Implemente, utilizando a função readln, um programa com funcionalidade similar ao comando nl,
que numera as linhas recebidas no seu standard input. Compare o desempenho deste programa com as
duas versões da função readln. */

/* 6. Considere uma estrutura (struct) com dados de uma pessoa (nome, idade, ...) e um ficheiro binario que 
contem registos organizados segundo esta estrutura. Não assuma que o ficheiro cabe todo em memoria. 
Escreva um programa que, consoante a opção, permita:

-i – Acrescentar pessoas a um ficheiro de dados (binario – a idade e um inteiro). 
-u – Actualizar a idade de uma determinada pessoa no ficheiro de dados.

Exemplos:
$ pessoas -i "José Mourinho" 55
$ pessoas -u "José Mourinho" 56

Meça o tempo que demora a inserir 1.000.000 de pessoas (nomes e idades gerados por si). Observa
alguma degradação de desempenho  a medida que o ficheiro cresce. */

/* 7. Faca com que a opção -i diga qual a posição no ficheiro do registo inserido e acrescente a possibilidade 
de actualizar a idade de registos por essa posição. 

Exemplo:
$ pessoas -i "José Mourinho" 55
registo 973
$ pessoas -u 973 56  */

/* 8. Meca o tempo que demora a alterar idades usando ambos os metodos. Observa alguma degradação de 
desempenho a medida que o ficheiro cresce. */
