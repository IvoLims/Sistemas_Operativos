/*1 Usando SIGINT, SIGQUIT e SIGALRM, escreva um programa que vá contando o tempo em segundos
desde que começou. Se, entretanto, o utilizador carregar em Ctrl+C, o programa deverá imprimir o
tempo passado. Se carregar em Ctrl+\ o programa deverá indicar quantas vezes o utilizador carregou
em Ctrl+C e terminar de seguida.*/

unsigned int count = 0;
unsigned int seconds = 0;

void sig_handler(int signum){
  seconds++;
}
void quit_handler(int signum){
  printf(" Recebi o sinal %d (CTR+\\), o CTR+C foi chamado %d vezes.\n",signum,count);
  exit(0);
}
void int_handler(int signum){
  count++;
  printf(" Recebi o sinal %d (CTR+C) e passaram-se %d segundos",signum,seconds);
}

int main(int argc,char* argv[]){
  if(signal(SIGALRM,sig_handler)){
    perror("SIGALRM");
  }
  if(signal(SIGQUIT,quit_handler)){
    perror("SIGALRM");
  }
  if(signal(SIGINT,int_handler)){
    perror("SIGALRM");
  }
  do{
    alarm(1);
    pause();
    printf("tictac\n");
  }while(1);
  return 0;
}
