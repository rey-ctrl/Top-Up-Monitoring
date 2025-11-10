#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PELANGGAN 100
#define MAX_TRANSAKSI 500

// --- DATA STRUCT ---
struct Pelanggan {
    char nomor_hp[15];
    int saldo;
    char op_name[15];
};

struct Transaksi {
    char nomor_hp[15];
    char jenis[50];
    int nominal;
};

struct PaketData {
    char nama[50];
    int harga;
};

// --- DATABASE ---
struct Pelanggan dataPelanggan[MAX_PELANGGAN];
int jumlahTerdaftar = 0;

struct Transaksi riwayatTransaksi[MAX_TRANSAKSI];
int jumlahTransaksi = 0;

struct PaketData listPaket[] = {
    {"1GB/30 Hari", 15000},
    {"5GB/30 Hari", 40000},
    {"10GB/30 Hari", 65000}
};
const int JUMLAH_PAKET = sizeof(listPaket) / sizeof(listPaket[0]);

// --- ADMIN LOGIN INFO ---
const char ADMIN_USER[] = "admin";
const char ADMIN_PASS[] = "12345";

// --- UTILITAS ---
void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void pause_and_clear() {
    printf("\nTekan ENTER untuk kembali ke menu...");
    getchar();
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int cariPelanggan(const char *nomor) {
    for (int i = 0; i < jumlahTerdaftar; i++) {
        if (strcmp(dataPelanggan[i].nomor_hp, nomor) == 0) {
            return i;
        }
    }
    return -1;
}

void logTransaksi(const char *nomor, const char *jenis, int nominal) {
    if (jumlahTransaksi < MAX_TRANSAKSI) {
        strncpy(riwayatTransaksi[jumlahTransaksi].nomor_hp, nomor, sizeof(riwayatTransaksi[jumlahTransaksi].nomor_hp)-1);
        strncpy(riwayatTransaksi[jumlahTransaksi].jenis, jenis, sizeof(riwayatTransaksi[jumlahTransaksi].jenis)-1);
        riwayatTransaksi[jumlahTransaksi].nominal = nominal;
        jumlahTransaksi++;
    }
}

// --- FUNGSI ADMIN ---
void hapusPelanggan() {
    char nomor[15];
    printf("\nMasukkan Nomor HP yang akan dihapus: ");
    scanf("%14s", nomor);
    clear_stdin();

    int idx = cariPelanggan(nomor);
    if (idx == -1) {
        printf("Nomor tidak ditemukan.\n");
        pause_and_clear();
        return;
    }

    for (int i = idx; i < jumlahTerdaftar - 1; i++) {
        dataPelanggan[i] = dataPelanggan[i + 1];
    }
    jumlahTerdaftar--;

    printf("Pelanggan %s berhasil dihapus.\n", nomor);
    pause_and_clear();
}

void isiPulsa() {
    char nomor[15];
    int nominal;
    printf("\nMasukkan Nomor HP: ");
    scanf("%14s", nomor);
    clear_stdin();

    int idx = cariPelanggan(nomor);
    if (idx == -1) {
        printf("Nomor tidak ditemukan.\n");
        pause_and_clear();
        return;
    }

    printf("Masukkan Nominal Pulsa: ");
    scanf("%d", &nominal);
    clear_stdin();

    dataPelanggan[idx].saldo += nominal;
    logTransaksi(nomor, "ISI_PULSA", nominal);
    printf("Isi pulsa berhasil!\n");
    pause_and_clear();
}

void lihatSemuaTransaksi() {
    printf("\n=== SEMUA TRANSAKSI ===\n");
    if (jumlahTransaksi == 0) {
        printf("Belum ada transaksi tercatat.\n");
    } else {
        for (int i = 0; i < jumlahTransaksi; i++) {
            printf("[%s] %s - Rp %d\n",
                   riwayatTransaksi[i].jenis,
                   riwayatTransaksi[i].nomor_hp,
                   riwayatTransaksi[i].nominal);
        }
    }
    pause_and_clear();
}

// --- FUNGSI USER ---
void daftarPelanggan() {
    int pilihan_op;
    char nomor_temp[15];

    if (jumlahTerdaftar >= MAX_PELANGGAN) {
        printf("\nERROR: Kapasitas penuh.\n");
        pause_and_clear();
        return;
    }

    printf("\n--- DAFTAR PELANGGAN BARU ---\n");
    printf("Pilih Operator:\n1. Telkomsel\n2. XL Axiata\n3. Tri\nPilihan: ");
    scanf("%d", &pilihan_op);
    clear_stdin();

    switch (pilihan_op) {
        case 1: strcpy(dataPelanggan[jumlahTerdaftar].op_name, "Telkomsel"); break;
        case 2: strcpy(dataPelanggan[jumlahTerdaftar].op_name, "XL Axiata"); break;
        case 3: strcpy(dataPelanggan[jumlahTerdaftar].op_name, "Tri"); break;
        default: strcpy(dataPelanggan[jumlahTerdaftar].op_name, "Lainnya"); break;
    }

    printf("Masukkan Nomor HP: ");
    scanf("%14s", nomor_temp);
    clear_stdin();

    if (cariPelanggan(nomor_temp) != -1) {
        printf("ERROR: Nomor sudah terdaftar!\n");
        pause_and_clear();
        return;
    }

    strcpy(dataPelanggan[jumlahTerdaftar].nomor_hp, nomor_temp);
    dataPelanggan[jumlahTerdaftar].saldo = 0;
    jumlahTerdaftar++;

    printf("Pendaftaran berhasil!\n");
    pause_and_clear();
}

void cekSaldo(const char *nomor) {
    int idx = cariPelanggan(nomor);
    if (idx == -1) {
        printf("Nomor tidak terdaftar.\n");
    } else {
        printf("Saldo Anda: Rp %d\n", dataPelanggan[idx].saldo);
    }
    pause_and_clear();
}

void beliKuota(const char *nomor) {
    int pilihan;
    int idx = cariPelanggan(nomor);
    if (idx == -1) {
        printf("Nomor tidak ditemukan.\n");
        pause_and_clear();
        return;
    }

    printf("\nPilih Paket Data:\n");
    for (int i = 0; i < JUMLAH_PAKET; i++) {
        printf("%d. %s (Rp %d)\n", i + 1, listPaket[i].nama, listPaket[i].harga);
    }
    printf("Pilihan: ");
    scanf("%d", &pilihan);
    clear_stdin();

    if (pilihan < 1 || pilihan > JUMLAH_PAKET) {
        printf("Pilihan tidak valid.\n");
        pause_and_clear();
        return;
    }

    if (dataPelanggan[idx].saldo < listPaket[pilihan - 1].harga) {
        printf("Saldo tidak cukup.\n");
        pause_and_clear();
        return;
    }

    dataPelanggan[idx].saldo -= listPaket[pilihan - 1].harga;
    logTransaksi(nomor, "BELI_PAKET_DATA", listPaket[pilihan - 1].harga);
    printf("Pembelian paket berhasil!\n");
    pause_and_clear();
}

void transferPulsa(const char *nomor) {
    char ke[15];
    int nominal;

    printf("\nNomor Tujuan: ");
    scanf("%14s", ke);
    printf("Nominal Transfer: ");
    scanf("%d", &nominal);
    clear_stdin();

    int idxDari = cariPelanggan(nomor);
    int idxKe = cariPelanggan(ke);

    if (idxKe == -1) {
        printf("Nomor tujuan tidak ditemukan.\n");
        pause_and_clear();
        return;
    }

    if (dataPelanggan[idxDari].saldo < nominal) {
        printf("Saldo tidak cukup.\n");
        pause_and_clear();
        return;
    }

    dataPelanggan[idxDari].saldo -= nominal;
    dataPelanggan[idxKe].saldo += nominal;
    logTransaksi(nomor, "TRANSFER_KELUAR", nominal);
    logTransaksi(ke, "TRANSFER_MASUK", nominal);

    printf("Transfer berhasil!\n");
    pause_and_clear();
}

void lihatRiwayat(const char *nomor) {
    int ada = 0;
    for (int i = 0; i < jumlahTransaksi; i++) {
        if (strcmp(riwayatTransaksi[i].nomor_hp, nomor) == 0) {
            printf("- %s | Rp %d\n", riwayatTransaksi[i].jenis, riwayatTransaksi[i].nominal);
            ada = 1;
        }
    }
    if (!ada) printf("Belum ada transaksi.\n");
    pause_and_clear();
}

// --- MENU ADMIN & USER ---
void menuAdmin() {
    int pilih;
    do {
        printf("\n=== MENU ADMIN ===\n");
        printf("1. Isi Pulsa\n");
        printf("2. Hapus Pelanggan\n");
        printf("3. Lihat Semua Transaksi\n");
        printf("4. Logout\n");
        printf("Pilihan: ");
        scanf("%d", &pilih);
        clear_stdin();

        switch (pilih) {
            case 1: isiPulsa(); break;
            case 2: hapusPelanggan(); break;
            case 3: lihatSemuaTransaksi(); break;
            case 4: break;
            default: printf("Pilihan tidak valid.\n"); pause_and_clear(); break;
        }
    } while (pilih != 4);
}

void menuUser(const char *nomor) {
    int pilih;
    do {
        printf("\n=== MENU USER (%s) ===\n", nomor);
        printf("1. Cek Saldo\n");
        printf("2. Beli Kuota\n");
        printf("3. Transfer Pulsa\n");
        printf("4. Lihat Riwayat Transaksi\n");
        printf("5. Logout\n");
        printf("Pilihan: ");
        scanf("%d", &pilih);
        clear_stdin();

        switch (pilih) {
            case 1: cekSaldo(nomor); break;
            case 2: beliKuota(nomor); break;
            case 3: transferPulsa(nomor); break;
            case 4: lihatRiwayat(nomor); break;
            case 5: break;
            default: printf("Pilihan tidak valid.\n"); pause_and_clear(); break;
        }
    } while (pilih != 5);
}

// --- MAIN PROGRAM ---
int main() {
    int login;
    char username[30], password[30], nomor[15];

    // Dummy data
    strcpy(dataPelanggan[0].nomor_hp, "08111");
    dataPelanggan[0].saldo = 100000;
    strcpy(dataPelanggan[0].op_name, "Telkomsel");
    jumlahTerdaftar++;

    strcpy(dataPelanggan[1].nomor_hp, "08999");
    dataPelanggan[1].saldo = 5000;
    strcpy(dataPelanggan[1].op_name, "Tri");
    jumlahTerdaftar++;

    do {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        printf("=== SISTEM TRANSAKSI PULSA ===\n");
        printf("1. Login sebagai Admin\n");
        printf("2. Masuk sebagai User\n");
        printf("3. Keluar\n");
        printf("Pilihan: ");
        scanf("%d", &login);
        clear_stdin();

        switch (login) {
            case 1:
                printf("\n--- LOGIN ADMIN ---\n");
                printf("Username: ");
                scanf("%29s", username);
                printf("Password: ");
                scanf("%29s", password);
                clear_stdin();

                if (strcmp(username, ADMIN_USER) == 0 && strcmp(password, ADMIN_PASS) == 0) {
                    printf("\nLogin berhasil!\n");
                    pause_and_clear();
                    menuAdmin();
                } else {
                    printf("\nLogin gagal! Username atau password salah.\n");
                    pause_and_clear();
                }
                break;

            case 2: {
                int pilihUser;
                printf("\n--- MENU USER ---\n");
                printf("1. Daftar Akun Baru\n");
                printf("2. Login Nomor HP\n");
                printf("3. Kembali\n");
                printf("Pilihan: ");
                scanf("%d", &pilihUser);
                clear_stdin();

                if (pilihUser == 1) {
                    daftarPelanggan();
                } else if (pilihUser == 2) {
                    printf("Masukkan Nomor HP: ");
                    scanf("%14s", nomor);
                    clear_stdin();
                    int idx = cariPelanggan(nomor);
                    if (idx == -1) {
                        printf("Nomor belum terdaftar.\n");
                        pause_and_clear();
                    } else {
                        printf("Login berhasil!\n");
                        pause_and_clear();
                        menuUser(nomor);
                    }
                }
                break;
            }

            case 3:
                printf("Terima kasih telah menggunakan sistem ini!\n");
                break;

            default:
                printf("Pilihan tidak valid.\n");
                pause_and_clear();
                break;
        }
    } while (login != 3);

    return 0;
}
