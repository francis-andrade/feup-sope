#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <signal.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

//Prototypes
void sigIntHandler(int signo);
void sigUsr1Handler(int signo);
void findName(char dirPath[], char name[], char func[], char* execArgs[]);
void findType(char dirPath[], char type[], char func[], char* execArgs[]);
void findPerm(char dirPath[], char perm[], char func[], char* execArgs[]);
void doFunc(char filePath[], char func[], char* execArgs[]);

pid_t fatherPid;
int flag = 1;

int main(int argc, char* argv[]){
    if(argc < 5){
        fprintf(stderr, "Invalid number of arguments, insert at least 4\n");
        exit(1);
    }
    
    char* rootDirPath = argv[1];

    DIR* rootFolder = opendir(rootDirPath);

    if(rootFolder == NULL){
        fprintf(stderr, "The path you specified to start your search, couldn't be opened\n");
        exit(2);
    }
    
    
    //Sigint handler installation
    struct sigaction sigAct;
    sigAct.sa_handler = sigIntHandler;
    sigemptyset(&sigAct.sa_mask);
    sigaddset(&sigAct.sa_mask, SIGINT);
    sigAct.sa_flags = 0;    
    
    if(sigaction(SIGINT, &sigAct, NULL) < 0){
        fprintf(stderr, "The Interruption Signal Handler couldn't be installed\n");
        exit(3);
    }

    sigAct.sa_handler = sigUsr1Handler;
    sigemptyset(&sigAct.sa_mask);
    sigaddset(&sigAct.sa_mask, SIGINT);
    sigAct.sa_flags = 0;    
    
    if(sigaction(SIGUSR1, &sigAct, NULL) < 0){
        fprintf(stderr, "The User1 Signal Handler couldn't be installed\n");
        exit(3);
    }

    char* option = argv[2];
    char* fileProp = argv[3];
    char* func = argv[4];

    if (strcmp(func, "-print") && strcmp(func, "-exec") && strcmp(func, "-delete")){
        fprintf(stderr, "%s argument not found ('-print', '-delete' or '-exec')\n", func);
        exit(5);
    }
    
    int i;
    char ** execArgs;
    if (argc > 5){
        execArgs = (char **)malloc(sizeof(char*)*(argc - 5));
        for(i = 0; i < argc - 5; i++){
            execArgs[i] = argv[i + 5];            
            execArgs[argc -6] = NULL;
        }
    }
    
    fatherPid = getpid();
    
    if(!strcmp(option, "-name"))
        findName(rootDirPath, fileProp, func, execArgs);
    else if(!strcmp(option, "-type"))
        findType(rootDirPath, fileProp, func, execArgs);
    else if(!strcmp(option, "-perm"))
        findPerm(rootDirPath, fileProp, func, execArgs);
    else{
        fprintf(stderr, "%s argument not found ('-name', '-type' or '-perm')\n", option);
        exit(4);
    }

    if (argc > 5)
        free(execArgs);
    
    return 0;        
}

void findName(char dirPath[], char name[], char func[], char* execArgs[]){
    DIR* folderPtr = opendir(dirPath);
    struct dirent* dirEntry;
    struct stat fileStat;
    char copySTR[200];
    
    if(folderPtr == NULL){
        //fprintf(stderr, "The path couldn't be opened\n");
        exit(2);
    }
    
    chdir(dirPath);

    while( (dirEntry = readdir(folderPtr)) != NULL){
        strcpy(copySTR, dirPath);
        if(dirPath[strlen(dirPath) - 1] != '/')
                strcat(copySTR, "/");
        
        lstat(dirEntry->d_name, &fileStat);
        
        if(S_ISDIR(fileStat.st_mode)){
            
            if(!strcmp(dirEntry->d_name, ".") || !strcmp(dirEntry->d_name, ".."))
                continue;
            
            if(fork() == 0){
		flag = 1;
                findName(strcat(copySTR, dirEntry->d_name), name, func, execArgs); //ruiAndreLeixo/Aliados\0                                                                                    
		exit(0);
	    }

	    if(!strcmp(dirEntry->d_name, name)){
                doFunc(strcat(copySTR, name), func, execArgs);
            }

        }
        else if(S_ISREG(fileStat.st_mode)){
            if(!strcmp(dirEntry->d_name, name)){
                doFunc(strcat(copySTR, name), func, execArgs);
            }
        }
    }
    
 
    pid_t pid;
    while ((pid = waitpid(-1, NULL, 0))){ //Wait for all children
        if (errno == ECHILD){
            break;
        }
    } 
}

