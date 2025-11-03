#include <stdio.h>
#include <sqlite3.h>
#include "topup_sqlite.h"

int main() {
    sqlite3 *db;
    int rc = buka_database(&db);

    if (rc != SQLITE_OK) {
        return 1;
    }

    // Membuat tabel jika belum ada
    buat_tabel(db);

    printf("\n=== MENU TOP-UP ===\n");
    printf("1. Tambah Transaksi\n");
    printf("2. Lihat Semua Transaksi\n");
    printf("3. Keluar\n");

    int pilihan;
    printf("Pilih menu: ");
    scanf("%d", &pilihan);

    if (pilihan == 1) {
        char nomor[20];
        int nominal;

        printf("Masukkan nomor tujuan: ");
        scanf("%s", nomor);

        printf("Masukkan nominal: ");
        scanf("%d", &nominal);

        char *err_msg = 0;
        char sql[200];
        snprintf(sql, sizeof(sql),
                 "INSERT INTO TRANSAKSI (NOMINAL, NOMOR_TUJUAN) "
                 "VALUES (%d, '%s');",
                 nominal, nomor);

        rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "❌ Gagal menambah transaksi: %s\n", err_msg);
            sqlite3_free(err_msg);
        } else {
            printf("✅ Transaksi berhasil ditambahkan!\n");
        }
    } else if (pilihan == 2) {
        printf("\nDaftar Transaksi:\n");
        const char *sql_select = "SELECT * FROM TRANSAKSI;";
        sqlite3_stmt *stmt;

        rc = sqlite3_prepare_v2(db, sql_select, -1, &stmt, 0);
        if (rc == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                int id = sqlite3_column_int(stmt, 0);
                const unsigned char *waktu = sqlite3_column_text(stmt, 1);
                int nominal = sqlite3_column_int(stmt, 2);
                const unsigned char *nomor = sqlite3_column_text(stmt, 3);

                printf("ID: %d | Waktu: %s | Nominal: %d | Nomor: %s\n",
                       id, waktu, nominal, nomor);
            }
        } else {
            fprintf(stderr, "❌ Gagal membaca data: %s\n", sqlite3_errmsg(db));
        }

        sqlite3_finalize(stmt);
    } else {
        printf("Keluar program.\n");
    }

    sqlite3_close(db);
    return 0;
}
