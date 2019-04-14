#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printManPage(void);

int main(int argc, char **argv) {
	char line[256];
	int ii, count = 0;
	int n;
	
	FILE * pFile;
	pFile = fopen("/dev/mygpio", "r");
	if (pFile==NULL) {
		fputs("mygpio module isn't loaded\n",stderr);
		return -1;
	}

	// Check if in read mode
	if (argc == 2) {
	  n = atoi(argv[1]);
	  printf("Value  Speed  State  Direction  Brightness\n");
	  while(1) {
	    while(fgets(line, 256, pFile) != NULL){
	      printf("%s", line);
	    }
	    fclose(pFile);
	    pFile = fopen("/dev/mygpio", "r");
	    sleep(n);
	  }
	}
	else {
		printManPage();
	}

	fclose(pFile);
	return 0;
}

void printManPage() {
	printf("Error: invalid use.\n");
	printf(" counterstat [number]\n");
}
