#include "TP4.h"
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 300

Position *init_Position(int ligne, int ordre, int phrase)
{
    Position *res = malloc(sizeof(Position));
    res->numeroLigne = ligne;
    res->ordre = ordre;
    res->numeroPhrase = phrase;
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
    res->nbPhrasesTotal = 0;
    res->nbLignes = 0;
    res->racine = NULL;
    res->phrases = NULL;
    return res;
}


Phrase *init_Phrase()
{
    Phrase *sent = malloc(sizeof(Phrase));
    sent->mots = NULL;
    sent->suivante = NULL;
    return sent;
}

Mot *init_Mot(Noeud *node)
{
    Mot *word = malloc(sizeof(Mot));
    word->noeud = node;
    word->lineEnd = false;
    word->suivant = NULL;
    return word;
}


void free_Index(Index **index)
{
    free_Noeud((*index)->racine);
    free(*index);
    *index = NULL;
}

void free_Noeud(Noeud *node)
{
    if (node != NULL)
    {
        free_Noeud(node->gauche);
        free_Noeud(node->droit);
        free(node->mot);
        free_Position(node->listePositions);
        free(node);
    }
}

void free_Position(Position *pos)
{
    if (pos != NULL)
    {
        free_Position(pos->suivant);
        free(pos);
    }
}

void ajouterPosition(Noeud *mot, int ligne, int ordre, int phrase)
{
    Position *p = init_Position(ligne, ordre, phrase);

    Position *pred = NULL;
    Position *courant = mot->listePositions;

    // parcours de la liste
    while (courant != NULL && !(courant->numeroLigne > ligne || (courant->numeroLigne == ligne && courant->ordre > ordre)))
    {
        if (courant->numeroLigne == ligne && courant->ordre == ordre)
        {
            return; // Dans ce cas, on ne réinsère une position qui était déjà dans la liste ...
        }
        pred = courant;
        courant = courant->suivant; // Si on ne vérifie aucune conditions, on passe à celui d'après...
    }

    // si previous_elem est NULL, ça veut dire qu'on n'a pas itéré une seule fois dans le while au dessus (le previous_elem est mis à jour à chaque itération), ce qui donne de cas:
    // => on insère process en tête
    if (pred == NULL)
    {
        p->suivant = mot->listePositions;
        mot->listePositions = p;
        mot->nbOccurence++;
    }
    else
    {
        // sinon on insère process après la liste après la tête :
        // - soit à la fin
        // - soit en plein millieu
        // => dans tout les cas le même comportement est attendu
        pred->suivant = p;
        p->suivant = courant;
        mot->nbOccurence++;
    }
}



Noeud *ajouterOccurence(Index *index, char *mot, int ligne, int ordre, int phrase)
{
    if (index == NULL)
    {
        printf("NULL INDEX\n");
        return NULL;
    }

    Noeud *courant = index->racine;
    Noeud *pred = NULL;

    int comp;
    while (courant != NULL)
    {
        // Si le mot existe déjà dans l'indexe on s'arrête quand on est dedans, sinon on s'arrête quand on est sur un feuille
        comp = compare(courant->mot, mot);

        if (comp == -1)
        {
            // Dans ce cas, notre mot se trouve à droite de l'arbre
            pred = courant;
            courant = courant->droit;
        }
        else if (comp == 1)
        {
            // A l'inverse dans ce cas, c'est que notre mot doit être dans la partie gauche
            pred = courant;
            courant = courant->gauche;
        }
        else
        {
            // Dans ce cas, les deux mots sont identiques, donc inutile de créér unnouveau noeud
            index->nbMotsTotal++;
            ajouterPosition(courant, ligne, ordre, phrase);
            return courant; // On eut directement mettre fin à la fonction dans cas.
        }
    } // Ici, nous sommes arrivés au moment où pred est une feuille.

    Noeud *node = init_Noeud(mot);
    ajouterPosition(node, ligne, ordre, phrase);

    index->nbMotsTotal++;
    index->nbMotsDistincts++;
    if (pred == NULL)
    {
        index->racine = node;
    }
    else
    {
        if (comp == 1)
        {
            // Alors on insère le nouveau noeud à gauche
            pred->gauche = node;
        }
        else
        {
            pred->droit = node; // La cas d'égalité a déjà été traité plus haut
        }
    }
    return node;
}

int indexerFichier(Index *index, char const *filename)
{
    const char *separators = " "; // Il faudrait aussi prendre en compte les points, les virgules, les points virgules...

    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_LENGTH];
    int ligne = 0;
    int nb_mots = 0; // Je rajoute la variable qui compte le nombre de mots

    Phrase *currentSent = init_Phrase();
    Mot *added = NULL;
    while (1)
    {
        // une ligne ne doit pas faire plus de MAX_LINE_LENGTH char
        char *ret = fgets(line, sizeof(char) * MAX_LINE_LENGTH, file); // on lit la ligne brute
        if (ret == NULL)
        {
            break; // EOF REACHED
        }

        int ordre = 0;
        int phrase = 0;
        char *tok = strtok(ret, separators); // strok permet de découper la chaine de caractère avec separator
        while (tok != NULL)
        {
            char *hasPoint = strchr(tok, '.'); // Renvoie un pointeur vers la première occurence de '.'

            char *buff = malloc(strlen(tok) + 1);
            strcpy(buff, tok);
            str_trim(buff);
            str_low(buff);
            if (strlen(buff) != 0)
            {

                Noeud *node = ajouterOccurence(index, buff, ligne, ordre, phrase);
                added = add_Mot(currentSent, node);
            }

            if (hasPoint != NULL)
            {
                add_Phrase(index, currentSent);
                phrase++;
                currentSent = init_Phrase();
            }
            tok = strtok(NULL, separators); // On lit le prochain mot de la ligne ...
            ordre++;
        }
        free(tok);
        if (added != NULL)
        {
            added->lineEnd = true;
        }
        ligne++;
    }
    if (added != NULL)
    {
        added->lineEnd = false;
    }
    index->nbLignes = ligne;

    return nb_mots; // D' après la consigne, on doit renvoyer le nombre de mots lus.
}







void afficherOccurencesMot(Index *index, const char *mot)
{
    if (index == NULL)
        return;

    char *word = malloc(strlen(mot) + 1);
    strcpy(word, mot);
    str_trim(word);
    str_low(word);

    Noeud *node = rechercherMot(index, word);
    free(word);
    if (node != NULL)
    {
        printf("Mot = \"%s\"\n", node->mot);
        printf("Occurences = %d\n", node->nbOccurence);
        Position *pos = node->listePositions;
        for (int i = 0; i < node->nbOccurence; i++)
        {
            printf("| Ligne %d, mot %d : ", pos->numeroLigne, pos->ordre);
            Phrase *sent = get_Phrase(index, pos->numeroPhrase);
            afficherPhrase(sent);
            pos = pos->suivant;
        }
    }
}

void construireTexte(Index *index, const char *destFile)
{
    Phrase *sent = index->phrases;
    FILE *file = fopen(destFile, "w");
    parseSentences(sent, file);
    fclose(file);
}










