#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <time.h>

/* 1. Implemente em C um programa mycp com funcionalidade similar ao comando cp. Varie o tamanho do
buffer usado e meca o tempo necessario para copiar um ficheiro de grande dimensão. */

int main(int argc, char *argv[]) {
    if (argc >= 3) {
        clock_t start_time = clock();
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
        printf("Execution time = %lfs\n", (double)(clock() - start_time) / CLOCKS_PER_SEC);
    }else{
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

//Reads both lines

ssize_t readln1(int fd, char *line, ssize_t size) {
	ssize_t res = 0;
	ssize_t i = 0;
	while ((res = read(fd, &line[i], size)) > 0 && ((char) line[i] != '\n')) {
		i+=res;
	}
	return i;
}

int main(int argc, char *argv[]) {
    char line [100];
    size_t size;
    int file = open("Test2.txt", O_RDONLY);
    if(file == -1){
       perror("Couldn't open the file to read.");
       return -1;
    }
    size = readln1(file, line, 100);
    write(1, &line, size);
    putchar('\n');
    close(file);
    return 0;
    }

/*Or
Only reads one line */

ssize_t readln2(int fd, char* line, size_t size) {
    ssize_t i = 0;
    while(i < size - 1) {
        ssize_t bytes_read = read(fd, &line[i], 1);
        if(bytes_read < 1) break;
        if(line[i++] == '\n') break;
    }
    line[i] = 0;
    return i;
}

int main(int argc, char *argv[]) {
    char line [100];
    size_t size;
    int file = open("Test2.txt", O_RDONLY);
    if(file == -1){
       perror("Couldn't open the file to read.");
       return -1;
    }
    size = readln1(file, line, 100);
    write(1, &line, size);
    putchar('\n');
    close(file);
    return 0;
    }

//Or

ssize_t readln2(int fd, char* line, size_t size) {
    ssize_t i = 0;
    while(i < size - 1) {
        ssize_t bytes_read = read(fd, &line[i], 1);
        if(bytes_read < 1) break;
        if(line[i++] == '\n') break;
    }
    line[i] = 0;
    return i;
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

ssize_t readln1(int fd, char *line, size_t size){
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
    while((line_bytes=readln1(fd,buf,60))>0){
      write(STDOUT_FILENO,buf,line_bytes);
    }
    putchar('\n');
    close(fd);
    return 0;
}

// Or

ssize_t readln2(int fd, char* line, size_t size) {
    ssize_t bytes_read = read(fd, line, size);
    if(!bytes_read) return 0;

    size_t line_length = strcspn(line, "\n") + 1;
    if(bytes_read < line_length) line_length = bytes_read;
    line[line_length] = 0;
    
    lseek(fd, line_length - bytes_read, SEEK_CUR);
    return line_length;
}

/* 5. Implemente, utilizando a função readln, um programa com funcionalidade similar ao comando nl,
que numera as linhas recebidas no seu standard input. Compare o desempenho deste programa com as
duas versões da função readln. */

int main(int argc, char const *argv[]) {
    char line[1024];
    int file = open(argv[1], O_RDONLY);
    if(file == -1){
       perror("Couldn't open the file to read.");
       return -1;
    }
    size_t size;
    clock_t start = clock();
    int i = 0;
    while((size = readln1(file, line, 1024))) {
        char lineno[100];
        sprintf(lineno, "%*d  ", 6, i++);
        write(STDOUT_FILENO, lineno, strlen(lineno));
        write(STDOUT_FILENO, line, size);
    }
    printf("\n\nExecution time (mode %d) = %lf s\n", (mode + 1), (double)(clock() - start) / CLOCKS_PER_SEC);
    close(file);
    return 0;
}

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

int escreveFicheiro(){
    struct person p1 = {"Vitor Limao", 51};
    struct person p2 = {"Jose Mourinho",40};
    struct person p3 = {"Rita Tavares",10};
    int fd = open("Person.dat",_O_CREAT | O_APPEND| O_WRONLY,0666);
    if(fd == -1){
        perror("Couldn't open file.");
        return -1;
    }
    write(fd,&p1,sizeof(p1));
    write(fd,&p2,sizeof(p2)); 
    write(fd,&p3,sizeof(p3)); 
    close(fd);
    return 0; 
}

int acrescentaPessoa(char* nome, int idade){
    Person pessoa;
    strcpy(pessoa.name, nome);
    pessoa.age = idade;
    int file = open("Person.dat", O_CREAT | O_WRONLY,0600);
    if (file == -1) {
        printf("Couldn't open file.\n");
        exit(-1);
    }
    int res = write(file, &pessoa, sizeof(Person));
    if(res < 0){
        perror("Couldn't write in file.");
        exit(-1);
    }
    close(file);
    return 0;
}

int mudaIdade(char* nome, int idade){
    Person pessoa;
    int file = open("Person.dat", O_CREAT | O_WRONLY,0600);
    if(file < 0){
        perror("Couldn't open file.\n");
        exit(-1);
    }
    while(read(file,&pessoa,sizeof(Person))){
        if(strcmp(pessoa.name,nome) == 0){
            pessoa.age = idade;
            int ls = lseek(fd,-sizeof(Person),SEEK_CUR);
            if (ls<0) {
		perror("Error lseek.");
		return -1;
	    }
            int wr = write(fd,&pessoa,sizeof(Person));
            if (wr<0) {
	        perror("Error write.");
		return -1;
	    }
            printf("Wrote person name %s age- %d\n", p.name, p.age);
        } else printf("Couldn't find person.\n");
    }
    close(file);
    return 0;
}

int main(int argc, char *argv[]) {
    if(escreveFicheiro() == -1){
        printf("Couldn't create file pessoas.dat.\n");
        return -1;
    }
    if (argc <= 4){
      perror("Invalid Input");
      return 1;
    }
	char id[20] ="";
	if(strcmp(argv[1],"-i") == 0) {
		int res = acrescentaPessoa(argv[2], atoi(argv[3]));
		snprintf(id,20,"registo %d\n",res);
		write(1,id,sizeof(id));
	}
	if(strcmp(argv[1],"-u") == 0) {
		mudaIdade(argv[2], atoi(argv[3]));
	}
	return 0;
}

/* 7. Faca com que a opção -i diga qual a posição no ficheiro do registo inserido e acrescente a possibilidade 
de actualizar a idade de registos por essa posição. 
Exemplo:
$ pessoas -i "José Mourinho" 55
registo 973
$ pessoas -u 973 56  */

int acrescentaPessoa2(char* nome, int idade){
    return 0;
}

int main(int argc, char *argv[]){
    if(escreveFicheiro() == -1){
        printf("Couldn't create file pessoas.dat.\n");
        return -1;
    }
    if (argc <= 4){
      perror("Invalid Input");
      return 1;
    }
    if(strcmp(argv[1],"-o") == 0) {
		acrescentaPessoa2(atoi(argv[2]),atoi(argv[3]));
	}
    return 0;
}

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
