#include <stdio.h>
#include <stdlib.h>
#include <qrencode.h>

#define BLOCK "██"  // 方塊
#define SPACE "  "  // 空格

void printQRCode(QRcode *qrcode) {
    int size = qrcode->width;
    unsigned char *data = qrcode->data;

    // 顯示頂部邊框
//    for (int i = 0; i < size + 2; ++i) printf(BLOCK);
//    printf("\n");

    for (int y = 0; y < size; ++y) {
//        printf(BLOCK);  // 左側邊框
        for (int x = 0; x < size; ++x) {
            printf(data[y * size + x] & 1 ? BLOCK : SPACE);
        }
//        printf(BLOCK);  // 右側邊框
        printf("\n");
    }

    // 顯示底部邊框
//    for (int i = 0; i < size + 2; ++i) printf(BLOCK);
//    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "用法: %s <要編碼的文字>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // 使用 libqrencode 生成 QR Code
    QRcode *qrcode = QRcode_encodeString(argv[1], 0, QR_ECLEVEL_L, QR_MODE_8, 1);
    if (!qrcode) {
        fprintf(stderr, "生成 QR Code 失敗。\n");
        return EXIT_FAILURE;
    }

    // 打印 QR Code
    printQRCode(qrcode);

    // 釋放 QR Code 記憶體
    QRcode_free(qrcode);
    return EXIT_SUCCESS;
}
