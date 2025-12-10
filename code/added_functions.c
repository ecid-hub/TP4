#include "TP4.h"


// Cette fonction tranforme toutes les lettres en minuscule
char char_low(char c)
{
    
    if (c <= 'Z' && c >= 'A')
    {
        return c + 'a' - 'A';
    }
    return c;
}


// Cette fonction transforme toutes les lettres en majuscules
char char_cap(const char c)
{
    if (c <= 'z' && c >= 'a')
    {
        return c + 'A' - 'a';
    }
    return c;
}


// Cette fonction transforme une chaine de caractère en chaine de carcateres minuscules
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


// Cette fonction transforme une chaîne de caractères en caractères majuscules
void str_cap(char *str)
{
    str[0] = char_cap(str[0]);
}


// Cette fonction prend une chaîne de caractère et lui retire tousles points et les expaces 
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


//Cette fonction permet de comparer deux caractères sans se soucier de la casse
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


// Cette fonction est une recherche dans un ABRà partir d'un noeud. On renvoie le noeud qui contient le mot recherché
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

// Cette fonction renvoie le noeud contenant le mot **mot** dans l'index **index**
Noeud *rechercherMot(Index *index, const char *mot)
{
    get_node(index->racine, mot);
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

Mot *add_Mot(Phrase *sent, Noeud *node)
{
    if (sent == NULL || node == NULL)
        return NULL;

    Mot *word = init_Mot(node);
    word->suivant = sent->mots;
    sent->mots = word;
    return word;
}


