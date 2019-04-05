#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#define PORT 8080

void *bli(void *k) {
	int *barang;
	barang=(void *)k;
	if(*barang>0) {
		*barang-=1;
	}
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

	key_t key = 1234;
        int *barang;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        barang = shmat(shmid, NULL, 0);

	char *gagal="Transaksi Gagal";
	char *berhasil="Transaksi Berhasil";
	char *beli="Beli";

    //shmdt(value);
    //shmctl(shmid, IPC_RMID, NULL);

    //printf("Hello message sent\n");

    pthread_t thread1;//inisialisasi awal
    const char *message1 = "Thread 1";
    int  iret1, iret2;
    while(1){
        valread = read( new_socket , buffer, 1024);
        if(*barang<=0) {
            send(new_socket , gagal , strlen(gagal) , 0 );
        }
        else {
            send(new_socket , berhasil , strlen(berhasil) , 0 );
        } 
        if(strcmp(buffer, beli)==0) {
            *barang--;
        }
    }
    return 0;
}