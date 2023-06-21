#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int quit(char** args);
int cd(char** args);
int a(char** args);
int b(char** args);
int c(char** args);
int d(char** args);
int f(char** args);
int g(char** args);
char **split(char *line);
char *readLine();
int execute(char** args);
void addHistory(char* line);
void split1(char *s);

#define BUFSIZE  1024
#define NUMBUILTINS  8

char* builtins[] = {"quit", "cd", "a", "b", "c", "d", "f", "g"};
int (*builtinFN[])(char **) = {&quit, &cd, &a, &b, &c, &d, &f, &g};

int main(int argc, char*argv[]) {
  system("clear");
  FILE *history;
  history = fopen("/home/alireza/Desktop/history.txt","w+");
  if (!history) {
      fprintf(stderr,"can't open file\n");
   }
  fclose(history);
  int quit = 1;
  while(quit != 0) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("%s> $ ", cwd);
	FILE *history ;
	char* input = readLine();
	addHistory(input);
    char** inputArray = split(input);
    quit = execute(inputArray);
  }
  return 1;
}

char *readLine()
{
	char *line = (char *)malloc(sizeof(char) * 1024); // Dynamically Allocate Buffer
	char c;
	int pos = 0, bufsize = 1024;
	if (!line) // Buffer Allocation Failed
	{
		fprintf(stderr,"\nBuffer Allocation Error.");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		c=getchar();
		if (c == EOF || c == '\n') // If End of File or New line, replace with Null character
		{
			line[pos] = '\0';
			return line;
		}
		else
		{
			line[pos] = c;
		}
		pos ++;
		// If we have exceeded the buffer
		if (pos >= bufsize)
		{
			bufsize += 1024;
			line = realloc(line, sizeof(char) * bufsize);
			if (!line) // Buffer Allocation Failed
			{
			fprintf(stderr,"\nBuffer Allocation Error.");
			exit(EXIT_FAILURE);
			}
		}
	}

}


