#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "warung-kang-roger.h"

// Mengvalidasi input dari user
void validation(int min, int max, int *select){
    while(*select < 1 || *select > max){
        printf("NANI?! Watashi tidak mengenal input seperti itu! Silahkan coba kembali!\n");
        printf("Your input: ");
        scanf("%d", &(*select));
    }
}

// Proses setelah toko dibuka
void sellerView(int *buka){
    printf("Apa yang akan kamu lakukan sekarang?\n");
    printf("1. Tutup warung\n2. Biarkan pelanggan masuk\n");
    printf("Your input: ");
    scanf("%d", &(*buka));
    validation(1,2,buka);
    if(*buka == 1){
        *buka = 0;
    }else{
        *buka = 1;
    }
}

// Proses ketika pelanggan sudah masuk
void customerView(char *name){
    int choice = 0;
    while(choice != 3){
        printf("\nApa yang akan kamu lakukan sekarang %s-san?\n", name);
        printf("1. Watashi mau lihat menu!\n2. Watashi mau beli sesuatu!\n3. Watashi mau keluar aja\n");
        printf("Your input: ");
        scanf("%d", &choice);
        validation(1,3,&choice);
        printf("---------------------------------------------------\n");
        if(choice == 1){
            displayMenu();
        }else if(choice == 2){
            buySomething(name);
        }else{
            printf("Sayonara %s-san!\n", name);
            printf("---------------------------------------------------\n");
        }
    }
}

// Menampilkan menu ke user
void displayMenu(){
    file = fopen("menu.txt", "r");
    printf("Douzo!\n");
    printf("ID | Name       | Category   | Price      | Stock\n");
    int col = 1;
    
    // Membagikan setiap baris pada textfile menjadi sebuah atribut
    while(fgets(current, 50, file) != NULL){
        current[strcspn(current, "\n")] = 0;
        if(col != 5){
            if(col == 1){
                printf("%-2s | ", current);
            }else{
                printf("%-10s | ", current);
            }
            col++;
        }
        
        // Jika sudah baris kelipatan 5 / Kolom ke-5, kembali ke kolom pertama
        else{
            printf("%-10s\n", current);
            col = 1;
        }
    }
    fclose(file);
    printf("---------------------------------------------------\n");
}

// Proses user jika membeli
void buySomething(char *nameP){
    int select;
    char thing[10];
    char *name = (char*) malloc((21)*sizeof(char));
    int id;

    printf("Pilih bagaimana kamu ingin membeli!\n");
    printf("1. By ID\n2. By name\n");
    printf("Your input: ");
    scanf("%d", &select);
    validation(1,2,&select);
    int price;
    
    // Jika memilih dengan cara nama barang
    if(select == 2){
        price = 0;
        while(price == 0){
            printf("Masukkan nama barang: ");
            scanf(" %[^\n]%*c", thing);
            
            // Standarisasi input agar sama dengan nama pada menu
            thing[strcspn(thing, "\n")] = 0;
            for(int i = 0; thing[i]; i++){
                if(i == 0){
                    thing[i] = toupper(thing[i]);
                }else{
                    thing[i] = tolower(thing[i]);
                }
            }
            price = searchByName(thing, &id);

            // Jika tidak ditemukan, maka diminta untuk input kembali
            if(price == 0){
                printf("NANI?! Watashi tidak mengenal nama barang itu! Silahkan coba kembali!\n");
            }
            
            // Jika barang stocknya habis, maka diminta untuk input kembali
            else if(getStock(id) <= 0){
                printf("Stock barang sudah habis! Silahkan coba barang lain!\n");
                price = 0;
            }
        }
    }
    
    // Jika memilih dengan cara ID barang
    else{
        price = 0;
        while(price == 0){
            printf("Masukkan ID barang: ");
            scanf(" %d", &id);
            price = searchByID(id);
            
            // Jika tidak ditemukan, maka diminta untuk input kembali
            if(price == 0){
                printf("NANI?! Watashi tidak mengenal id barang itu! Silahkan coba kembali!\n");
            }
            
            // Jika barang stocknya habis, maka diminta untuk input kembali
            else if(getStock(id) <= 0){
                printf("Stock barang sudah habis! Silahkan coba barang lain!\n");
                price = 0;
            }
        }
    }
    

    int total;
    int amount = 0;

    // Jika jumlah yang dibeli invalid atau melebihi stock yang ada, maka ulangi input
    while(amount <= 0 || amount > getStock(id)){
        printf("Jumlah yang ingin dibeli: ");
        scanf("%d", &amount);
        if(amount <= 0 || amount > getStock(id)){
            printf("Jumlah invalid atau melebihi stock! Silahkan coba kembali!\n");
        }
    }
    
    // Menghitung total harga
    total = price*amount;
    
    // Mencari nama barang agar bisa dicetak
    name = getName(id);

    // GAK NGERTI BAT HANDLE STRING DI C ASTAGA
    if(select == 1){
        printf("Berhasil membeli %s berjumlah %d dengan total harga %d koin!\n", name, amount, total);
        fprintf(fileW, "%s\n%s\n%d\n%d\n", nameP, name, amount, total);
    }else{
        printf("Berhasil membeli %s berjumlah %d dengan total harga %d koin!\n", thing, amount, total);
        fprintf(fileW, "%s\n%s\n%d\n%d\n", nameP, thing, amount, total);
    }

    // Kurangi stock barang setelah berhasil dibeli
    decreaseStock(id, amount);
    free(name);
    printf("---------------------------------------------------\n");
}


