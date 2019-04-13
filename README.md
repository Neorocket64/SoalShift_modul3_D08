# SISOP 2019 : Modul 2 [Soal Shift] Kelompok D8

## #1 Sorted Factorial
### Pembuatan
* Mengurutkan variabel:
    ```c
      int o, p, smt;
      for(o = 0; o < i - 1; o++)
    {
        for(p = i - 1; p > o; p--)
        {
            if(factarr[p - 1] > factarr[p])
            {
                smt = factarr[p - 1];
                factarr[p - 1] = factarr[p];
                factarr[p] = smt;
            }
        }
    }
    ```
    Kenapa sorting dahulu? karena lebih mudah mensorting input, kemudian memanggil thread!
    
* Passing ke fungsi
    ```c
    err = pthread_create(&(tid[i]), NULL, factoriul, (void*)factarr[i]);
    ```
    lihat `(void*)factarr[i]`! ini ada bagian input untuk fungsi `factoriul`
    
    singkatnya, array `factarr` yang menyimpan input dipassing ke fungsi `factoriul`.
    
    Tapi, ditransform menjadi `void*`.
    
* Unsigned Long Long?

    untuk sekarang ini, melebihi 20an faktorial akan menghasilkan angka 0.
    
    Belum diketahui cara untuk membetulkan


