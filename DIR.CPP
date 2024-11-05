#include <iostream.h>
#include <conio.h>
#include <dos.h>
#include <dir.h>

void displayFileInfo(struct ffblk file) {
    // Hiển thị tên file
    cout << file.ff_name << "\t";
    
    // Kiểm tra nếu là thư mục
    if (file.ff_attrib & FA_DIREC) {
        cout << "<DIR>\t\t";
    } else {
        // Hiển thị kích thước file
        cout << file.ff_fsize << " bytes\t";
    }

    // Hiển thị ngày giờ
    int year = ((file.ff_fdate >> 9) & 0x7F) + 1980;
    int month = (file.ff_fdate >> 5) & 0x0F;
    int day = file.ff_fdate & 0x1F;
    int hour = (file.ff_ftime >> 11) & 0x1F;
    int minute = (file.ff_ftime >> 5) & 0x3F;

    // Chuyển đổi định dạng 12 giờ với ký tự 'a' hoặc 'p' để giống như MS-DOS
    char period = (hour >= 12) ? 'p' : 'a';
    if (hour > 12) hour -= 12;
    if (hour == 0) hour = 12;

    // Hiển thị định dạng ngày tháng và giờ phút
    cout << (month < 10 ? "0" : "") << month << "-"
         << (day < 10 ? "0" : "") << day << "-"
         << year << "  "
         << (hour < 10 ? " " : "") << hour << ":"
         << (minute < 10 ? "0" : "") << minute << period << "m" << endl;
}

int main() {
    struct ffblk file;
    int done;
    long total_size = 0;
    int file_count = 0;
    int dir_count = 0;

    // Khởi đầu chương trình với thông báo giống dir
    cout << " Volume in drive C has no label." << endl;
    cout << " Directory of C:\\*.*" << endl << endl;

    // Tìm các tệp tin trong thư mục hiện tại
    done = findfirst("*.*", &file, FA_ARCH | FA_DIREC);

    // Duyệt qua tất cả các tệp tin và thư mục tìm thấy
    while (!done) {
        displayFileInfo(file);
        if (file.ff_attrib & FA_DIREC) {
            dir_count++;
        } else {
            total_size += file.ff_fsize;
            file_count++;
        }
        done = findnext(&file);
    }

    // Hiển thị tổng kết số lượng tệp tin và thư mục
    cout << "              " << file_count << " file(s)\t    " << total_size << " bytes" << endl;
    cout << "              " << dir_count << " dir(s)\t" << "95,219,712 bytes free" << endl;

    getch();
    return 0;
}