// Mendapatkan nama barang berdasarkan ID barang
char* getName(int id){
    file = fopen("menu.txt", "r");
    int found = false;
    int col = 1;
    while(fgets(current, 50, file) != NULL){
        current[strcspn(current, "\n")] = 0;
        
        // Jika ditemukan ID yang sama, maka pada kolom ke-2, nilai dikembalikan
        if(col == 1){
            if(atoi(current) == id){
                found = true;
            }
        }

        // Jika sebelumnya ID sudah ditemukan maka kembalikan nama barang
        if(col == 2 && found){
            fclose(file);
            return current;
        }

        // Kembali ke kolom pertama jika sudah pada kolom ke-5
        if(col == 5){
            col = 1;
        }else{
            col++;
        }
    }
    fclose(file);
}


// Mendapatkan harga barang berdasarkan nama barang
int searchByName(char* name, int *id){
    file = fopen("menu.txt", "r");
    int found = false;
    int col = 1;

    while(fgets(current, 50, file) != NULL){
        current[strcspn(current, "\n")] = 0;
        
        // Menyimpan ID barang
        if(col == 1){
            *id = atoi(current);
        }

        // Jika ditemukan nama yang sama, maka pada kolom ke-4, nilai dikembalikan
        if(col == 2){
            if(strcmp(current, name) == 0){
                found = true;
            }
        }       
        
        // Jika sebelumnya nama sudah ditemukan maka kembalikan harga barang
        if(col == 4 && found){
            fclose(file);
            return atoi(current);
        }

        // Kembali ke kolom pertama jika sudah pada kolom ke-5
        if(col == 5){
            col = 1;
        }else{
            col++;
        }
    }
    fclose(file);

    // Jika tidak ditemukan maka kembalikan 0 (invalid)
    return 0;
}

// Mendapatkan harga barang berdasarkan ID barang
int searchByID(int id){
    file = fopen("menu.txt", "r");
    int found = false;
    int col = 1;

    while(fgets(current, 50, file) != NULL){
        current[strcspn(current, "\n")] = 0;
        
        // Jika ditemukan ID yang sama, maka pada kolom ke-4, nilai dikembalikan
        if(col == 1){
            if(atoi(current) == id){
                found = true;
            }
        }

        // Jika sebelumnya ID sudah ditemukan maka kembalikan harga barang
        if(col == 4 && found){
            fclose(file);
            return atoi(current);
        }

        // Kembali ke kolom pertama jika sudah pada kolom ke-5
        if(col == 5){
            col = 1;
        }else{
            col++;
        }
    }
    fclose(file);
    
    // Jika tidak ditemukan maka kembalikan 0 (invalid)
    return 0;
}

// Menampilkan riwayat transaksi
void displayTransactions(){
    fileW = fopen("transaction.txt", "r");
    printf("Recap!\n");
    printf("Name       | Bought     | Amount     | Total\n");
    int col = 1;
    while(fgets(current, 50, fileW) != NULL){
        current[strcspn(current, "\n")] = 0;
        
        // Tampilkan data pada setiap kolom, jika pada kolom terakhir maka newline
        if(col != 4){
            printf("%-10s | ", current);
            col++;
        }else{
            printf("%-10s\n", current);
            col = 1;
        }
    }
    fclose(fileW);
}

// Menyimpan riwayat transaksi pada suatu file
void saveFile(){
    char filename[100];
    char txt[5] = ".txt";

    // Meminta nama file
    printf("Tuliskan nama textfile yang akan dibuat: ");
    scanf(" %[^\n]%*c", filename);

    // Menggabungkan nama file dengan extension file .txt
    strcat(filename, txt);

    FILE* write = fopen(filename, "w");
    fileW = fopen("transaction.txt", "r");

    fprintf(write, "Name       | Bought     | Amount     | Total\n");
    int col = 1;
    while(fgets(current, 50, fileW) != NULL){
        current[strcspn(current, "\n")] = 0;
        
        // Tuliskan data pada setiap kolom, jika pada kolom terakhir maka newline
        if(col != 4){
            fprintf(write, "%-10s | ", current);
            col++;
        }else{
            fprintf(write, "%-10s\n", current);
            col = 1;
        }
    }
    fclose(fileW);
    fclose(write);
    printf("Riwayat berhasil disimpan pada file %s\n", filename);
}

