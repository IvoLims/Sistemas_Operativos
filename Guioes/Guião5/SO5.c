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

//2

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