void findType(char dirPath[], char type[], char func[], char* execArgs[]){
    DIR* folderPtr = opendir(dirPath);
    struct dirent* dirEntry;
    struct stat fileStat;
    
    if(folderPtr == NULL){
        //fprintf(stderr, "The path couldn't be opened\n");
        exit(2);
    }
    
    
    
    chdir(dirPath);

    while( (dirEntry = readdir(folderPtr)) != NULL){
        char copySTR[200];
        strcpy(copySTR, dirPath);
        if(dirPath[strlen(dirPath) - 1] != '/')
                strcat(copySTR, "/");
        lstat(dirEntry->d_name, &fileStat);
        
        if(S_ISDIR(fileStat.st_mode)){
            
            if(!strcmp(dirEntry->d_name, ".") || !strcmp(dirEntry->d_name, ".."))
                continue;
            
            strcat(copySTR, dirEntry->d_name);
            
            if(!strcmp(type, "d")){
                doFunc(copySTR, func, execArgs);
            }
            
            if(fork() == 0){
                findType(copySTR, type, func, execArgs);
            }
            //printf("Teste %s\n", strcat(copySTR, dirEntry->d_name));
        }
        else if(S_ISREG(fileStat.st_mode)){
             if(!strcmp(type, "f"))
                doFunc(strcat(copySTR, dirEntry->d_name), func, execArgs);            
        }
    }
    
    pid_t pid;
    while ( (pid = waitpid(-1, NULL, 0) != 0)){ //Wait for all children
        if (errno == ECHILD){
            break;
        }
    } 
}
void findPerm(char dirPath[], char perm[], char func[], char* execArgs[]){
    DIR* folderPtr = opendir(dirPath);
    struct dirent* dirEntry;
    struct stat fileStat;
    char copySTR[200];
    
    if(folderPtr == NULL){
        //fprintf(stderr, "The path couldn't be opened\n");
        exit(2);
    }
    
    strcpy(copySTR, dirPath);
    if(dirPath[strlen(dirPath) - 1] != '/')
                strcat(copySTR, "/");
    
    chdir(dirPath);

    while( (dirEntry = readdir(folderPtr)) != NULL){
        lstat(dirEntry->d_name, &fileStat);
        
        if(S_ISDIR(fileStat.st_mode)){
            
            if(!strcmp(dirEntry->d_name, ".") || !strcmp(dirEntry->d_name, ".."))
                continue;
            
            if(fork() == 0){
                findPerm(strcat(copySTR, dirEntry->d_name), perm, func, execArgs);
            }
            //printf("Teste %s\n", strcat(copySTR, dirEntry->d_name));
        }
        else if(S_ISREG(fileStat.st_mode)){
           
            
        }
    }
    
    pid_t pid;
    while ( (pid = waitpid(-1, NULL, 0) != 0)){ //Wait for all children
        if (errno == ECHILD){
            break;
        }
    } 
}



void sigIntHandler(int signo){
    char answer;
   
    if(fatherPid == getpid()){
        do {
            printf("Are you sure you want to terminate (Y/N)? ");
            scanf("%c", &answer);
        
            if(toupper(answer) == 'Y'){
        	kill(0, SIGTERM);
            }
	    else if(toupper(answer) == 'N'){
		kill(0, SIGUSR1);
	    }

	}while(toupper(answer) != 'Y' && toupper(answer) != 'N');

    }else{
	while(flag) sleep(1);
	flag = 1;
    }
}

void sigUsr1Handler(int signo){
    flag = 0;
}

void doFunc(char filePath[], char func[], char* execArgs[]){
    if(!strcmp(func, "-print"))
      printf("%s\n", filePath);
    
    else if(!strcmp(func, "-delete"))
        remove(filePath);
    
    else{
        if(fork() == 0){
            execArgs[1] = filePath;
            execvp(execArgs[0], execArgs + 1);
            
        }
    }
}


