#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<ncurses.h>

pthread_t tid[5]; //inisialisasi array untuk menampung thread
int nyawa, lapar, higen, fstock, bstat, enmh = 50, status;
char c;

void* memilih(void *arg)
{

		timeout(1000);
		char c = getch();

	//return;
}

void* standby(void *arg)
{
	while(1)
	{
		system("clear");
		
		printf("Standby Mode\n");
		printf("Health: %d\n", nyawa);
		printf("Hunger: %d\n", lapar);
		printf("Hygiene: %d\n", higen);
		printf("Food Left :%d\n", fstock);
		
		if(bstat == 0)
		{
			printf("Bath is ready\n");
		}
		else
		{
			printf("Bath will be ready in %d s\n", bstat);
			bstat--;
		}
		
		printf("\nChoices\n");
		printf("1. Eat\n");
		printf("2. Bath\n");
		printf("3. Battle\n");
		printf("4. Shop\n");
		printf("5. Exit\n");
		
		switch(c)
		{
			case '1':
				fstock--;
				nyawa += 25;
				lapar += 15;
				break;
			case '2':
				bstat = 20;
				higen += 30;
				break;
			case '3':
				status = 2;
				return;
			case '4':
				status = 3;
				return;
			case '5':
				exit(0);
			default:
				break;
		}
		
		sleep(1);
	}
	return;
}

void* battle(void *arg)
{
	int pilihan;
	while(enmh != 0 && nyawa != 0)
	{
		printf("Battle Mode\n");
		printf("Monster's Health: %d'\n", enmh);
		printf("Enemy's Health: %d'\n", nyawa);
		printf("\nChoices\n");
		printf("1. Attack\n");
		printf("2. Run\n");
		scanf("%1d", pilihan);
		if(pilihan == 1)
		{
			enmh -= 20; 
		}
		else if(pilihan == 2)
		{
			return;
		}
	}
	return;
}

void* shop(void *arg)
{
	printf("Coming Soon...\n");
	sleep(1);
	return;
}

int main()
{
	initscr();
	
	int err;
	
	char nama[5];
	printf("Input nama monster (max 5): ");
	scanf("%5s", nama);
	printf("%s", nama);
	
	nyawa = 300;
	higen = 100;
	lapar = 200;
	
	status = 1;
	
	while(nyawa != 0)
	{
		err = pthread_create(&(tid[4]), NULL, standby, NULL); //membuat thread
		
		if(err != 0) //cek error
		{
			printf("\n can't create thread : [%s]",strerror(err));
		}
		else
		{
			printf("\n create thread success\n");
		}
		
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
	
	endwin();
	
	return 0;
}

//gcc -pthread -std=c99 -lncurses soal3.c -o soal3
