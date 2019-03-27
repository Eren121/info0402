#ifndef FILE_H
#define FILE_H

typedef void* ELEMENT_FILE;

typedef struct CelluleFile {

    ELEMENT_FILE val;
    struct CelluleFile* suivant;

} CELLULE_FILE;

typedef struct {

    struct CelluleFile *tete;
    struct CelluleFile *queue;

} FIFO;

FIFO* CreerFile();
ELEMENT_FILE Defiler(FIFO* file);
void Enfiler(FIFO* file, ELEMENT_FILE valeur);
int FileEstVide(FIFO* file);
void DetruireFile(FIFO* file);
void AfficherFile(FIFO* file);

#endif
