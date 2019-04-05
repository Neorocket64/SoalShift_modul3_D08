#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<termios.h>            //termios, TCSANOW, ECHO, ICANON
#include<unistd.h>     //STDIN_FILENO
#include<sys/types.h>
#include<sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

pthread_t tid[2]; //inisialisasi array untuk menampung thread
int inputan;
char c;

void* masuk()
{
	//source : https://stackoverflow.com/questions/1798511/how-to-avoid-pressing-enter-with-getchar
	static struct termios oldt, newt;
	
    /*tcgetattr gets the parameters of the current terminal
    STDIN_FILENO will tell tcgetattr that it should write the settings
    of stdin to oldt*/
    tcgetattr( STDIN_FILENO, &oldt);
    /*now the settings will be copied*/
    newt = oldt;

    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    newt.c_lflag &= ~(ICANON);          

    /*Those new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

    /*This is your part:
    I choose 'e' to end input. Notice that EOF is also turned off
    in the non-canonical mode*/
    while((c=getchar()))      
    {
    	switch(c)
			{
				case '1':
					inputan = 1;
					break;
				case '2':
					inputan = 2;
					break;
				default:
					inputan = 0;
					break;
			}
	}
                         

    /*restore the old settings*/
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}

void main()
{
        key_t key = 1234;
        int *value, status, err;
        
        status = 1;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        value = shmat(shmid, NULL, 0);

		if(*value == NULL) *value = 0;
		
		err = pthread_create(&(tid[1]), NULL, masuk, NULL); //membuat thread

        while(1)
        {
        	system("clear");
			printf("Shop\n");
			printf("Food Stock: %d\n", *value);
			printf("\nChoices\n");
			printf("1. Restock\n");
			printf("2. Exit\n");
			switch(inputan)
			{
				case 1:
					inputan = 0;
					*value = *value + 1;
					break;
				case 2:
					status = 0;
					break;
				default:
					break;
			}
			if(status != 1) break;
			sleep(0);
		}
        shmdt(value);
        shmctl(shmid, IPC_RMID, NULL);
}
