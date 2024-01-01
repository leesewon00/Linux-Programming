#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

char *workfile = "junk";

int main(){
	int filedes;

	if((filedes = open(workfile, O_RDWR))==-1){
		printf("Couldn't open %s\n", workfile);
		exit(1); // 비정상 종료
	}

	exit(0); // 정상종료
	return 0;
}
