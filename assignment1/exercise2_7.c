#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZE 512
#define PERM	0664

int copyfile(const char* name1, const char* name2){
	int infile, outfile;
	ssize_t nread;
	char buffer[BUFSIZE];

	if((infile=open(name1, O_RDONLY))==-1){
		return (-1);
	}

	if((outfile = open(name2, O_WRONLY|O_CREAT|O_TRUNC, PERM))==-1){
		close(infile);
		return (-2);
	}

	while((nread=read(infile,buffer,BUFSIZE))>0){
		if(write(outfile,buffer,nread)<nread){
			close(infile);
			close(outfile);
			return (-3);
		}
	}

	close(infile);
	close(outfile);

	if(nread==-1){
		return (-4);
	}else{
		return (0);
	}
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
		printf("need two fileName (origin, copy).\n");
        exit(1);
    }

    const char* origin = argv[1];
    const char* copy = argv[2];
    int result = copyfile(origin, copy);

    if (result == 0) {
        printf("File copied successfully.\n");
    } else if (result == -1 || result == -2 || result == -3 || result == -4) {
        printf("error\n");
    }

    return result;
}