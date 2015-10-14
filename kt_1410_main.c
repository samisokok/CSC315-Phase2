//Name:Cheng Yik Yin
//SID:1155048656
//CSCI3150 Assignment1
#include <errno.h>
#include <stdio.h>
#include <limits.h> // Needed by PATH_MAX
#include <unistd.h> // Needed by getcwd()
#include <stdlib.h> // Needed by exit()
#include <string.h> // Needed by strtok()
#include <sys/wait.h>
#include <sys/types.h>

#define MAX 255

int child_pid;
int pipe_no;
int input_size;

char buf[MAX];
char cwd[PATH_MAX+1];
char *input[MAX];
char path[MAX];
char tokenflag;

void display();
void tokenize(char buf[]);
void new_exit();
void cd();
void handle_signal(int check);
void run_cmd(char tokenflag);

int main(int argc,char *argv[]){
  setenv("PATH","/bin:/usr/bin:.",1);
  handle_signal(0);
  while( 1){
    display();
  }
  return 0;
}

void display() {
  if(getcwd(cwd,PATH_MAX+1) != NULL){
   int j;
    for(j=0;j<MAX;j++){
    buf[j]='\0';
    }
    printf("[3150 Shell:%s]$:",cwd); // print the shell with dir
    fgets(buf,MAX,stdin); // get cmd
printf("buf:");
int i;
	for (i=0; i<strlen(buf) ;i++){
    printf("%c",buf[i]);}
    buf[strlen(buf)-1] = '\0';
    tokenize(buf);
  }
  else{
    printf("Error Occured!\n");
  }

  return;
}

void tokenize(char buf[]) {
  tokenflag='s',pipe_no=0;
  int j=0;
  for(j=0;j<MAX;j++){
    input[j]=NULL;
  }
  input_size=0;
  char *token = strtok(buf," ");
  while(token != NULL){
    input[input_size]=token;
    token = strtok(NULL," ");
    if(token == "|") pipe_no++;

    if((strcmp(input[0],"cd")==0)){
      tokenflag='c';
    }
    else if((strcmp(input[0],"exit")==0)){
      tokenflag='e';
    }
    else if((strcmp(input[0],"fg")==0)){
      tokenflag='f';
    } 
    else if((strcmp(input[0],"jobs")==0)){
      tokenflag='j';
    }
    input_size++;
  }
	run_cmd(tokenflag);
  return;
}
void cd(){
  if((input_size)!=2){
    printf("cd: wrong number of arguments\n");
  }
  else if(chdir(input[1]) == -1){
      printf("[arg]: cannot change directory\n");
    }
return; 
}

void new_exit(){
    if(input_size==1){
      exit(0);
    }
    else if(input_size!=1){
      printf("exit: wrong number of arguments\n");
    }
}

void run_cmd(char tokenflag){
if (tokenflag == 'c'){
cd();
}
else if(tokenflag =='e'){
new_exit();
}
else if(tokenflag == 'f'){
//fg();
}
else if(tokenflag == 'j'){
//jobs();
}
else if(tokenflag=='s'){
    if(!(child_pid = fork())){
        handle_signal(1);
        execvp(*input,input);
        if(errno == ENOENT){
        printf("command not found\n\n");
       } else{
        printf("unknown error\n");
       } exit(0);
         }
      else{
        waitpid(child_pid,NULL,WUNTRACED);
        }
    }
return;
}


void handle_signal(int check){
if(check==0){
signal(SIGINT,SIG_IGN);
signal(SIGQUIT,SIG_IGN);
signal(SIGTERM,SIG_IGN);
signal(SIGTSTP,SIG_IGN);}
else if(check==1){
signal(SIGINT,SIG_DFL);
signal(SIGQUIT,SIG_DFL);
signal(SIGTERM,SIG_DFL);
signal(SIGTSTP,SIG_DFL);}

return;}
