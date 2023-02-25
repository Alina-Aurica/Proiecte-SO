#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "a2_helper.h"

sem_t *sem1, *sem2, *sem3, *sem4, *sem5, *sem6, *sem7;
//sem_t sem[49];
int nrTh = 0;

typedef struct {
	int noP;
	int noTh;
} TH_STRUCT;

void* thread_function4(void* arg) 
{
	TH_STRUCT *s = (TH_STRUCT*)arg;
	
	if(s->noTh == 1)
	{	
		sem_wait(sem1); //punem in asteptare T1, pana cand porneste T3, prin semaforul sem1
		info(BEGIN, s->noP, s->noTh);
		info(END, s->noP, s->noTh);
		sem_post(sem2); //deblocam T3
	}
	else
		if(s->noTh == 3)
		{
			info(BEGIN, s->noP, s->noTh);
			sem_post(sem1); //dupa ce T3 a pornit, activam T1
			sem_wait(sem2); //asteptam sa se incheie T1, prin activarea sem2
			info(END, s->noP, s->noTh);
		}
		else 
			if(s->noTh == 2)
			{
				sem_wait(sem6);
				info(BEGIN, s->noP, s->noTh);
				info(END, s->noP, s->noTh);
				sem_post(sem7);
			}
			else
			{
				info(BEGIN, s->noP, s->noTh);
				info(END, s->noP, s->noTh);
			}
	
	return NULL;
}

void* thread_function2(void* arg)
{
	TH_STRUCT *s = (TH_STRUCT*)arg;
	
	/*
	sem_wait(sem3);
	info(BEGIN, s->noP, s->noTh);
	sem_wait(sem4);
	nrTh++;
	if(nrTh <= 4)
	{
		sem_post(sem3);
		//sem_post(sem5);
	}
	sem_post(sem4);
	sem_post(sem5);
	sem_wait(sem5);
	info(END, s->noP, s->noTh);
	sem_wait(sem3);
	nrTh--;
	sem_post(sem3);
	*/
	
	/*
	if(s->noTh < 5)
	{
	info(BEGIN, s->noP, s->noTh);
	info(END, s->noP, s->noTh);
	sem_post(&sem[s->noTh]);
	}
	
	
	if(s->noTh >= 5)
	{
	sem_wait(&sem[s->noTh-4]);
	sem_wait(&sem[s->noTh-3]);
	sem_wait(&sem[s->noTh-2]);
	sem_wait(&sem[s->noTh-1]);
	info(BEGIN, s->noP, s->noTh);
	info(END, s->noP, s->noTh);
	sem_post(&sem[s->noTh]);
	}
	*/
	
	sem_wait(sem3);
	info(BEGIN, s->noP, s->noTh);
	info(END, s->noP, s->noTh);
	sem_post(sem3);
	
		
	return NULL;
}

void* thread_function3(void* arg) 
{
	TH_STRUCT *s = (TH_STRUCT*)arg;
	
	if(s->noTh == 4)
	{
		info(BEGIN, s->noP, s->noTh);	
		info(END, s->noP, s->noTh);
		sem_post(sem6);
	}
	else 
		if(s->noTh == 2)
		{
			sem_wait(sem7);
			info(BEGIN, s->noP, s->noTh);	
			info(END, s->noP, s->noTh);
		}
		else
		{
			info(BEGIN, s->noP, s->noTh);	
			info(END, s->noP, s->noTh);
		}
		
	return NULL;
}

