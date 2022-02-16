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
        perror("Couldn't open the file to read.");
        return -1;
    }
        while(res != 0) {
            res = read(fileToRead, &buffer, 1);
            if (res == 0) break;
            res = write(fileToWrite, &buffer, 1);
        }
        close(fileToWrite);
        close(fileToRead);
    } else{
        printf("Can't execute the command.\nMissing %d arguments.\n",(3 - argc));
        return -1;
    }
    printf("Sucess.\n");
    return 0;
}

/* 2. Implemente em C um programa mycat com funcionalidade similar ao comando cat, que lê do seu 
stdin e escreve para o seu stdout. */

#define MAX_BUFFER 10

int main(int argc, char *argv[]) {
    char buffer = MAX_BUFFER;
    ssize_t bitsLidos=0;
    bitsLidos = read(STDIN_FILENO, &buffer, MAX_BUFFER);
    while(bitsLidos != 0){
         write(STDOUT_FILENO,&buffer,bitsLidos);
    }
  return 0;
}

/*Note: STDIN_FILENO is 0, STDOUT_FILENO is 1 and STDERR_FILENO is 2.*/

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

//Or

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

int main(int argc, char *argv[]) {
    char buf[60]={};
    int fd = open("Test.txt", O_RDONLY);
    int line_bytes = 0;
    while((line_bytes=readln(fd,buf,60))>0){
      printf("%zd",write(STDOUT_FILENO,buf,line_bytes));
    }
    putchar('\n');
    close(fd);
    return 0;
}

/* 4. Melhore a implementação da função readln evitando ler um carater de cada vez. */

#define BUFFER_SIZE 10

char buffer[BUFFER_SIZE];
int next_pos2=0;
int last_read_bytes=0;
//Escrevemos aqui fora ou se for dentro temos de meter static antes

int readch(int fd,char* buf){
  /*static char buffer[BUFFER_SIZE];
    static int next_pos2=0;
    static int last_read_bytes=0;*/
    if(next_pos2 == last_read_bytes){
    //Chegamos ao fim do ficheiro e temos de ir a disco
    int bytes=0;
    if((bytes=read(fd,buffer,BUFFER_SIZE))<1){
      return 1;
    }
    next_pos2=0;
    last_read_bytes=bytes;
    }
    *buf = buffer[next_pos2];
    next_pos2++;
    return 0;
}

ssize_t readln(int fd, char *line, size_t size){
        int next_pos=0;
        int read_bytes =0;
        while(next_pos < size && readch(fd,line+next_pos)>0){
          read_bytes++;
          if(line[next_pos] == '\n'){
            break;
          }
          next_pos++;
        }
        return read_bytes;
}

int main(int argc, char *argv[]) {
    char buf[60]={};
    int fd = open("Test.txt", O_RDONLY);
    int line_bytes = 0;
    while((line_bytes=readln(fd,buf,60))>0){
      write(STDOUT_FILENO,buf,line_bytes);
    }
    putchar('\n');
    close(fd);
    return 0;
}

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

struct Person {
    char name[200];
    int age;
}Person;
/*
int main(int argc, char *argv[]) {
    if (argc <= 4){
      perror("Invalid Input");
      return 1;
    } 
    int file;
    struct Person pessoa;
    int escreve = 0;
    switch(*(argv[1]+1)){
           case 'i':
                    file = open("Person.txt", O_RDWR);
                    if (file == -1) {
                        printf("Can't open file \n");
                        exit(0);
                    }
                    if (escreve != 0){
                        strcpy(pessoa.name,argv[2]);
                        pessoa.age = atoi(argv[3]);
                    } else write(1,"Already exists\n", 17);
                    close(file);
           break;
           case 'u':
                    file = open("Person.txt", O_RDWR);
                    if (file == -1) {
                        printf("Can't open file \n");
                        exit(0);
                    }
                    if (escreve != 0){
                        pessoa.age = atoi(argv[3]);
                    } else write(1,"Something happen\n", 19);
                    close(file);
           break;
           default: perror("That mode doesn't exists");
           break;
    }
    return 0;
    }
*/
/* 7. Faca com que a opção -i diga qual a posição no ficheiro do registo inserido e acrescente a possibilidade 
de actualizar a idade de registos por essa posição. 
Exemplo:
$ pessoas -i "José Mourinho" 55
registo 973
$ pessoas -u 973 56  */

/* 8. Meca o tempo que demora a alterar idades usando ambos os metodos. Observa alguma degradação de 
desempenho a medida que o ficheiro cresce. */

/*-♅-----------------------------------------------------------------------✃----------------------------------------------------------------------♅-*/
//Notes:

struct Person {
    char name[13];
    int age;
}Person;

int main(int argc, char *argv[]) {
    // Escreve na struct
    int file = open("Person.txt",O_CREAT|O_TRUNC|O_RDWR,0644);  // Está sempre a apagar e a escrever de novo
    int file = open("Person.txt",O_CREAT|O_APPEND|O_RDWR,0644); // Escreve seguidamente no fim do ficheiro mais forte
    // Ou então no inicio lseek(file,o,SEEK_END);
    if (file == -1) {
        printf("Can't open file \n");
        exit(0);
    }
    struct Person pessoa;
    strcpy(pessoa.name,"José Mourinho");
    pessoa.age=34;
    printf("Name: %s; Age: %d\n",pessoa.name,pessoa.age);
    //Esta parte permitirá escrever no file
    if((write(file,&pessoa,sizeof(Person)))<0){
      perror("Couldn't write");
    }
    // Ler
    
    // Temos de voltar ao inicio do ficheiro
    lseek(file, 0, SEEK_SET);
    // Ou deslocar o file sizeof(Person) bytes para a esquerda
    lseek(file, -sizeof(Person), SEEK_CUR); // lê apenas 1 posição atrás
    
    struct Person read_pessoa;
    if(read(file, &pessoa, sizeof(Person)>0)){
       printf("Name: %s; Age: %d\n",read_pessoa.name,read_pessoa.age);
    }
    close(file);
    return 0;
}

// Tamanho do ficheiro escrito n*sizeof(Person)
