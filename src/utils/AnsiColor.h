#ifndef ANSI_COLORS_H
#define ANSI_COLORS_H

#include <stdio.h>

/**
 *      Longueur d'une chaîne de caractères sans les codes de formatage ANSI (qui ne prennent pas de place à l'affichage)
 */
static int strlenAnsi(char* s) {

    int i = 0;

    while(*s != '\0') {

        if(*s == '\033') {

            while(*s != 'm') {

                if(*s == '\0') {

                    printf("Erreur strlenAnsi() : code ANSI mal formaté\n");
                    return i;
                }

                s++;
            }
        }
        else {

            i++;
        }

        s++;
    }

    return i;
}

#define DISABLE_COLORS 0

/* Code de couleur ansi */
#if DISABLE_COLORS
#define ANSI(code)
#else
#define ANSI(code)      "\033[" #code "m"
#endif

/* Couleur d'avant-plan */
#define FG(code, x)     ANSI(code) x ANSI(39)

/* Couleur d'arrière-plan */
#define BG(code, x)     ANSI(code) x ANSI(49)

#define RED(x)          FG(31, x)
#define GREEN(x)		FG(32, x)
#define DGREY(x)        FG(90, x)
#define BG_LRED(x)      BG(101, x)
#define BOLD(x)         ANSI(1) x ANSI(22)

/* Underligned */
#define UL(x) ANSI(4) x ANSI(24)

#endif /* ANSI_COLORS_H */
