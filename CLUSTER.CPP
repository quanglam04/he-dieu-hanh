#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>

struct BOOT {
    char jmp[3];
    char OEM[8];
    int bytes_per_sector;
    char sectors_per_cluster;
    int reserved;
    char FAT_cnt;
    int ROOT_size;
    int total_sectors;
    char media;
    int FAT_size;
    int sectors_per_track;
    int head_cnt;
    long hidden_sectors;
    long total_sectors_long;
    char unknown[3];
    long serial;
    char volume[11];
    char FAT_type[8];
    char loader[448];
    char mark[2];
};

struct ROOT {
    char name[8];
    char ext[3];
    char attr;
    char reserved[10];
    char time[2];
    char date[2];
    int first_cluster;
    long size;
};

void main() {
    int drive = 3; // Dung o dia D (A=0, B=1, C=2, D=3 ...)
    BOOT boot;
    
    // Doc boot sector tu o dia
    int res = absread(drive, 1, 0, &boot);
    if(res != 0) {
        printf("Khong the doc boot sector\n");
        return;
    }

    // Tinh toan kich thuoc bang FAT va doc bang FAT
    int FAT_size_in_bytes = boot.FAT_size * boot.bytes_per_sector;
    unsigned int *fat = (unsigned int *)malloc(FAT_size_in_bytes);
    if (fat == NULL) {
        printf("Khong du bo nho\n");
        return;
    }
    res = absread(drive, boot.FAT_size, boot.reserved, fat);
    if(res != 0) {
        printf("Khong the doc bang FAT\n");
        free(fat);
        return;
    }

    // Tinh toan kich thuoc cua thu muc goc (ROOT) va vi tri bat dau cua no
    int root_size_in_bytes = boot.ROOT_size * 32; // Moi muc thu muc goc chiem 32 byte
    int root_start_sector = boot.reserved + boot.FAT_cnt * boot.FAT_size;

    // Cap phat bo nho cho thu muc goc va doc ROOT
    ROOT *root = (ROOT *)malloc(root_size_in_bytes);
    if(root == NULL) {
        printf("Khong du bo nho\n");
        free(fat);
        return;
    }
    int root_sector_count = root_size_in_bytes / boot.bytes_per_sector;
    res = absread(drive, root_sector_count, root_start_sector, root);
    if(res != 0) {
        printf("Khong the doc ROOT\n");
        free(root);
        free(fat);
        return;
    }

    // Tim file cho truoc trong ROOT
    char filename[8] = "FILE    "; // Ten file can tim (toi da 8 ky tu, phai dien du khoang trang)
    int start_cluster = -1;
    for(int i = 0; i < boot.ROOT_size; i++) {
        if (memcmp(root[i].name, filename, 8) == 0) {
            start_cluster = root[i].first_cluster;
            printf("Tim thay file %s bat dau tu cluster: %d\n", filename, start_cluster);
            break;
        }
    }

    if (start_cluster == -1) {
        printf("Khong tim thay file %s trong ROOT\n", filename);
        free(root);
        free(fat);
        return;
    }

    // Duyet qua cac cluster cua file
    printf("Danh sach cac cluster cua file %s:\n", filename);
    unsigned int current_cluster = start_cluster;
    while (current_cluster < 0xFFF8) { // Kiem tra cluster hop le
        printf("%u ", current_cluster);
        current_cluster = fat[current_cluster];
    }

    // Giai phong bo nho
    free(root);
    free(fat);

    getch(); // Dung man hinh cho nguoi dung nhan phim
}
