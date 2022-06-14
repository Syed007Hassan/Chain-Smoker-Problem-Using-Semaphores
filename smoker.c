#include <stdio.h>
#include <unistd.h> /* Symbolic Constants */
#include <sys/types.h> /* Primitive System Data Types */
#include <errno.h> /* Errors */
#include <stdio.h> /* Input/Output */
#include <stdlib.h> /* General Utilities */
#include <pthread.h> /* POSIX Threads */
#include <string.h> /* String handling */
#include <semaphore.h> /* Semaphore */
#include <sys/syscall.h>
#include <linux/kernel.h>

sem_t more_needed;
sem_t match;
sem_t paper;
sem_t tobacco;


void *agent ()
{
int i=0;
int sm=1;
int s=0,p=0,m=0;
 while (1)
    {
      int number = rand() % 3;
	if(i==10){
		printf("\n\nTotal number of time smoker with ciggerete smoked: %d\n",s);
		printf("\nTotal number of time smoker with paper smoked: %d\n",p);
		printf("\nTotal number of time smoker with match smoked: %d\n",m);
		exit(0);
	}
	sleep(1);
      switch (number)
        {
           case 0: sem_post (&match); /* match and paper */
                   sem_post (&paper);
			syscall(333,"Agent has put match and paper on the table\n");
			//printf("Agent has put match and paper on the table\n");
			printf("Smoking %d time\n", sm++);
			s++;
                   break;
           case 1: sem_post (&match); /* match and tobacco */
                   sem_post (&tobacco);
			syscall(333,"Agent has put match and tobacco on the table\n");
			//printf("Agent has put match and tobacco on the table\n");
				printf("Smoking %d time\n", sm++);
			p++;
                   break;
           case 2: sem_post (&paper); /* tobacco and paper */
                   sem_post (&tobacco);
			syscall(333,"Agent has put paper and tobacco on the table\n");
			//printf("Agent has put paper and tobacco on the table\n");
				printf("Smoking %d time\n", sm++);
			m++;
                   break;
        }
      sem_wait (&more_needed); /* wait for request for more */
	i++;
    }
}

void *smoker_with_tobacco ()
{
  while (1)
    {
      sem_wait (&match); /* grab match from table */
      if (sem_trywait (&paper) == 0) /* grab paper */
        {
          /* roll cigarette and smoke */
		syscall(333,"match and paper feched");
		syscall(333,"smoker with tobacco is smoking\n");
		//printf("tobacco smoking\n");
		sleep(0.5);
          	sem_post (&more_needed); /* signal to agent */
        }
      else sem_post (&match); /* drop the match */
    }
}

void *
smoker_with_match ()
{
  while (1)
    {
      sem_wait (&paper); /* grab match from table */
      if (sem_trywait (&tobacco) == 0) /* grab paper */
        {
          /* roll cigarette and smoke */
		syscall(333,"tobacco and paper feched");
		syscall(333,"smoker with match is smoking\n");
		//printf("match smoking\n");
		sleep(0.5);
          	sem_post (&more_needed); /* signal to agent */
        }
      else sem_post (&paper); /* drop the match */
    }
}

void *
smoker_with_paper ()
{
  while (1)
    {
      sem_wait (&tobacco); /* grab match from table */
      if (sem_trywait (&match) == 0) /* grab paper */
        {
		/* roll cigarette and smoke */
		syscall(333,"match and tobacco feched");
		syscall(333,"smoker with paper is smoking\n");
		//printf("paper smoking\n");
		sleep(0.5);
         	sem_post (&more_needed); /* signal to agent */
        }
      else sem_post (&tobacco); /* drop the match */
    }
}

int main() {
	pthread_t th_1, th_2, th_3, th_4;
	sem_init(&more_needed,0,1);
	sem_init(&match,0,0);
	sem_init(&paper,0,0);
	sem_init(&tobacco,0,0);
pthread_create(&th_1,NULL,agent,NULL); // Here 6 threads equals to 6 cars on the road.
	pthread_create(&th_2,NULL,smoker_with_tobacco,NULL); // 3 cars are on the North road ready to move to south
	pthread_create(&th_3,NULL,smoker_with_paper,NULL); // 3 cars are on the east road ready to move to west
        pthread_create(&th_4,NULL,smoker_with_match,NULL);


        pthread_join(th_1,NULL);
        pthread_join(th_2,NULL);
	pthread_join(th_3,NULL);
	pthread_join(th_4,NULL);
	
	return 0;

}

