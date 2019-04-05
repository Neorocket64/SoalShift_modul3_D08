#include<stdio.h>
#include<stdlib.h>
#include<pthread.h> //library thread
#include<sys/types.h>
#include<string.h>
#include<unistd.h>


int n=1;
char str[250];
pthread_t tid[10];

void *tulis(int n){
    //Apus
    strcpy(str, "");    
    sprintf(str, "rm ~/Documents/FolderProses%d -dr", n);
    system(str);

    // Membuat FolderProses
    strcpy(str, "");
    sprintf(str, "mkdir ~/Documents/FolderProses%d", n);
    system(str);

    // Mengambil isi aux
    strcpy(str, "");
    sprintf(str, "ps aux --no-heading | head -10 > ~/Documents/FolderProses%d/SimpanProses%d.txt", n, n);
    system(str);

    // Ngezip
    strcpy(str, "");
    sprintf(str, "zip -qmj ~/Documents/FolderProses%d/KompresProses%d ~/Documents/FolderProses%d/SimpanProses%d.txt", n, n, n, n);
    system(str);
}

// void *unzip(void *args){
void *unzip(int n){
    strcpy(str, "");
    sprintf(str, "unzip -qd ~/Documents/FolderProses%d ~/Documents/FolderProses%d/KompresProses%d.zip", n, n, n);
    system(str);
}

int main(){
    int x=2;

    for(int i=1; i<=x; i++){
        pthread_create(&tid[x], NULL, &tulis, i);
        pthread_join(tid[x], NULL);
    }   

    printf("Menunggu 15 detik untuk mengkompress...\n");
    for(int a=1; a<=15; a++){
        printf("%d\n", a);
        sleep(1);
    }
    printf("Proses Selesai!\n");

    for(int j=1; j<=x; j++){
        pthread_create(&tid[j], NULL, &unzip, j);
        pthread_join(tid[j], NULL);
    }

    return 0;
}
