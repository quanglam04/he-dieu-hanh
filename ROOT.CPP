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

    // Tinh toan kich thuoc cua thu muc goc (ROOT) va vi tri bat dau cua no
    int root_size_in_bytes = boot.ROOT_size * 32; // Moi muc thu muc goc chiem 32 byte
    int root_start_sector = boot.reserved + boot.FAT_cnt * boot.FAT_size;

    // Cap phat bo nho cho thu muc goc
    ROOT *root = (ROOT *)malloc(root_size_in_bytes);
    if(root == NULL) {
        printf("Khong du bo nho\n");
        return;
    }

    // Doc thu muc goc tu dia
    int root_sector_count = root_size_in_bytes / boot.bytes_per_sector;
    res = absread(drive, root_sector_count, root_start_sector, root);
    if(res != 0) {
        printf("Khong the doc ROOT\n");
        free(root);
        return;
    }

    // Hien thi 10 muc dau tien trong thu muc goc
    printf("10 muc dau tien trong ROOT:\n");
    printf("Ten Tep\tMo rong\tKich thuoc (byte)\tCluster dau tien\n");
    printf("--------------------------------------------------\n");

    for(int i = 0; i < 10; i++) {
        if(root[i].name[0] == 0x00) break; // Neu gap muc rong, dung lai
        if(root[i].name[0] == 0xE5) continue; // Bo qua muc da xoa
        
        // Hien thi ten tep
        for(int j = 0; j < 8 && root[i].name[j] != ' '; j++)
            printf("%c", root[i].name[j]);
        printf("\t");

        // Hien thi mo rong tep
        for(int j = 0; j < 3; j++)
            printf("%c", root[i].ext[j]);
        printf("\t");

        // Hien thi kich thuoc tep va cluster dau tien
        printf("%ld\t\t%d\n", root[i].size, root[i].first_cluster);
    }

    // Giai phong bo nho
    free(root);
    
    getch(); // Dung man hinh cho nguoi dung nhan phim
}
