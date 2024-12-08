#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qrencode.h>

#define SPACE "  "  // 空格

// 從文件中獲取下一個非空字元（跳過換行和空格）
char nextCharacter(FILE *file) {
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch != '\n' && ch != ' ') {
            return ch;
        }
    }
    rewind(file); // 若到文件結尾，回到開頭
    return nextCharacter(file);
}

void printQRCode(QRcode *qrcode, FILE *file) {
    int size = qrcode->width;
    unsigned char *data = qrcode->data;

    // 顯示頂部邊框
    for (int i = 0; i < size + 2; ++i) printf(SPACE);
    printf("\n");

    for (int y = 0; y < size; ++y) {
        printf(SPACE);  // 左側邊框
        for (int x = 0; x < size; ++x) {
            if (data[y * size + x] & 1) {
                // 替換黑色區域為文件中的字元
                char ch = nextCharacter(file);
                printf("%c", ch);
                ch = nextCharacter(file);
                printf("%c", ch);
                ch = nextCharacter(file);
                printf("%c", ch);
            } else {
                printf(SPACE);
            }
        }
        printf(SPACE);  // 右側邊框
        printf("\n");
    }

    // 顯示底部邊框
    for (int i = 0; i < size + 2; ++i) printf(SPACE);
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 4 || strcmp(argv[2], "-i") != 0) {
        fprintf(stderr, "用法: %s \"要生成QRcode的文字\" -i article.txt\n", argv[0]);
        return EXIT_FAILURE;
    }

    // 打開文件
    FILE *file = fopen(argv[3], "r");
    if (!file) {
        perror("無法打開文件");
        return EXIT_FAILURE;
    }

    // 使用 libqrencode 生成 QR Code
    QRcode *qrcode = QRcode_encodeString(argv[1], 0, QR_ECLEVEL_L, QR_MODE_8, 1);
    if (!qrcode) {
        fprintf(stderr, "生成 QR Code 失敗。\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // 打印 QR Code
    printQRCode(qrcode, file);

    // 清理資源
    QRcode_free(qrcode);
    fclose(file);
    return EXIT_SUCCESS;
}
