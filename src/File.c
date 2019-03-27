#include "File.h"
#include <stdlib.h>
#include <stdio.h>

FIFO* CreerFile() {

    FIFO* file = malloc(sizeof(FIFO));
    file->tete = NULL;
    file->queue = NULL;

    return file;
}

void DetruireFile(FIFO* file) {

    while(!FileEstVide(file)) {

        Defiler(file);
    }

    free(file);
}

int FileEstVide(FIFO* file) {

    return file->tete == NULL;
}

ELEMENT_FILE Defiler(FIFO* file) {

    CELLULE_FILE* tete = file->tete;
    ELEMENT_FILE val = tete->val;

    file->tete = tete->suivant;
    free(tete);

    if(file->tete == NULL) {

        file->queue = NULL;
    }
    
    return val;
}

void Enfiler(FIFO* file, ELEMENT_FILE val) {

    CELLULE_FILE* queue = malloc(sizeof(CELLULE_FILE));
    queue->suivant = NULL;
    queue->val = val;

    if(file->queue != NULL) {

        file->queue->suivant = queue;
    }
    else {

        file->tete = queue;
    }

    file->queue = queue;
}

void AfficherFile(FIFO* file) {

    CELLULE_FILE* cel = file->tete;

    printf("Tete: ");

    while(cel != NULL) {

        printf("%d<-", cel->val);
        cel = cel->suivant;
    }

    printf("\n");
}

int TestFile() {

    FIFO* file = CreerFile();
    Enfiler(file, 1);
    Enfiler(file, 2);
    Enfiler(file, 3);

    AfficherFile(file);
    printf("%d\n", Defiler(file));
    AfficherFile(file);

    DetruireFile(file);
}
