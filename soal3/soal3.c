#include<stdio.h>
#include<stdlib.h>
#include<pthread.h> //library thread
#include<sys/types.h>
#include<string.h>
#include<unistd.h>


int count = 0;
int counter = 0;
int stat;
int wakeUp_Status = 0;
int Spirit_Status = 100;

pthread_t thread1, thread2;


void *agmal(){
    if(stat == 2){
        if(counter >= 3){
            // pthread_join(thread1, NULL);
            sleep(5);
            counter = 0;            
        }
        // count++;
        wakeUp_Status += 15;
        // if(count == 3){
        //     printf("Fitur Iraj Ayo Tidur disabled 10 s\n\n");
        //     // count = 0;
        // }
        
        if(wakeUp_Status >= 100){
        printf("Agmal Terbangun,mereka bangun pagi dan berolahraga\n\n");
        exit(0);
        }

        else if(count <3)
            printf("WakeUp_Status = %d\n\n", wakeUp_Status);        
    }
}

void *iraj(){
    if(stat == 3){
        if(count >= 3){
            // pthread_join(thread2, NULL);
            sleep(5);
            count = 0;
        }
        // counter++;
        Spirit_Status -= 20;
        // if(counter == 3){
        //     printf("Agmal Ayo Bangun disabled 10 s\n\n");
        //     // counter = 0;
        // }

        if(Spirit_Status <= 0){
        printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n\n");
        exit(0);
        }

        else
            printf("Spirit_Status = %d\n\n", Spirit_Status);
    }   
}

int main(){
    int pili;  
    while(1){
        printf("1. All Status\n");
        printf("2. Agmal Ayo Bangun\n");
        printf("3. Iraj Ayo Tidur\n>>> ");
        scanf("%d", &pili);
        
        if(pili == 1){    
            printf("WakeUp_Status = %d\n", wakeUp_Status);
            printf("Spirit_Status = %d\n\n\n", Spirit_Status);
        }
        else if(pili == 2){
            stat = 2;
            count++;
            if(count == 3 ){
                printf("Fitur Iraj Ayo Tidur disabled 10 s\n\n");
            }
            pthread_create(&thread1, NULL, agmal, NULL);
            pthread_join(thread1, NULL);
        }
        else if(pili == 3){
            stat = 3;
            counter++;
            if(counter == 3 ){
                printf("Agmal Ayo Bangun disabled 10 s\n\n");
            }
            pthread_create(&thread2, NULL, iraj, NULL);
            pthread_join(thread2, NULL);
        }

        // printf("count = %d\ncounter = %d\n\n", count, counter);
    } 
    return 0;
}