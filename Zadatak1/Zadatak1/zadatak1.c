#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define M 50
#define MAXBOD 100.0

typedef struct student {
    char ime[M];
    char prez[M];
    int bod;
}_stud;

int Br(char* dat) {
    int i = 0;
    FILE* file = fopen(dat, "r");

    if (file == NULL) {
        printf("GRESKA!!!\n");
        return -1;
    }

    char buffer[M];
    while (fgets(buffer, M, file)) {
        i++;
    }

    fclose(file);
    return i;
}

int Ucitajstud(char* dat, _stud* studenti, int brS) {
    int i = 0;
    FILE* file = fopen(dat, "r");

    if (file == NULL) {
        printf("GRESKA!!!\n");
        return -1;
    }

    for (i = 0; i < brS; i++) {
        fscanf(file, "%s %s %d", studenti[i].ime, studenti[i].prez, &studenti[i].bod);
    }

    fclose(file);
    return 0;
}

int main() {
    int Brstud;
    int i;

    char* fp = "studenti.txt";
    Brstud = Br(fp);

    _stud* studenti = (_stud*)malloc(Brstud * sizeof(_stud));
    if (!studenti) {
        printf("Greska: Alokacija memorije nije uspjela.\n");
        return 1;
    }

    if (Ucitajstud(fp, studenti, Brstud) != 0) {
        printf("Greska: Ucitavanje studenata nije uspjelo.\n");
        return 1;
    }

    for (i = 0; i < Brstud; i++) {
        float rel = ((float)studenti[i].bod / MAXBOD) * 100.0;
        printf("%s %s %d %f\n", studenti[i].ime, studenti[i].prez, studenti[i].bod, rel);
    }

    return 0;
}
