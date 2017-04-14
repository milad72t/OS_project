//CHUNK AND MMAP PROJETC 2 QUESTION 2
//milad teimouri
//91522059
//gcc -o p CHUNK-project2.c





#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

int sizeofread = 0;
int sizeofchunk = 0;
int totalRead = 0;
int printable = 0, nonprintable = 0 ;
struct stat structfile;
int i;

int main(int argc, char *argv[]){

	char * buffer;
	int infile = open(argv[1],O_RDONLY);
	clock_t begin, end;
	double time_spent;
	begin = clock();
	if ( argc == 2 ){

		sizeofchunk = 1024;

		buffer = (char *) malloc(sizeofchunk);
		int readByte = 1;
		while ( readByte != 0 ) {
			readByte = read(infile,buffer,sizeofchunk);
			totalRead = totalRead + readByte;
			
			for (i = 0; i < readByte; ++i)
			{
				if(isprint(buffer[i])){
					++printable;
				
					if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t'){
						++nonprintable;
				    }
				}
			}

		}
		free(buffer);
	}
		else if(argc == 3 && !strcmp(argv[2],"mmap")){
		
		stat(argv[1],&structfile);
		totalRead = structfile.st_size;
		char * memoryMap = (char *) mmap(0,totalRead,PROT_READ,MAP_SHARED,infile,0);
		for (i = 0; i < totalRead; ++i)
		{
			if(isprint(memoryMap[i])){
				//printf("%c\n", memoryMap[i]);
					++printable;
					
					if (memoryMap[i] == ' ' || memoryMap[i] == '\n' || memoryMap[i] == '\t'){
						//printf("%d\n", memoryMap[i]);
						++nonprintable;
					}
			}
		}
	
		munmap(memoryMap,totalRead);
		//printf("%d\n",totalRead );
		close(infile);

	}
	else if(argc == 3 && strcmp(argv[2],"1024")){

		sizeofchunk = atoi(argv[2]);

		buffer = (char *) malloc(sizeofchunk);
		int readByte = 1;
		while ( readByte != 0 ) {
			readByte = read(infile,buffer,sizeofchunk);
			totalRead = totalRead + readByte;
			
			for (i = 0; i < readByte; ++i)
			{
				if(isprint(buffer[i])){
					//printf("%c\n", buffer[i]);

					++printable;
				
					if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t'){
					++nonprintable;
					}
				}
			}

		}
		free(buffer);
	}


	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
   

	float perOfprint = ((float)printable*100/totalRead);
	float perOfwhite = ((float)nonprintable*100/totalRead);

	printf("%d printable characters out of %d bytes, %.2f % \n",printable,totalRead,perOfprint);
	printf("%d whitespace characters out of %d bytes, %.2f % \n",nonprintable,totalRead,perOfwhite);

	printf("time of executation : %lf  \n",time_spent);

}