## #2 Penjual & Pembeli
### Pembuatan
* Membuat Client penjual dan pembeli
    
    Menggunakan template client untuk menghubungkan dengan server

    client jual dan server jual dihubungkan dengan diberi `PORT 9090`

    client beli dan server beli dihubungkan dengan diberi `PORT 8080' 
    ```c
    int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    ```
    Didalam client jual diberi beberapa line of code untuk menerima input dan mengirimkan ke server jual
    ```c
    char jual[10];
    while(1){
        scanf("%s", jual);
        send(sock, jual, strlen(jual), 0 );
    }
    ```
    Didalam client beli juga diberi line of code yang sama untuk menerima input pembelian dan mengirim ke server beli

* Membuat Server penjual dan pembeli

    * Menggunakan template server untuk penjual dan pembeli
    ```c
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
    ```
    * Menggunakan template shared memmory
    ```c
    key_t key = 1234;
    int *barang;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    barang = shmat(shmid, NULL, 0);
    ```
    * Fungsi main

    Dalam server jual terdapat thread yang berjalan untuk menerima input dari client jual dan jika string adalah 'tambah' maka akan menambah nilai `stok barang`

    Server beli memiliki isi yang sama, yang membedakan hanyalah pada server beli menerima dari client beli. Dan jika string yang diterima adalah 'beli' maka akan mengurangi nilai stock melalui shared memmory
    ```c
    iret1 = pthread_create( &thread1, NULL, cetak, (void*) barang);

    while(1) {
    valread = read( new_socket , buffer, 1024);
    if(iret1)
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
        exit(EXIT_FAILURE);
    }
    if(strcmp(buffer, tambah)==0) {
        *barang++;
    }
    memset(buffer,0,1024);
    }
    ```
    * Fungsi untuk mencetak stock
    
    Fungsi berikut dijadikan thread didalam server penjual untuk mengeluarkan output nilai stock setiap 5 detik sekali
    ```c
    void *cetak(void *c) {
    int *barang;
    barang=(void *)c;
    printf("Stock barang = %d\n", *barang); 
    sleep(5);
    }
    ```


## #3 Agmal vs Siraj
### Pembuatan
* Membuat 2 thread `Agmal` dan `Iraj`

    Thread pertama bernama Agmal yang tugas utamanya akan mengubah variabel global `wakeUp_Status`

    Agmal akan berhenti pada saat Iraj dijalankan 3 kali, maka digunakan variabel global `counter` dan `count` untuk menandai sudah berapa kali dijalankannya suatu fitur

    Thread kedua bernama Iraj sama persis dengan Agmal, yang membedakan hanyalah variabel yang diubah yaitu 'Spirit_Status'

    ```c
    void *agmal(){
        if(stat == 2){
        if(counter >= 3){
            sleep(5);
            counter = 0;            
        }
        wakeUp_Status += 15;

        if(wakeUp_Status >= 100){
        printf("Agmal Terbangun,mereka bangun pagi dan berolahraga\n\n");
        exit(0);
        }

        else if(count <3)
            printf("WakeUp_Status = %d\n\n", wakeUp_Status);        
        }
    }
    ```

* Fungsi Main

    ```c
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
        }
    ```
    Memberi pilihan fitur, dan meminta input dari user untuk menentukan pilihan yang mana

    Jika user memilih `1` Maka program akan mengeluarkan output wakeUpStatus dan SpiritStatus

    Jika user memilih `2` maka program akan menjalankan thread agmal. Jika `count` (penghitung agmal) sudah 3 kali maka program mengeluarkan pesan iraj di disable

    Jika user memilih `3` maka program akan menjalankan thread iraj. Jika `counter` (penghitung iraj) sudah 3 kali maka program mengeluarkan pesan agamal di disable


## #4 Menyimpan Proses
### Pembuatan
* Menggunakan 2 thread

    Thread pertama berisi perintah perintah untuk:
    * Menghapus FolderProses yang sudah ada
    * Membuat FolderProses
    * Mengambil proses yang berjalan dan menyimpannya dalam file.txt
    * Mengkompres file dalam bentuk zip
    
    ```c
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
    ```
    Menggunakan string agar dapat diiterasi `file-n` menjadi `file1` dan `file2` menggunakan `sprintf`
    

    ```c
    void *unzip(int n){
    strcpy(str, "");
    sprintf(str, "unzip -qd ~/Documents/FolderProses%d ~/Documents/FolderProses%d/KompresProses%d.zip", n, n, n);
    system(str);
    }
    ```
    Kemudian dibuat thread kedua untuk melakukan extract. Digunakan perintah `unzip`.

* Fungsi Main
    
    Didalam fungsi main berisi perintah untuk menjalankan kedua thread tadi dan digunakan iterasi agar membuat 2 folder dan file yang berbeda.
    ```c
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
    ```
    Diawal adalah untuk membuat thread pertama untuk membuat file, dan mengcompress

    Kemudian menampilkan loading selama 15 detik

    Setelah itu thread yang berfungsi mengextract dijalankan


## #5 Tamagochi warfare
### Pembuatan
* Input tanpa enter

  Sumber : https://stackoverflow.com/questions/1798511/how-to-avoid-pressing-enter-with-getchar
  
  singkatnya, 
  
  ada suatu manipulasi terminal untuk menangkap input (`getchar`) dan value `char` akan dijadikan `int` dalam variabel lain
  
* Thread calling and while() breaking

  Kita memiliki variabel `status` yang akan diassign kepada `thread id` untuk setiap fungsi;
  
  ```c
  while(nyawa > 0 || lapar > 0 || higen > 0)
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
        if(nyawa <= 0 || lapar <= 0 || higen <= 0) status = 0;
    }
  ```
  Ini dilakukan karena fungsi tersebut HARUS berjalan satu per 3 set!
  
  Karena infinite loop, maka perlu sebuah syarat agar bisa `break` / keluar dari loop.
  
  Setelah loop selesai, baru suatu fungsi bisa exit.
  
  Ada 2 thread yang penting untuk berjalan, input, dan `bath timer`
  
  ```c
    err = pthread_create(&(tid[4]), NULL, masuk, NULL); //membuat thread
    err = pthread_create(&(tid[5]), NULL, pemandian, NULL); //membuat thread
  ```
    
* SLEEP
  
  Dilakukan sebuah fungsi `sleep()` (bawaan, dalam satuan detik) untuk menunggu menuju loop lagi.
  
  Buat apakah sleep? supaya beberapa variabel bisa `increment` / `decrement` selama (suatu) detik;
  
  ```c
  kelaparan--;
  kebusukan--;
  hidup--;
  ```
  
  Selain itu, digunakan untuk hapus dan update layar (aka `system("clear");`).
  
* Stok
  
  Memory sharing dilakukan agar program Penjual dan Pembeli bisa satu variabel (hanya satu variabel, `stock`)
  
  ```c
  key_t key = 1234;
    int *value;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
  -------------------------------------------------------------------- //insert code here
  shmdt(value);
  shmctl(shmid, IPC_RMID, NULL); //close value (for terminate memory)
  ```
