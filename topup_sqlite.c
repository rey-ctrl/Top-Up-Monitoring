#include <stdio.h>
#include <sqlite3.h>
#include "topup_sqlite.h"

// Callback kosong (dibutuhkan oleh sqlite3_exec)
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

// Fungsi untuk membuat tabel
void buat_tabel(sqlite3 *db) {
    char *err_msg = 0;
    const char *sql =
        "CREATE TABLE IF NOT EXISTS TRANSAKSI ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "WAKTU_TOPUP DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "NOMINAL INTEGER NOT NULL, "
        "NOMOR_TUJUAN TEXT NOT NULL);";

    int rc = sqlite3_exec(db, sql, callback, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "❌ Gagal membuat tabel: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("✅ Tabel TRANSAKSI siap digunakan.\n");
    }
}

// Fungsi untuk membuka database
int buka_database(sqlite3 **db) {
    int rc = sqlite3_open("topup.db", db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "❌ Gagal membuka database: %s\n", sqlite3_errmsg(*db));
        return rc;
    }
    printf("📂 Database berhasil dibuka.\n");
    return SQLITE_OK;
}
