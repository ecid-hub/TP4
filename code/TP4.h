#ifndef TP3_H
#define TP3_H

#include <stdio.h>
#include <stdbool.h>

// Indexation primaire
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

// Indexation secondaire
typedef struct t_Mot
{
    Noeud *noeud;
    bool lineEnd;
    struct t_Mot *suivant;
} Mot;

typedef struct t_Phrase
{
    struct t_Phrase *suivante;
    Mot *mots;

} Phrase;

typedef struct t_Index
{
    Noeud *racine;
    int nbMotsDistincts;
    int nbMotsTotal;
    int nbPhrasesTotal;
    int nbLignes;
    Phrase *phrases;
} Index;

// Fonctions de gestion des struct 

Position *init_Position(int ligne, int ordre, int phrase);
Noeud *init_Noeud(char *mot);
Index *init_Index();

void free_Index(Index **index);
void free_Noeud(Noeud *node);
void free_Position(Position *pos);

// Fonctions de base

Noeud *ajouterOccurence(Index *index, char *mot, int ligne, int ordre, int phrase);
void ajouterPosition(Noeud *mot, int ligne, int ordre, int phrase);

int indexerFichier(Index *index, char const *sourceFile);

Noeud *rechercherMot(Index *index, const char *mot);
void afficherOccurencesMot(Index *index, const char *mot);
void afficher_caracteristiques_Index(Index *index);
void construireTexte(Index *index, const char *destFile);

// Fonctions ajoutées

int compare(const char *mot1, const char *mot2);
char char_low(const char c);
char char_cap(const char c);
void str_low(char *str);
void str_cap(char *str);
void str_trim(char *str);
Noeud *get_node(Noeud *node, const char *mot);

Phrase *init_Phrase();
void add_Phrase(Index *index, Phrase *sent);
Phrase *get_Phrase(Index *index, int number);
Mot *init_Mot(Noeud *node);
Mot *add_Mot(Phrase *sent, Noeud *node);

void parseSentences(Phrase *sent, FILE *handle);
void parseWords(Mot *word, FILE *handle);

// Fonctions ajoutées pour le debogage

void afficherPositions(Position *liste);
void afficherArbre(Noeud *noeud, int niveau);

void afficherPhrase(Phrase *sent);
void afficherMotRecurs(Mot *word);
void renderPlantUML(Noeud *node, int level, FILE *handle);

#endif
