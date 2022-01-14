#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "function.h"

#define QUIT_CODE -2202
#define NOTFOUND_CODE -2002

#define DATA_X_AIRLINE "./data/X.txt"
#define DATA_Y_AIRLINE "./data/Y.txt"
#define MAX 1000

#define X_CURRENCY '$'
#define Y_CURRENCY 'Y'

const int X_COST = 50;
const int Y_COST = 6000;

typedef struct _Node{
    int value;
    struct _Node *next;
} Node;

typedef struct _City {
    int src;
    int numsOfDest;
    Node *dest;
} City;

City cities[MAX];

void addNode(Node **root,int dest) {
    Node *new = (Node*)malloc(sizeof(Node));

    if((*root) == NULL) {
        (*root) = new;
    }

    new->value = dest;
    new->next = NULL;
    Node *cur = (*root), *prev = NULL;
    while(cur != NULL) {
        prev = cur;
        cur = cur->next;
    }
    prev->next = new;
    new->next = cur;
}

void deleteNode(Node **root, int dest) {
    if((*root) == NULL) {
        return ;
    }
    Node *cur = (*root), *prev;
    if((*root)->value == dest) {
        (*root) = (*root)->next;
        free(cur);
        return ;
    }
    while(cur != NULL && cur->value != dest) {
        prev = cur;
        cur = cur->next;
    }
    if(cur != NULL) {
        prev->next = cur->next;
        free(cur);
    }
    return ;
}

/**
 * @brief 1 là tìm thấy, 0 là không tìm thấy
 * 
 * @param root 
 * @param dest 
 * @return int 
 */
int findNode(Node *root, int dest) {
    Node *node = root;
    while(node != NULL) {
        if(node->value == dest) return 1;
        node = node->next;
    }
    return 0;
}

void showList(FILE *fo, Node *root) {
    Node *node = root;
    if(node == NULL) {
        fprintf(fo, "");
        return ;
    }
    while(node->next != NULL) {
        fprintf(fo, "%d,", node->value);
        node = node->next;
    }
    fprintf(fo, "%d", node->value);
}

void show(Node *root) {
    Node *node = root;
    if(node == NULL) return ; 
    while(node->next != NULL) {
        printf("%d,", node->value);
        node = node->next;
    }
    printf("%d", node->value);
}

City formatCity(char *line) {
    City ct;
    int count = 0;
    char *t1 = strtok(line, "   ");
    ct.src = atoi(t1);
    ct.dest = NULL;
    t1 = strtok(NULL, "   ");
    char *t2 = strtok(t1, ",");
    while(t2 != NULL) {
        int check = atoi(t2);
        if(check == 0) {
            count = 0;
            break;
        } else {
            addNode(&ct.dest, check);
            ++count;
        }
        t2 = strtok(NULL, ",");
    }
    ct.numsOfDest = count;
    return ct;
}

int readFile(FILE *fi) {
    char line[1024];
    fgets(line, 1024, fi);
    int n = atoi(line);
    for(int i = 0; i < n; ++i) {
        cities[i].numsOfDest = 0;
    }
    for(int i = 0; i < n; ++i) {
        fgets(line, 1024, fi);
        City ct = formatCity(line);
        cities[ct.src] = ct;
    }
    return n;
}

int totalNumsOfDest(char company) {
    FILE *fi;
    if(company == 'x' || company == 'X') {
        fi = fopen(DATA_X_AIRLINE, "r");
    } 
    if(company == 'y' || company == 'Y') {
        fi = fopen(DATA_Y_AIRLINE, "r");
    }
    int total = 0;
    int nums = readFile(fi);
    for(int i = 1; i <= nums; ++i) {
        // printf("%d", cities[i].numsOfDest);
        total += cities[i].numsOfDest;
    }
    fclose(fi);
    return total;
}

void rewriteFile(int n, char company) {
    FILE *fo;
    if(company == 'x' || company == 'X') {
        fo = fopen(DATA_X_AIRLINE, "w+");
    } 
    if(company == 'y' || company == 'Y') {
        fo = fopen(DATA_Y_AIRLINE, "w+");
    }
    fprintf(fo, "%d\n", n);
    for(int i = 1; i <= n; i++) {
        fprintf(fo, "%d   ", i);
        showList(fo, cities[i].dest);
        fprintf(fo, "\n");
    }
    fclose(fo);
    // printf("%d\n", n);
    // for(int i = 1; i <= n; ++i) {
    //     printf("%d   ", i);
    //     show(cities[i].dest);
    //     printf("\n");
    // }
}

void addRoute(int A, int B, int company) {
    FILE *fi;
    if(company == 'x' || company == 'X') {
        fi = fopen(DATA_X_AIRLINE, "r");
    }
    if(company == 'y' || company == 'Y') {
        fi = fopen(DATA_Y_AIRLINE, "r");
    }
    int n = readFile(fi);
    fclose(fi);
    if(A > n || A < 1) {
        printf("[ERROR] Thanh pho %d khong ton tai\n", A);
        return ;
    }
    if(B > n || B < 1) {
        printf("[ERROR] Thanh pho %d khong ton tai\n", B);
        return ;
    }
    if(findNode(cities[A].dest, B) == 1) {
        printf("[ERROR] Tuyen duong da ton tai\n");
        return ;
    }
    addNode(&cities[A].dest, B);
    printf("[%c-AIRLINE] Da them thanh cong tuyen duong bay tu %d den %d\n", toupper(company), A, B);
    rewriteFile(n, company);
    return ;
}

