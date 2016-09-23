/********* consumer.c ***********/
#include "headers.h"

int main()
{
	const char *name = "OS";
    int shm_fd;
    shared_struct *ptr;
	int size, i;
	FILE * pFile = fopen ("output.txt","w");
	if (pFile == NULL) {
		printf("fopen failed\n");
		exit(-1);
	}
    
	/* open the shared memory segment as if it was a file */
	shm_fd = shm_open(name, O_RDWR, 0666);
	if (shm_fd == -1) {
		printf("Shared memory failed\n");
		exit(-1);
	}
    
	/* the size of the shared memory segment */
	size = sizeof(shared_struct);

    /* map the shared memory segment to the address space of the process */
    ptr = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Map failed\n");
        exit(-1);
    }

    /* Read data and write to file output.txt */
	int readIndex = 0;
    while(1){
		int readResult = fputs((*ptr).data[readIndex]), pFile);
		if(readResult == EOF){
			printf("Failure to Write index No.%d ",readIndex);
			break;
		}
		readIndex++;
	}
	fclose(pFile);
	shm_unlink(name);
}
