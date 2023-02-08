#include <stdio.h>
#include "warung-kang-roger.c"

int main(){
    // Mengembalikan stock barang seperti semula
    restock();

    // Membuat temporary transaction file
    fileW = fopen("transaction.txt", "w");
    int buka = 3;
    
    while(buka != 0){
        printf("Selamat datang di Warung Kang Roger!\n");
        sellerView(&buka);
        printf("---------------------------------------------------\n");
        if(buka == 1){
            char *name;
            printf("Youkoso pelanggan-tachi!\nPlease tell us your name!\n");
            printf("Your name: ");
            scanf(" %[^\n]%*c", name);
            
            // Pindah proses ke customer view
            customerView(name);
        }else{
            fclose(fileW);
            tutup();
        }
    }
    return 0;
}