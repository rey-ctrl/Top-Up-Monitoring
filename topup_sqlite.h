#ifndef TOPUP_SQLITE_H
#define TOPUP_SQLITE_H

#include <sqlite3.h>

// Deklarasi fungsi
void buat_tabel(sqlite3 *db);
int buka_database(sqlite3 **db);

#endif
