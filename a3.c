#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>

#define FIFO_NAME1 "RESP_PIPE_23210"
#define FIFO_NAME2 "REQ_PIPE_23210"
    		
int main(void)
{
    int fd1 = -1, fd2 = -1, fd3 = -1;
    int ok = 1;
 
    char connectmsj[8];
    char pong[5];
   
    int requestnr = 0;
    int connectnr = 7;
    int shm;
    char *sharedChar = NULL;
    volatile char *data;
    int size_fis = 0;
    unsigned int dimMem;

    unlink(FIFO_NAME1);
    //"RESP_PIPE_23210"
    if(mkfifo(FIFO_NAME1, 0600) != 0)
    {
        //printf("1");
        printf("ERROR\n");
        printf("cannot create the response pipe | cannot open the request pipe\n");
        return 1;
    }
    
    //"REQ_PIPE_23210"
    fd1 = open(FIFO_NAME2, O_RDONLY);
    if(fd1 == -1)
    {
        //printf("2");
        printf("ERROR\n");
        printf("cannot create the response pipe | cannot open the request pipe\n");
        return 1;
    }

    //"RESP_PIPE_23210"
    fd2 = open(FIFO_NAME1, O_WRONLY);
    if(fd2 == -1)
    {
        //printf("3");
        printf("ERROR\n");
        printf("cannot create the response pipe | cannot open the request pipe\n");
        return 1;
    }

    strcpy(connectmsj, "CONNECT");
    if(write(fd2, &connectnr, 1) != 1 || write(fd2, connectmsj, 7) != 7)
    {
        //printf("4");
        printf("ERROR\n");
        printf("cannot create the response pipe | cannot open the request pipe\n");
        return 1;
    }
    else
    {
        printf("SUCCESS\n");
    }
    
    while(ok == 1)
    {
    	//ok++;
    	requestnr=0;
    	read(fd1, &requestnr, 1);
    	//printf("requestnr:%d\n",requestnr);
    	char requestmsj[requestnr+1];
    	//requestmsj=(char*)malloc((requestnr+1)*sizeof(char));
    	read(fd1, requestmsj, requestnr);
    	requestmsj[requestnr] = '\0';
    	//printf("requestmsj:%s\n",requestmsj);
    	
    	if(strcmp(requestmsj, "PING") == 0)
    	{
    		char dim = 4;
    		unsigned int ping_pong_nr = 23210;
    		strcpy(pong, "PONG");
    		write(fd2, &dim, 1);
    		write(fd2, requestmsj, 4);
    		write(fd2, &dim, 1);
    		write(fd2, pong, 4);
    		write(fd2, &ping_pong_nr, 4);
    	}
    	
    	
    	if(strcmp(requestmsj, "CREATE_SHM") == 0)
    	{
    		
    		
    		//printf("intram in create shm");
    		shm = shm_open("/RTsZPZ", O_CREAT | O_RDWR, 0664);
    		read(fd1, &dimMem, 4);
    		
    		//printf("%d\n",dimMem);
    		ftruncate(shm, dimMem);
    		sharedChar = (char*)mmap(NULL, dimMem, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
    		
    		char succ[8], err[6], creat[11];
    		char succnr = 7, errnr = 5, creatnr = 10; 
    		strcpy(succ, "SUCCESS");
    		strcpy(err, "ERROR");
    		strcpy(creat, "CREATE_SHM");
    		
    		if(shm > 0 && sharedChar != (void*)-1)
    		{
    			write(fd2, &creatnr, 1);
    			write(fd2, creat, creatnr);
    			write(fd2, &succnr, 1);
    			write(fd2, succ, succnr);
    			
    		}
    		else
    		{
    			write(fd2, &creatnr, 1);
    			write(fd2, creat, creatnr);
    			write(fd2, &errnr, 1);
    			write(fd2, err, errnr);
    		}
    		
    		
    	}
    	
    	if(strcmp(requestmsj, "WRITE_TO_SHM") == 0)
    	{
    		//printf("suntem aici\n\n");
    		unsigned int offset;
    		read(fd1, &offset, 4);
    		unsigned int value;
    		read(fd1, &value, 4);
    		//printf("offset: %d\n,value:%d\n",offset,value);
    		char succ[8], err[6], writee[13];
    		char succnr = 7, errnr = 5, writenr = 12; 
    		strcpy(succ, "SUCCESS");
    		strcpy(err, "ERROR");
    		strcpy(writee, "WRITE_TO_SHM");
    		
    		if(offset >= 0 && offset <= dimMem && offset + sizeof(value) < dimMem)
    		{
    			//char adresa = offset + (char) sharedChar;
    			//char adresa = offset + 5036964;
    			//char *sharedMem = (char*)sharedChar;
    			memcpy((sharedChar + offset), &value, 4);
    			
    			write(fd2, &writenr, 1);
    			write(fd2, writee, writenr);
    			write(fd2, &succnr, 1);
    			write(fd2, succ, succnr);
    			
    		}
    		else
    		{
    			write(fd2, &writenr, 1);
    			write(fd2, writee, writenr);
    			write(fd2, &errnr, 1);
    			write(fd2, err, errnr);
    			
    		}
    	}
    	
    	
    	if(strcmp(requestmsj, "MAP_FILE") == 0)
    	{
    		unsigned int file_dim;
    		read(fd1, &file_dim, 1);
    		char file_name[file_dim+1];
    		read(fd1, &file_name, file_dim);
    		file_name[file_dim] = '\0';
    		
    		char succ[8], err[6], map[13];
    		char succnr = 7, errnr = 5, mapnr = 8; 
    		strcpy(succ, "SUCCESS");
    		strcpy(err, "ERROR");
    		strcpy(map, "MAP_FILE");
    		
    		fd3 = open(file_name, O_RDONLY);
    		if(fd3 == -1)
    		{
    			write(fd2, &mapnr, 1);
    			write(fd2, map, mapnr);
    			write(fd2, &errnr, 1);
    			write(fd2, err, errnr);
    		}
    		
    		size_fis = lseek(fd3, 0, SEEK_END);
    		lseek(fd3, 0, SEEK_SET);
    		
    		data = (char*)mmap(0, size_fis, PROT_READ, MAP_SHARED, fd3, 0);
    		if(data == (void*)-1)
    		{
    			write(fd2, &mapnr, 1);
    			write(fd2, map, mapnr);
    			write(fd2, &errnr, 1);
    			write(fd2, err, errnr);
    		}
    		else
    		{
    			write(fd2, &mapnr, 1);
    			write(fd2, map, mapnr);
    			write(fd2, &succnr, 1);
    			write(fd2, succ, succnr);
    		}
    	}
    	
    	if(strcmp(requestmsj, "READ_FROM_FILE_OFFSET") == 0)
    	{
    		//IMPLEMENTARE
    		//printf("suntem aici\n\n");
    		unsigned int offset = 0;
    		read(fd1, &offset, 4);
    		printf("Offset %d\n", offset);
    		unsigned int no_of_bytes = 0;
    		read(fd1, &no_of_bytes, 4);
    		printf("no_of_bytes %d\n", no_of_bytes);
    		
    		char succ[8], err[6], readoff[22];
    		char succnr = 7, errnr = 5, readoffnr = 21; 
    		strcpy(succ, "SUCCESS");
    		strcpy(err, "ERROR");
    		strcpy(readoff, "READ_FROM_FILE_OFFSET");
    		
    		printf("%d\n", size_fis);
    		if(offset + no_of_bytes <= size_fis)
    		{
    			//printf("\n\n suntem aici");
    			char *dataa = (char*) data;
    			memcpy(sharedChar, (dataa + offset), no_of_bytes);
    			
    			write(fd2, &readoffnr, 1);
    			write(fd2, readoff, readoffnr);
    			write(fd2, &succnr, 1);
    			write(fd2, succ, succnr);
    		}
    		else
    		{
    			write(fd2, &readoffnr, 1);
    			write(fd2, readoff, readoffnr);
    			write(fd2, &errnr, 1);
    			write(fd2, err, errnr);
    		}
    	}
    	
    	if(strcmp(requestmsj, "READ_FROM_FILE_SECTION") == 0)
    	{
    		//IMPLEMENTARE
    		return 0;
    	}
    	
    	if(strcmp(requestmsj, "READ_FROM_LOGICAL_SPACE_OFF") == 0)
    	{
    		//IMPLEMENTARE
    		return 0;
    	}
    	
    	if(strcmp(requestmsj, "EXIT") == 0)
    	{
    		close(fd1);
    		close(fd2);
    		unlink(FIFO_NAME1);
    		return 0;
    	}
    	
    	//return 0;
    }

    //close(fd1);
    //close(fd2);
    //unlink(FIFO_NAME1);


    return 0;
}