// Mendapatkan jumlah stock barang berdasarkan ID barang
int getStock(int id){
    file = fopen("menu.txt", "r");
    int found = false;
    int col = 1;

    while(fgets(current, 50, file) != NULL){
        current[strcspn(current, "\n")] = 0;
        
        // Jika ditemukan ID yang sama, maka pada kolom ke-5, nilai dikembalikan
        if(col == 1){
            if(atoi(current) == id){
                found = true;
            }
        }

        // Jika sebelumnya ID sudah ditemukan maka kembalikan stock barang
        if(col == 5 && found){
            fclose(file);
            return atoi(current);
        }

        // Kembali ke kolom pertama jika sudah pada kolom ke-5
        if(col == 5){
            col = 1;
        }else{
            col++;
        }
    }
    fclose(file);
}

// Mengurangi stock barang
void decreaseStock(int id, int sold){
    int new;
    int flag = false;
    char menu[10] = "menu.txt";

    // Membuat file temporary
    FILE *temp = fopen("temporary.txt", "w");
    file = fopen(menu, "r");
    int found = false;
    int col = 1;

    while(fgets(current, 50, file) != NULL){
        
        // Jika ditemukan ID yang sama, maka pada kolom ke-5, nilai dimodifikasi
        if(col == 1){
            if(atoi(current) == id){
                found = true;
            }
        }

        // Jika sebelumnya ID sudah ditemukan maka kurangi stock barang dan modifikasi menu
        if(col == 5 && found){
            new = atoi(current) - sold;
            fprintf(temp, "%d\n", new);
            found = false;
            flag = true;
        }

        // Kembali ke kolom pertama jika sudah pada kolom ke-5
        if(col == 5){
            col = 1;
        }else{
            col++;
        }

        // Jika bukan ID yang dicari maka tulis ulang baris menu
        if(flag != true){
            fprintf(temp,current);
        }else{
            flag = false;
        }
        
    }
    fclose(file);

    // Hapus file menu lama
    remove(menu);
    fclose(temp);

    // Rename file temporary menjadi menu baru
    rename("temporary.txt", menu);
}


// Melakukan restock barang / Mengembalikan menu pada kondisi semula
void restock(){
    int i = 0;
    int stocks[10] = {10, 10, 5, 10, 2, 1, 8, 20, 30, 5}; // List stock barang
    char menu[10] = "menu.txt";

    // Membuat file temporary
    FILE *temp = fopen("temporary.txt", "w");
    file = fopen(menu, "r");
    int col = 1;

    while(fgets(current, 50, file) != NULL){
        
        // Jika pada kolom ke-5 / baris kelipatan 5, modifikasi stock menu. Selain itu, tulis ulang menu
        if(col == 5){
            fprintf(temp, "%d\n", stocks[i]);
            i++;
        }else{
            fprintf(temp,current);
        }

        // Kembali ke kolom pertama jika sudah pada kolom ke-5
        if(col == 5){
            col = 1;
        }else{
            col++;
        }
    }
    fclose(file);

    // Hapus menu lama
    remove(menu);
    fclose(temp);

    // Rename file temporary menjadi menu baru
    rename("temporary.txt", menu);

}

// Proses ketika user ingin keluar
void tutup(){
    fileW = fopen("transaction.txt", "r");
    int count = 0;
    char c;

    // Menghitung isi file
    for (c = getc(fileW); c != EOF; c = getc(fileW)){
        count++;
    }   
    fclose(fileW);

    // Jika isi file kosong maka tidak ada tampilan riwayat, jika tidak kosong tampilkan riwayat
    if(count != 0){
        displayTransactions();
        printf("Apakah ingin menyimpan riwayat transaksi?\n1) Ya\n2) Tidak\n");
        int yon;
        printf("Your input: ");
        scanf("%d", &yon);
        validation(1,2,&yon);
        if(yon == 1){
            saveFile();
        }
    }else{
        printf("Tidak Ada Penjualan\n");
    }
    printf("---------------------------------------------------\n");

    // Hapus file temporary transaction file
    remove("transaction.txt");
    printf("Warung Kang Roger secara resmi telah tutup! Otsukare~\n");
}