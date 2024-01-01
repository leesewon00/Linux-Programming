#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define BUFSIZE 512

int main(){
	char buffer[BUFSIZE];
	int filedes;
	ssize_t nread;
	long total_chars = 0;
	long total_words = 0;
	long total_lines = 0;
	int flag = 0; // in word : 1

	if((filedes=open("anotherfile",O_RDONLY))==-1){
		printf("error in opening anotherfile\n");
		exit(1);
	}	
		
	while((nread=read(filedes,buffer,BUFSIZE))>0){
		total_chars += nread;
		for(ssize_t i = 0; i<nread; i++){
			if(ispunct(buffer[i])){
				total_words++;
			}
			else if(!isspace(buffer[i])){
				flag = 1; // in word : 1
			}
			else if(buffer[i]=='\n'){
				total_lines++;
				if(flag==1){
					total_words++;
					flag = 0;
				}
			}
			else if(buffer[i]==' '||buffer[i]=='\t'){
				if(flag==1){					
					total_words++;
					flag = 0;
				}			
			}
		}
	}

	if(flag==1){
		total_words++;
	}

	printf("total chars in anotherfile: %ld\n",total_chars);
	printf("total words in anotherfile: %ld\n",total_words);
	printf("total lines in anotherfile: %ld\n",total_lines);
	
	exit(0);
}
