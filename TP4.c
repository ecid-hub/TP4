#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "TP4.h"

Position *init_Position()
{
    Position *res = malloc(sizeof(Position));
    res->numeroLigne = 0;
    res->numeroPhrase = 0;
    res->ordre = 0;
    res->suivant = NULL;
    return res;
}

Noeud *init_Noeud(char *mot)
{
    Noeud *res = malloc(sizeof(Noeud));
    res->droit = NULL;
    res->gauche = NULL;
    res->listePositions = NULL;
    res->mot = mot;
    res->nbOccurence = 0;

    return res;
}

Index *init_Index()
{
    Index *res = malloc(sizeof(Index));
    res->nbMotsDistincts = 0;
    res->nbMotsTotal = 0;
    res->racine = NULL;
    return res;
}

int compare(char *mot1, char *mot2)
{
    char *c1 = mot1;
    char *c2 = mot2;

    while (*c1 != '\0' && *c2 != '\0' && *c1 == *c2)
    {
        c1++;
        c2++;
    }
    if (*c1 == '\0' && *c2 == '\0')
    {
        return 0;
    }
    if (*c1 == '\0')
    {
        return -1;
    }
    if (*c2 == '\0')
    {
        return 1;
    }
    if (*c1 < *c2)
    {
        return -1;
    }
    if (*c1 > *c2)
    {
        return 1;
    }

    return 0;
}

char *toLower(const char *mot)
{
    char *res = mot;
    while (*res != '\0')
    {
        *res = tolower(*res);
        res++;
    }
    return res;
}

int main()
{
    printf("%s\n", toLower("test"));
    printf("%d\n", compare("abc", "abd"));
}