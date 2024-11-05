#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>

struct BOOT {
    int bytes_per_sector;
    int FAT_size;
    int reserved;
    char FAT_cnt;
};

void main() {
    int drive = 3; // A=0, B=1, C=2, D=3 ...
    BOOT boot;
    int res;

    // Đọc boot sector để lấy thông tin về FAT
    res = absread(drive, 1, 0, &boot);
    if(res != 0) {
        printf("Khong the doc boot sector\n");
        return;
    }

    // Tính toán kích thước của FAT và cấp phát bộ nhớ
    unsigned int *fat = (unsigned int *)malloc(boot.FAT_size * boot.bytes_per_sector);
    if (fat == NULL) {
        printf("Khong du bo nho\n");
        return;
    }

    // Đọc bảng FAT từ ổ đĩa
    res = absread(drive, boot.FAT_size, boot.reserved, fat);
    if(res != 0) {
        printf("Khong the doc bang FAT\n");
        free(fat);
        return;
    }

    // In nội dung của 100 ô FAT đầu tiên
    printf("Noi dung cua 100 o FAT dau tien:\n");
    for (int i = 0; i < 100; i++) {
        printf("FAT[%d] = %u\n", i, fat[i]);
    }

    // Giải phóng bộ nhớ và dừng chương trình
    free(fat);
    getch(); // Dừng màn hình cho người dùng nhấn phím
}