void deleteRoute(int A, int B, char company) {
    FILE *fi;
    if(company == 'x' || company == 'X') {
        fi = fopen(DATA_X_AIRLINE, "r");
    }
    if(company == 'y' || company == 'Y') {
        fi = fopen(DATA_Y_AIRLINE, "r");
    }
    int n = readFile(fi);
    fclose(fi);
    if(A > n || A < 1) {
        printf("[ERROR] Thanh pho %d khong ton tai\n", A);
        return ;
    }
    if(B > n || B < 1) {
        printf("[ERROR] Thanh pho %d khong ton tai\n", B);
        return ;
    }
    if(findNode(cities[A].dest, B) == 0) {
        printf("[ERROR] Tuyen duong bay khong ton tai\n");
        return ;
    }
    deleteNode(&cities[A].dest, B);
    printf("[%c-AIRLINE] Da xoa thanh cong tuyen duong bay tu %d den %d\n", toupper(company), A, B);
    rewriteFile(n, company);
    return ;
}

int costOfTrips(int numsOfCity, char company) {
    int cost = 0, airline_cost = 0;
    char currency;
    Node *trips = NULL;
    FILE *fi;
    if(company == 'x' || company == 'X') {
        fi = fopen(DATA_X_AIRLINE, "r");
        airline_cost = X_COST;
        currency = X_CURRENCY;
    }
    if(company == 'y' || company == 'Y') {
        fi = fopen(DATA_Y_AIRLINE, "r");
        airline_cost = Y_COST;
        currency = Y_CURRENCY;
    }
    int n = readFile(fi);
    fclose(fi);
    for(int i = 1; i <= numsOfCity; i++) {
        int city;
        do {
            printf("[4] Nhap thanh pho thu %d: ", i);
            scanf("%d", &city);
            if(city < 1 || city > n) {
                printf("Thanh pho khong hop le\n");
            }
            if(city == 'Q') return QUIT_CODE; // Lệnh QUIT
        } while(city < 1 || city > n);
        addNode(&trips, city);
    }
    Node *p = trips;
    int begin = p->value;
    p = p->next;
    while(p != NULL) {
        if(findNode(cities[begin].dest, p->value) == 0) {
            printf("[ERROR] Khong ton tai tuyen duong bay tu thanh pho %d toi thanh pho %d\n", begin, p->value);
            return NOTFOUND_CODE;
        }
        else cost += airline_cost;
        begin = p->value;
        p = p->next;
    }
    return cost;
}

void function1() {
    char company;
    fflush(stdin);
    printf("[1] Nhap ten hang may bay: ");
    scanf("%c", &company);
    if(company == 'x' || company == 'X' || company == 'y' || company == 'Y') {
        printf("So luong tuyen duong bay cua hang %c la: %d\n", toupper(company), totalNumsOfDest(company));
    } else {
        printf("[ERROR] Nhap sai ten hang may bay\n");
    }
    return ;
}

void function2() {
    char company;
    fflush(stdin);
    printf("[2] Nhap ten hang may bay can xoa tuyen duong: ");
    scanf("%c", &company);
    if(company == 'Q' || company == 'q') return ;
    if(company == 'x' || company == 'X' || company == 'y' || company == 'Y') {
        int A, B;
        printf("[2] Nhap thanh pho xuat phat: "); scanf("%d", &A);
        printf("[2] Nhap thanh pho den      : "); scanf("%d", &B);
        deleteRoute(A, B, company);
    } else {
        printf("[ERROR] Nhap sai ten hang may bay\n");
    }
}

void function3() {
    char company;
    fflush(stdin);
    printf("[3] Nhap ten hang may bay: ");
    scanf("%c", &company);
    if(company == 'Q' || company == 'q') return ;
    if(company == 'x' || company == 'X' || company == 'y' || company == 'Y') {
        int A, B;
        printf("[3] Nhap thanh pho di : "); scanf("%d", &A);
        printf("[3] Nhap thanh pho den: "); scanf("%d", &B);
        addRoute(A, B, company);
    } else {
        printf("[ERROR] Nhap sai ten hang may bay\n");
    }
}

void function4() {
    char company;
    fflush(stdin);
    printf("[4] Nhap ten hang may bay: ");
    scanf("%c", &company);
    if(company == 'Q' || company == 'q') return ;
    if(company == 'x' || company == 'X' || company == 'y' || company == 'Y') {
        int nc; 
        printf("[4] Nhap so thanh pho trong tuyen duong: "); scanf("%d", &nc);
        if(nc <= 2) {
            printf("[ERROR] So luong thanh pho phai tu 2 tro len\n");
            return ;
        }
        int cost = costOfTrips(nc, company);
        if(cost == QUIT_CODE || cost == NOTFOUND_CODE) return ;
        char currency = (company == 'x' || company == 'X') ? X_CURRENCY : Y_CURRENCY;
        printf("[%c-AIRLINE] Tong chi phi: %d %c\n", toupper(company), cost, currency);
    } else {
        printf("[ERROR] Nhap sai ten hang may bay\n");
    }
}