int main(){
    init();

    info(BEGIN, 1, 0);
    
    pid_t pid2, pid3, pid4, pid5, pid6, pid7;
    pthread_t tid4[5], tid2[49], tid3[6];
    TH_STRUCT s4[5], s2[49], s3[6];
    
    sem1 = sem_open("sem1", O_CREAT, 0644, 0);
    sem2 = sem_open("sem2", O_CREAT, 0644, 0);
    sem3 = sem_open("sem3", O_CREAT, 0644, 4);
    sem4 = sem_open("sem4", O_CREAT, 0644, 0);
    sem5 = sem_open("sem5", O_CREAT, 0644, 0);
    sem6 = sem_open("sem6", O_CREAT, 0644, 0);
    sem7 = sem_open("sem7", O_CREAT, 0644, 0);
    
    /*
    sem_init(&sem[0], 0, 1);
    for(int i = 1; i <= 48; i++)
    {
    	sem_init(&sem[i], 0, 0);
    }
    */
    
    pid2 = fork();
    if(pid2 == 0) //creem procesul P2
    {
    	info(BEGIN, 2, 0);
    	
    	pid3 = fork();
    	if(pid3 == 0) //creem procesul P3
    	{
    		info(BEGIN, 3, 0);
    		
    		//aici trebuie create thread-urile procesului P3
    		
	    	for(int i = 1; i <= 5; i++)
	    	{
	    		s3[i].noP = 3;
	    		s3[i].noTh = i;
	    		pthread_create(&tid3[i], NULL, thread_function3, &s3[i]);
	    	}
	    	
	    	for(int i = 1; i <= 5; i++)
	    		pthread_join(tid3[i], NULL);
    		
    		info(END, 3, 0);
    		exit(0);
    	}
    	
    	waitpid(pid3, NULL, 0);
    	
    	//aici trebuie create thread-urile procesului P2
    	
    	for(int i = 1; i <= 48; i++)
    	{
    		s2[i].noP = 2;
    		s2[i].noTh = i;
    		pthread_create(&tid2[i], NULL, thread_function2, &s2[i]);
    	}
    	
    	for(int i = 1; i <= 48; i++)
    		pthread_join(tid2[i], NULL);
    	
    	info(END, 2, 0);
    	exit(0);
    }
    
	    pid4 = fork();	
	    if(pid4 == 0) //creem procesul P4
	    {
	    	info(BEGIN, 4, 0);
	    	
	    	pid5 = fork();
	    	if(pid5 == 0) //creem procesul P5
	    	{
	    		info(BEGIN, 5, 0);
	    		
	    		pid6 = fork();		
	    		if(pid6 == 0) //creem procesul P6
	    		{
	    			info(BEGIN, 6, 0);
	    			
	    			pid7 = fork();	
	    			if(pid7 == 0) //creem procesul P7
	    			{
	    				info(BEGIN, 7, 0);
	    				info(END, 7, 0);
	    				exit(0);
	    			}
	    			
	    			waitpid(pid7, NULL, 0);
	    			info(END, 6, 0);
	    			exit(0);
	    		}
	    		
	    		waitpid(pid6, NULL, 0);
	    		info(END, 5, 0);
	    		exit(0);
	    	}
	    	
	    	waitpid(pid5, NULL, 0);
	    	
	    	//aici trebuie create thread-urile procesului P4
	    	
	    	for(int i = 1; i <= 4; i++)
	    	{
	    		s4[i].noP = 4;
	    		s4[i].noTh = i;
	    		pthread_create(&tid4[i], NULL, thread_function4, &s4[i]);
	    		
	    	}
	    	
	    	for(int i = 1; i <= 4; i++)
	    		pthread_join(tid4[i], NULL);
	    	
	    		
	    	info(END, 4, 0);
	    	exit(0);
    }
    
    waitpid(pid2, NULL, 0);
    waitpid(pid4, NULL, 0);
    info(END, 1, 0);
    
    sem_close(sem1);
    sem_unlink("sem1");
    sem_close(sem2);
    sem_unlink("sem2");
    sem_close(sem3);
    sem_unlink("sem3");
    //sem_close(sem4);
    //sem_close(sem5);
    sem_close(sem6);
    sem_unlink("sem6");
    sem_close(sem7);
    sem_unlink("sem7");
    
    return 0;
}
