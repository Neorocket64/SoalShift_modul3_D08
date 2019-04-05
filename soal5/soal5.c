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

pthread_t tid[6]; //inisialisasi array untuk menampung thread
int nyawa, lapar, higen, fstock, bstat, status, inputan, mxn, mxl, mxh, mxf;
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
				case '3':
					inputan = 3;
					break;
				case '4':
					inputan = 4;
					break;
				case '5':
					inputan = 5;
					break;
				default:
					inputan = 0;
					break;
			}
	}
                         

    /*restore the old settings*/
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}

void* pemandian(void *arg)
{
	while(1)
	{
		
		sleep(1);
		if(bstat <= 0) bstat = 0;
		else bstat--;
	}
}

void* standby(void *arg)
{
	int kelaparan = 10, kebusukan = 30, hidup = 10;
	while(nyawa > 0)
	{
		system("clear");
		
		if (kelaparan < 0)
		{
			lapar -= 5;
			kelaparan = 10;
		}
		if (kebusukan < 0)
		{
			higen -= 10;
			kebusukan = 30;	
		}
		if (hidup < 0)
		{
			nyawa += 5;
			if(nyawa > mxn) nyawa = mxn;
			hidup = 10;	
		}		
		
		printf("Standby Mode\n");
		printf("Health: %d ++[%d]\n", nyawa, hidup);
		printf("Hunger: %d \n", lapar);
		printf("Hygiene: %d \n", higen);
		printf("Food Left: %d\n", fstock);
		
		if(bstat == 0)
		{
			printf("Bath is ready\n");
		}
		else
		{
			printf("Bath will be ready in %d s\n", bstat);
		}
		
		printf("\nChoices\n");
		printf("1. Eat\n");
		printf("2. Bath\n");
		printf("3. Battle\n");
		printf("4. Shop\n");
		printf("5. Exit\n");
		
		switch(inputan)
		{
			case 1:
				if(fstock > 0)
				{
					fstock--;
					lapar += 15;
					if(lapar > mxl) lapar = mxl;
				}
				break;
			case 2:
				if(bstat == 0)
				{
					bstat = 20;
					higen += 30;
					if(higen > mxh) higen = mxh;
				}
				break;
			case 3:
				status = 2;
				break;
			case 4:
				status = 3;
				break;
			case 5:
				exit(0);
			default:
				break;
		}
		if(status != 1) break;
		inputan = 0;
		sleep(1);
		kelaparan--;
		kebusukan--;
		hidup--;
		
	}
}

void* battle(void *arg)
{
	int pilihan, enmh = 100;
	while(enmh != 0 || nyawa != 0)
	{
		system("clear");
		printf("Battle Mode\n");
		printf("Monster's Health: %d'\n", enmh);
		printf("Enemy's Health: %d'\n", nyawa);
		printf("\nChoices\n");
		printf("1. Attack\n");
		printf("2. Run\n");
		switch(inputan)
		{
			case 1:
				inputan = 0;
				enmh -= 20;
				if(enmh != 0) 
				{
					system("clear");
					printf("Musuh melawan!\n");
					sleep(1);
					nyawa -= 20;
					printf("Player kena!\n");
				}
				break;
			case 2:
				status = 1;
				break;
			default:
				break;
		}
		if(status != 2) break;
		sleep(1);
	}
	if(enmh <= 0)
	{
		system("clear");
		printf("You win!\n");
		status = 1;
		sleep(1);
	}
	inputan = 0;
}

void* shop(void *arg)
{
	key_t key = 1234;
    int *value;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
    
    if(*value == NULL) *value = 0;
	
	while(nyawa != 0 || lapar != 0 || higen != 0)
	{
		system("clear");
		printf("Shop Mode\n");
		printf("Shop Food Stock: %d'\n", *value);
		printf("Your Food Stock: %d'\n", fstock);
		printf("\nChoices\n");
		printf("1. Buy\n");
		printf("2. Back\n");
		switch(inputan)
		{
			case 1:
				inputan = 0;
				if(*value != 0 && fstock < mxf) 
				{
					system("clear");
					*value = *value - 1;
					fstock++;
					printf("Terbeli!\n");
				}
				else if(fstock >= mxf)
				{
					system("clear");
					printf("Melewati batas stok!\n");
				}
				else
				{
					system("clear");
					printf("Stok shop habis!\n");
				}
				break;
			case 2:
				status = 1;
				break;
			default:
				break;
		}
		if(status != 3) break;
		sleep(1);
	}
	inputan = 0;
	status = 1;
	
	shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
}

int main()
{
	int err;
	
	char nama[5];
	printf("Input nama monster (max 5): ");
	scanf("%5s", nama);
	printf("%s", nama);
	
	nyawa = 300;
	mxn = nyawa;
	higen = 100;
	mxh = higen;
	lapar = 200;
	mxl = lapar;
	fstock = 10;
	mxf = fstock;
	
	status = 1;
	
	err = pthread_create(&(tid[4]), NULL, masuk, NULL); //membuat thread
	err = pthread_create(&(tid[5]), NULL, pemandian, NULL); //membuat thread
	
	while(nyawa != 0 || lapar != 0 || higen != 0)
	{
		switch(status)
		{
			case 1:
				err = pthread_create(&(tid[status]), NULL, standby, NULL); //membuat thread
				pthread_join(tid[status],NULL);
				break;
			case 2:
				err = pthread_create(&(tid[status]), NULL, battle, NULL); //membuat thread
				pthread_join(tid[status],NULL);
				break;
			case 3:
				err = pthread_create(&(tid[status]), NULL, shop, NULL); //membuat thread
				pthread_join(tid[status],NULL);
				break;
			default:
				break;
		}
	}
	
	return 0;
}

//gcc -pthread -std=c99 -lncurses soal3.c -o soal3