char **split(char *line)
{
	char **tokens = (char **)malloc(sizeof(char *) * 64);
	char *token;
	char space[10] = " \t\n\r\a";
	int pos = 0, bufsize = 64;
	if (!tokens)
	{
		fprintf(stderr,"\nBuffer Allocation Error.");	
		exit(EXIT_FAILURE);
	}
	token = strtok(line, space);
	while (token != NULL)
	{
		tokens[pos] = token;
		pos ++;
		if (pos >= bufsize)
		{
			bufsize += 64;
			line = realloc(line, bufsize * sizeof(char *));
			if (!line) // Buffer Allocation Failed
			{
			fprintf(stderr,"\nBuffer Allocation Error.");	
			exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, space);
	}
	tokens[pos] = NULL;
	return tokens;
}

int execute(char** args){
	int ret = 1;
	int isBuiltin = 0;
	if (args[0] == NULL){
		return ret;
	}
	int i;
	for ( i = 0; i < NUMBUILTINS; i++)
	{
		if(strcmp(args[0],builtins[i])==0){
			ret = (builtinFN[i])(args);
			isBuiltin = 1;
			break;
		}
	}
	if (isBuiltin == 0 )
	{
		pid_t pid = fork();
		if(pid < 0){
			fprintf(stderr,"Error Forking\n");	
		}
		else if(pid == 0){
			execvp(args[0], args);
			fprintf(stderr,"Command or executable not recognized.\n");	
		}
		else	waitpid(pid, NULL, 0);
	}
	/*
	pid_t pid = fork();
	if(pid < 0){
		printf("Error Forking\n");
	}
	else if(pid == 0){
		if (strcmp(args[0],"a") == 0)
			{
				a(args);
				exit(1);
			}
		else if(strcmp(args[0],"b") == 0)
			{
				b(args);
				exit(1);
			}
		else if(strcmp(args[0],"f") == 0)
			{
				f(args);
				exit(1);
			}
		else if(strcmp(args[0],"g") == 0)
			{
				g(args);
				exit(1);
			}
		else if(strcmp(args[0],"d") == 0)
			{
				d(args);
				exit(1);
			}
		else if(strcmp(args[0],"c") == 0)
			{
				c(args);
				exit(1);
			}
		else
			{
			execvp(args[0], args);
			}
		printf("Command or executable not recognized.\n");
	}

	else{
		waitpid(pid, NULL, 0);
	}
	*/
	return ret;
}


int quit(char** args) {
  return 0;
}
int a(char** args){
	FILE *file;
	file = fopen(args[1],"r");
	if (!file) {
      fprintf(stderr,"can't open file\n");	
      return 1;
   }
	char *line;
	char *space = " \t\n\r\a";
	while (fgets(line, 1024, file)) {
		if(strtok(line, space) != NULL){
			printf("%s\n",line); 
		}
	}
	fclose(file);
	return 1;
}
int f(char** args){
	FILE *file;
	file = fopen(args[1],"r");
	if (!file) {
      fprintf(stderr,"can't open file\n");	
      return 1;
   }
	char *line;
	int count = 0;
	while (fgets(line, 1024, file)) {
		count++;
	}
	printf("%d\n",count);
	fclose(file);
	return 1;
}
int g(char** args){
	FILE *file;
	file = fopen(args[1],"r");
	if (!file) {
      fprintf(stderr,"can't open file\n");	
      return 1;
   }
	char *line;
	int count = 0;
	while (fgets(line, 1024, file) && count < 10) {
		line[strlen(line)-1]='\0';
		printf("%s\n", line); 
		count++;
	}
	fclose(file);
	return 1;
}
int d(char** args){
	FILE *file;
	file = fopen(args[1],"r");
	if (!file) {
      fprintf(stderr,"can't open file\n");	
      return 1;
   }
	char *line;
	while (fgets(line, 1024, file)) {
		if (line[0] != '#')
		{
			printf("%s",line);
		}
		
    /*		int i;
		for ( i = 0; line[i] != '\0'; i++)
		{
			if(line[i] != '#'){
				printf("%c",line[i]);
			}
			else{
				break;
			}
		}
		printf("\n");
	*/
	}

	fclose(file);
	return 1;
}
void split1(char *s) {
    char *p2 = s;
    while(*s != '\0') {
        if(*s != '\t' && *s != '\n' && *s != ' ') {
            *p2++ = *s++;
        } else {
            ++s;
        }
    }
    *p2 = '\0';
}
int c(char** args){
	FILE *file;
	file = fopen(args[1],"r");
	if (!file) {
      fprintf(stderr,"can't open file\n");	
      return 1;
   }
	char* line;
	while (fgets(line, 1024, file)) {
		line[strlen(line)-1]='\0';
		split1(line);
		printf("%s",line);
	}
	printf("\n");
	fclose(file);
	return 1;
}
int b(char** args){
	FILE *file;
	file = fopen(args[1],"r");
	if (!file) {
      fprintf(stderr,"can't open file\n");	
      return 1;
   }
	FILE *temp;
	temp = fopen(args[1],"r");
	char result[1024];
	int max = -1;
	int i = 0;
	int j;
	while (!feof(file))
	{
		i++;
		fseek(file,0,SEEK_SET);
		char word[1024];
		for(j = i; j > 0; j--){
			fscanf(file,"%s",word);
		}
		int count = 0;
		*temp = *file;
		while (!feof(temp)) {
			char x[1024];
			fscanf(temp,"%s",x);
			if (strcmp(word,x)==0)
			{
				count++;
			}	
		}
		if (count > max)
		{
			max = count;
			strcpy(result,word);
		}
	}
	printf("%s\n",result);
	fclose(file);
	return 1;
	}
int cd(char** args){
	if (args[1] == NULL) 
	{
		fprintf(stderr,"expected argument to \"cd\"\n");
	} 
	else 
	{
		if (chdir(args[1]) != 0) 
		{
			perror("Error: ");
		}
	}
	return 1;
}
void addHistory(char* line){
	FILE* history ;
  	history = fopen("/home/alireza/Desktop/history.txt","a");
	if (!history) {
      fprintf(stderr,"can't open history file\n");	
      return ;
   }
	fprintf(history, "%s\n", line);
	fclose(history);
}


