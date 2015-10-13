//Name:Cheng Yik Yin
//SID:1155048656
//CSCI3150 Assignment1
#include <stdio.h>
#include <limits.h> // Needed by PATH_MAX
#include <unistd.h> // Needed by getcwd()
#include <stdlib.h> // Needed by exit()
#include <string.h> // Needed by strtok()
#include <glob.h> // added
#include <sys/wait.h>
#include <sys/types.h>

#define MAX 255

int tokenflag;
int exitflag;
int child_pid;

char buf[MAX];
char cwd[PATH_MAX+1];
char *input[MAX];
char path[MAX];

void display();
void tokenize(char buf[MAX]);
void newexit();
void cd();
void wildcard(char* token);


int main(int argc,char *argv[]){
  setenv("PATH","/bin:/usr/bin:.",1);
  while(1){
    display();
  }
  return 0;
}

void display() {
  if(getcwd(cwd,PATH_MAX+1) != NULL){
    printf("[3150 Shell:%s]$",cwd); // print the shell with dir
    fgets(buf,MAX,stdin); // get cmd
    buf[strlen(buf)-1] = '\0';
    tokenize(buf);
  }
  else{
    printf("Error Occured!\n");
  }
  return;
}

void tokenize(char buf[MAX]) {
  tokenflag=0;
  exitflag=0;
  int i=0;
  int j=0;
  for(j=0;j<MAX;j++){
    input[j]=NULL;
  }
  char *token = strtok(buf," ");
  while(token != NULL){
    input[i]=token;
    token = strtok(NULL," ");
    if(strstr(token,"*")!=NULL){
      wildcard(token);
    }
    if((strcmp(input[0],"cd")==0)){
      tokenflag=2;
    }
    else if((strcmp(input[0],"exit")==0)){
      tokenflag=1;
      newexit(token,i);
    }
    i=i+1;
  }
  if(tokenflag==2){
    cd(i);
  }
  if(tokenflag==0){
    if(!(child_pid = fork())){
        execvp(*input,input);
      }
      else{
        waitpid(child_pid,NULL,WUNTRACED);
      }
    }
  return;
}
void cd(int i){
  if(i!=2){
    printf("cd: wrong number of arguments\n");
  }
  else{
    if(chdir(input[1]) != -1){
      getcwd(buf,PATH_MAX+1);
    }
    else{
      printf("[arg]: cannot change directory\n");
    }
  }
}

void newexit(char* token,int i){
    if((token==NULL)&&(i==0)){
      exit(0);
    }
    else if((i!=0)&&(exitflag!=1)){
      printf("exit: wrong number of arguments\n");
      exitflag=1;
    }
}

void wildcard(char* token){
  int i;
  glob_t results;
  const char* pattern=token;
  glob(token, GLOB_NOCHECK |GLOB_APPEND, NULL, & results);
  for(i=0;i<results.gl_pathc;i++){
    printf("%s\n",results.gl_pathv[i]);
  }
  globfree(& results);
}
