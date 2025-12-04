#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "TP4.h"

Position *init_Position()
{
    Position *res = malloc(sizeof(Position));
    res->numeroLigne = 0;
    res->numeroPhrase = 0;
    res->ordre = NULL;
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
