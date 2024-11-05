#include <stdio.h>
#include <dos.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>

// Cấu trúc đại diện cho sector khởi động (boot sector) cho hệ thống FAT16
struct BOOT {
    char jmp[3];                 // Mã nhảy
    char OEM[8];                 // Nhãn OEM
    int bytes_per_sector;        // Số byte trên mỗi sector
    char sectors_per_cluster;    // Số sector trên mỗi cluster
    int reserved;                // Số sector dự trữ
    char FAT_cnt;                // Số bảng FAT
    int ROOT_size;               // Số lượng mục trong thư mục ROOT
    int total_sectors;           // Tổng số sector (nếu nhỏ hơn 65535)
    char media;                  // Byte định danh phương tiện lưu trữ
    int FAT_size;                // Kích thước mỗi bảng FAT
    int sectors_per_track;       // Số sector trên mỗi track
    int head_cnt;                // Số đầu đọc trên ổ đĩa
    long hidden_sectors;         // Số sector ẩn
    long total_sectors_long;     // Tổng số sector (nếu lớn hơn 65535)
    char unknown[3];             // Phần không xác định
    long serial;                 // Số serial của đĩa
    char volume[11];             // Nhãn của volume
    char FAT_type[8];            // Loại hệ thống FAT
    char loader[448];            // Boot loader
    char mark[2];                // Dấu kết thúc sector khởi động
};

void main() {
    int drive = 3; // Ổ đĩa cần đọc (A=0, B=1, C=2, D=3 ...)
    BOOT boot;

    // Đọc Boot Sector từ ổ đĩa
    int res = absread(drive, 1, 0, &boot);
    if (res != 0) {
        printf("Khong the doc sector khoi dong\n");
        return;
    }

    // Hiển thị thông tin từ Boot Sector
    printf("Thong tin Boot Sector:\n");
    printf("-------------------------\n");
    printf("Ma nhay: %02X %02X %02X\n", boot.jmp[0], boot.jmp[1], boot.jmp[2]);
    printf("Nham OEM: %.8s\n", boot.OEM);
    printf("So byte tren moi sector: %d\n", boot.bytes_per_sector);
    printf("So sector tren moi cluster: %d\n", boot.sectors_per_cluster);
    printf("So sector du tru: %d\n", boot.reserved);
    printf("So bang FAT: %d\n", boot.FAT_cnt);
    printf("So muc trong thu muc ROOT: %d\n", boot.ROOT_size);
    printf("Tong so sector (neu nho hon 65535): %d\n", boot.total_sectors);
    printf("Dinh danh phuong tien luu tru: 0x%X\n", boot.media);
    printf("Kich thuoc moi bang FAT: %d\n", boot.FAT_size);
    printf("So sector tren moi track: %d\n", boot.sectors_per_track);
    printf("So dau doc tren o dia: %d\n", boot.head_cnt);
    printf("So sector an: %ld\n", boot.hidden_sectors);
    printf("Tong so sector (neu lon hon 65535): %ld\n", boot.total_sectors_long);
    printf("Phan khong xac dinh: %02X %02X %02X\n", boot.unknown[0], boot.unknown[1], boot.unknown[2]);
    printf("So serial cua dia: %08lX\n", boot.serial);
    printf("Nham cua volume: %.11s\n", boot.volume);
    printf("Loai he thong FAT: %.8s\n", boot.FAT_type);
    printf("Dau ket thuc sector khoi dong: %02X %02X\n", boot.mark[0], boot.mark[1]);
    printf("-------------------------\n");
}
