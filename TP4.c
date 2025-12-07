#include "TP4.h"
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 200

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
    res->racine = NULL;
    return res;
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

char into_minsucule(char c)
{
    // Je transforme les majuscules en minuscules et laisse les minuscules comme elles sont*
    if (c <= 'Z' && c >= 'A')
    {
        return c + 'a' - 'A';
    }
    return c;
}

void toLower(char *mot)
{
    for (int i = 0; mot[i]; i++)
    {
        if (mot[i] != '\0')
        {
            mot[i] = into_minsucule(mot[i]);
        }
    }
}

char *trim(char *str)
{
    char *buff = malloc(sizeof(char) * 300);
    int indexBuff = 0;
    int i = 0;
    char *ptr = str;
    while (*ptr != '\0')
    {
        if (*ptr != ' ' && *ptr != '\n' && *ptr != '.')
        {
            buff[indexBuff++] = *ptr;
        }
        ptr++;
    }
    buff[indexBuff++] = '\0';
    buff = realloc(buff, sizeof(char) * indexBuff);
    return buff;
}

int compare(char *mot1, char *mot2)
{
    char *c1 = mot1;
    char *c2 = mot2;

    while (into_minsucule(*c1) != '\0' && into_minsucule(*c2) != '\0' && into_minsucule(*c1) == into_minsucule(*c2))
    {
        c1++;
        c2++;
    }
    if (into_minsucule(*c1) == '\0' && into_minsucule(*c2) == '\0')
    {
        return 0;
    }
    if (into_minsucule(*c1) == '\0')
    {
        return -1;
    }
    if (into_minsucule(*c2) == '\0')
    {
        return 1;
    }
    if (into_minsucule(*c1) < into_minsucule(*c2))
    {
        return -1;
    }
    if (into_minsucule(*c1) > into_minsucule(*c2))
    {
        return 1;
    }

    return 0;
}

void ajouterOccurence(Index *index, char *mot, int ligne, int ordre, int phrase)
{
    if (index == NULL)
    {
        printf("NULL INDEX\n");
        return;
    }

    Noeud *courant = index->racine;
    Noeud *pred = NULL;

    if (courant == NULL)
    {
        Noeud *node = init_Noeud(mot);
        ajouterPosition(node, ligne, ordre, phrase);
        index->racine = node;
        index->nbMotsTotal++;
        index->nbMotsDistincts++;
        return;
    } // On gère le cas où l'arbre est vide

    while (courant != NULL)
    {
        // Si le mot existe déjà dans l'indexe on s'arrête quand on est dedans, sinon on s'arrête quand on est sur un feuille
        int comp = compare(courant->mot, mot);

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
            return; // On eut directement mettre fin à la fonction dans cas.
        }
    } // Ici, nous sommes arrivés au moment où pred est une feuille.

    Noeud *n = init_Noeud(mot);
    ajouterPosition(n, ligne, ordre, phrase);
    index->nbMotsTotal++;
    index->nbMotsDistincts++;
    if (compare(pred->mot, mot) == 1)
    {
        // Alors on insère le nouveau noeud à gauche
        pred->gauche = n;
    }
    else
        pred->droit = n; // La cas d'égalité a déjà été traité plus haut
}

int indexerFichier(Index *index, char const *filename)
{
    const char *separators = " ";

    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_LENGTH];
    int ligne = 0;

    while (1)
    {
        char *ret = fgets(line, sizeof(char) * MAX_LINE_LENGTH, file); // on lit la ligne brute
        if (ret == NULL)
        {
            break; // EOF REACHED
        }

        int ordre = 0;
        int phrase = 0;
        char *tok = strtok(ret, separators);
        while (tok != NULL)
        {
            char *hasPoint = strchr(tok, '.');

            char *buff = trim(tok);
            ajouterOccurence(index, buff, ligne, ordre, phrase);

            if (hasPoint != NULL)
            {
                phrase++;
            }
            tok = strtok(NULL, separators);
            ordre++;
        }
        free(tok);
        ligne++;
    }
    // une ligne ne doit pas faire plus de MAX_LINE_LENGTH char

    return 1;
}

