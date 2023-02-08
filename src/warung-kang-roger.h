#include <stdio.h>

#ifndef __WARUNG_KANG_ROGER_H_
#define __WARUNG_KANG_ROGER_H_

#define true 1
#define false 0

FILE* file;
FILE* fileW;
char current[50];

// Mengvalidasi input dari user
void validation(int min, int max, int *select);

// Proses setelah toko dibuka
void sellerView(int *buka);

// Proses ketika pelanggan sudah masuk
void customerView(char* name);

// Menampilkan menu ke user
void displayMenu();

// Proses user jika membeli
void buySomething(char *name);

// Mendapatkan harga barang berdasarkan nama barang
int searchByName(char *name, int *id);

// Mendapatkan harga barang berdasarkan ID barang
int searchByID(int id);

// Mendapatkan nama barang berdasarkan ID barang
char* getName(int id);

// Menampilkan riwayat transaksi
void displayTransactions();

// Menyimpan riwayat transaksi pada suatu file
void saveFile();

// Mendapatkan jumlah stock barang berdasarkan ID barang
int getStock(int id);

// Mengurangi stock barang
void decreaseStock(int id, int sold);

// Melakukan restock barang / Mengembalikan menu pada kondisi semula
void restock();

// Proses ketika user ingin keluar
void tutup();

#endif