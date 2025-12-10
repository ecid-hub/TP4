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

char char_low(char c)
{
    // Je transforme les majuscules en minuscules et laisse les minuscules comme elles sont*
    if (c <= 'Z' && c >= 'A')
    {
        return c + 'a' - 'A';
    }
    return c;
}

char char_cap(const char c)
{
    if (c <= 'z' && c >= 'a')
    {
        return c + 'A' - 'a';
    }
    return c;
}

void str_low(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        if (str[i] != '\0')
        {
            str[i] = char_low(str[i]);
        }
    }
}

void str_cap(char *str)
{
    str[0] = char_cap(str[0]);
}

void str_trim(char *str)
{
    char buff[300 * sizeof(char)];
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
    strcpy(str, buff);
}

int compare(const char *mot1, const char *mot2)
{
    const char *c1 = mot1;
    const char *c2 = mot2;

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
    int nb_mots = 0 ; // Je rajoute la variable qui compte le nombre de mots

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
            Noeud *node = ajouterOccurence(index, buff, ligne, ordre, phrase);
            added = add_Mot(currentSent, node);

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

Noeud *rechercherMot(Index *index, const char *mot)
{
    get_node(index->racine, mot);
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

void parseSentences(Phrase *sent, FILE *handle)
{
    if (sent != NULL)
    {
        parseSentences(sent->suivante, handle);
        parseWords(sent->mots, handle);
        fprintf(handle, ".");
    }
}

void parseWords(Mot *word, FILE *handle)
{
    if (word != NULL)
    {
        char *buff = word->noeud->mot;
        if (word->suivant == NULL)
        {
            buff = malloc(sizeof(char) * (strlen(word->noeud->mot) + 1));
            strcpy(buff, word->noeud->mot);
            str_cap(buff);
        }
        else
        {
            parseWords(word->suivant, handle);
        }
        fprintf(handle, " %s", buff);
        if (word->lineEnd)
        {
            fprintf(handle, "\n");
        }
    }
}

Noeud *get_node(Noeud *node, const char *mot)
{
    if (node == NULL)
    {
        return NULL;
    }
    int comp = compare(node->mot, mot);

    if (comp == -1)
    {
        // Dans ce cas, notre mot se trouve à droite de l'arbre
        get_node(node->droit, mot);
    }
    else if (comp == 1)
    {
        // A l'inverse dans ce cas, c'est que notre mot doit être dans la partie gauche
        get_node(node->gauche, mot);
    }
    else
    {
        // Dans ce cas, les deux mots sont identiques, donc inutile de créér unnouveau noeud
        return node;
    }
}

Phrase *init_Phrase()
{
    Phrase *sent = malloc(sizeof(Phrase));
    sent->mots = NULL;
    sent->suivante = NULL;
    return sent;
}

void add_Phrase(Index *index, Phrase *sent)
{
    if (index == NULL || sent == NULL)
    {
        return;
    }

    index->nbPhrasesTotal++;
    sent->suivante = index->phrases;
    index->phrases = sent;
}

Phrase *get_Phrase(Index *index, int number)
{
    if (index == NULL)
        return NULL;
    Phrase *curr = index->phrases;
    for (int i = 0; i < (index->nbPhrasesTotal - 1 - number); i++)
    {
        curr = curr->suivante;
    }
    return curr;
}

Mot *init_Mot(Noeud *node)
{
    Mot *word = malloc(sizeof(Mot));
    word->noeud = node;
    word->lineEnd = false;
    word->suivant = NULL;
    return word;
}

Mot *add_Mot(Phrase *sent, Noeud *node)
{
    if (sent == NULL || node == NULL)
        return NULL;

    Mot *word = init_Mot(node);
    word->suivant = sent->mots;
    sent->mots = word;
    return word;
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

void afficher_caracteristiques_Index(Index *index)
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

void afficherPhrase(Phrase *sent)
{
    if (sent != NULL)
    {
        afficherMotRecurs(sent->mots);
        printf(".\n");
    }
}

void afficherMotRecurs(Mot *word)
{
    if (word != NULL)
    {
        char *buff = word->noeud->mot;
        if (word->suivant == NULL)
        {
            buff = malloc(sizeof(char) * (strlen(word->noeud->mot) + 1));
            strcpy(buff, word->noeud->mot);
            str_cap(buff);
        }
        else
        {
            afficherMotRecurs(word->suivant);
        }
        printf(" %s", buff);
    }
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
//int main()
//{
 //   Index *index = init_Index();

 //   indexerFichier(index, "animaux");

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

   // afficher_caracteristiques_Index(index);
    // afficherArbre(rechercherMot(index, "oiseau"), 0);
    // afficherPhrase(index->phrases);
   // afficherOccurencesMot(index, "oiseau");
    //construireTexte(index, "test.zub");
    //free_Index(&index);

  //  return 0;
//}
