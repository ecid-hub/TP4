#ifndef TP3_H
#define TP3_H

#include "TP4.h"

typedef struct t_Position
{
    int numeroLigne;
    int ordre;
    int numeroPhrase;
    struct t_Position *suivant;
} Position;

typedef struct t_Noeud
{
    char *mot;
    int nbOccurence;
    Position *listePositions;
    struct t_Noeud *gauche;
    struct t_Noeud *droit;
} Noeud;

typedef struct t_Index
{
    Noeud *racine;
    int nbMotsDistincts;
    int nbMotsTotal;
} Index;

Position *init_Position(int ligne, int ordre, int phrase);
Noeud *init_Noeud(char *mot);
Index *init_Index();

void free_Index(Index **index);
void free_Noeud(Noeud *node);
void free_Position(Position *pos);

// Fonctions de base
void ajouterOccurence(Index *index, char *mot, int ligne, int ordre, int phrase);
void ajouterPosition(Noeud *mot, int ligne, int ordre, int phrase);

int indexerFichier(Index *index, char const *filename);

Noeud *rechercherMot(Index index, char *mot);
void afficherOccurencesMot(Index index, char *mot);
void construireTexte(Index index, char *filename);

// Fonctions ajoutées

int compare(char *mot1, char *mot2);
char into_minsucule(const char c);
void toLower(char *word);
char *trim(char *str);

// Fonctions ajoutées pour le debogage

void afficherPositions(Position *liste);
void afficherArbre(Noeud *noeud, int niveau);
void afficherIndex(Index *index);

#endif