// Fonctions d'affichage pour le débogage
void afficherPositions(Position *liste)
{
    Position *courant = liste;
    while (courant != NULL)
    {
        printf("(l:%d, o:%d, p:%d)",
               courant->numeroLigne, courant->ordre, courant->numeroPhrase);
        courant = courant->suivant;
        if (courant != NULL)
            printf("->");
    }
    printf("\n");
}

void afficherArbre(Noeud *noeud, int niveau)
{
    if (noeud == NULL)
        return;

    afficherArbre(noeud->droit, niveau + 1);

    for (int i = 0; i < niveau; i++)
        printf("    ");
    printf("'%s' (occ:%d) :   ", noeud->mot, noeud->nbOccurence);

    // for (int i = 0; i < niveau; i++)
    //     printf("    ");
    // printf("  Positions: ");
    afficherPositions(noeud->listePositions);

    afficherArbre(noeud->gauche, niveau + 1);
}

void afficherIndex(Index *index)
{
    printf("\n========== INDEX ==========\n");
    printf("Nombre total de mots: %d\n", index->nbMotsTotal);
    printf("Nombre de mots distincts: %d\n\n", index->nbMotsDistincts);
    // afficherArbre(index->racine, 0);
    FILE *handle = fopen("tree.plantuml", "w");
    renderPlantUML(index->racine, 1, handle);
    fclose(handle);
    printf("===========================\n\n");
}

void renderPlantUML(Noeud *node, int level, FILE *handle)
{

    if (level == 1)
    {
        fprintf(handle, "@startmindmap index\n");
        fprintf(handle, "top to bottom direction\n\n");
    }

    char stars[level + 1];
    for (int i = 0; i < level; i++)
    {
        stars[i] = '*';
    }
    stars[level] = '\0';

    if (node != NULL)
    {
        fprintf(handle, "%s:%s\n", stars, node->mot);
        fprintf(handle, "<code>\n");
        Position *cur = node->listePositions;
        while (cur != NULL)
        {
            fprintf(handle, "(l:%d, o:%d, p:%d)\n", cur->numeroLigne, cur->ordre, cur->numeroPhrase);
            cur = cur->suivant;
        }

        fprintf(handle, "</code>\n;\n");

        renderPlantUML(node->gauche, level + 1, handle);
        renderPlantUML(node->droit, level + 1, handle);
    }
    else
    {
        fprintf(handle, "%s %s\n", stars, "NULL");
    }

    if (level == 1)
    {
        fprintf(handle, "@endmindmap\n");
    }
}

// Programme de test
int main()
{
    Index *index = init_Index();

    indexerFichier(index, "animaux");

    // printf("Test 1: Ajout de différents mots\n");
    // ajouterOccurence(index, "chat", 1, 1, 1);
    // ajouterOccurence(index, "chien", 1, 2, 1);
    // ajouterOccurence(index, "oiseau", 2, 1, 1);
    // afficherIndex(index);

    // printf("Test 2: Ajout d'occurrences du même mot\n");
    // ajouterOccurence(index, "chat", 2, 3, 1);
    // ajouterOccurence(index, "chat", 3, 1, 2);
    // afficherIndex(index);

    // printf("Test 3: Ajout d'un mot déjà existant (même position)\n");
    // ajouterOccurence(index, "chat", 1, 1, 1); // Doublon
    // afficherIndex(index);

    // printf("Test 4: Construction d'un arbre plus complet\n");
    // ajouterOccurence(index, "arbre", 4, 1, 1);
    // ajouterOccurence(index, "maison", 4, 2, 1);
    // ajouterOccurence(index, "enfant", 5, 1, 2);
    // ajouterOccurence(index, "soleil", 5, 2, 2);
    // afficherIndex(index);

    // printf("Test 5: Vérification du comptage total\n");
    // printf("Nombre total de mots indexés: %d\n", index->nbMotsTotal);
    // printf("Nombre de mots distincts: %d\n", index->nbMotsDistincts);

    afficherIndex(index);
    free_Index(&index);

    return 0;
}
