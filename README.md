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
