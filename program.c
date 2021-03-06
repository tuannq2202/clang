#include <stdio.h>
#include <stdlib.h>
#include "utils/function.h"

void menu() {
    printf("Ban hay nhap chuc nang can thuc hien\n");
    printf("1. Xac dinh so luong cac tuyen duong bay cua hang\n");
    printf("2. Xoa di cac tuyen duong bay cua mot hang\n");
    printf("3. Them cac tuyen duong bay cua mot hang\n");
    printf("4. Tinh toan chi phi di lai\n");
    printf("5. Thoat\n");
}

void run() {
    menu();
    int option;
    do {
        printf(">> ");
        scanf("%d", &option);
        if(option < 1 || option > 5) {
            printf("[ERROR] Chuc nang khong hop le. Hay nhap lai\n");
            continue;
        }
        if(option == 1) {
            function1();
        }
        if(option == 2) {
            function2();
        }
        if(option == 3) {
            function3();
        }
        if(option == 4) {
            function4();
        }
        if(option == 5) {
            fflush(stdin);
            printf("[EXIT] Ban chac chan muon thoat (Y): ");
            char ch; scanf("%c", &ch);
            if(ch == 'y' || ch == 'Y') {
                option = 5;
            } else {
                option = 0;
            }
        }
    } while(option != 5);
    printf("Ban da thoat chuong trinh\n");
}

int main() {
    run();
    system("pause");
}