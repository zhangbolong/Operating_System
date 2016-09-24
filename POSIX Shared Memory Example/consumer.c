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
	int readResult = 1;
	item elem;
    while(1){
		/* Check whether the buffer is empty and update elem */
		while(ptr->in  == ptr ->out);
		elem = (*ptr).buffer[(*ptr).out];
		(*ptr).out = ((*ptr).out + 1)  % BUFFER_SIZE;

		readResult = fprintf(pFile, "%s", elem.data);
		if(readResult < 0){
			printf("Failure to Write index ");
			break;
		}
	}
	fclose(pFile);
	shm_unlink(name);
}