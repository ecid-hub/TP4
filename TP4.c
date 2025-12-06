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
    res->nbOccurence = 1;

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

Position *ajouterPosition(Position *listeP, int ligne, int ordre, int phrase)
{
    Position *p = init_Position();
    p->numeroLigne = ligne;
    p->numeroPhrase = phrase;
    p->ordre = ordre;
    p->suivant = NULL;
    Position *pred = NULL;
    Position *courant = listeP;

    // Cas de liste vide
    if (listeP == NULL)
    {
        return p;
    } // Si la liste est vide on renvoie p

    // Cas d'insertion en tête
    if (listeP->numeroLigne > ligne || (listeP->numeroLigne == ligne && listeP->ordre > ordre))
    {
        p->suivant = listeP;
        return p;
    }

    // parcours de la liste
    while (courant != NULL)
    {
        // Condition d'insertion : on a trouvé un élément plus grand
        if (courant->numeroLigne > ligne || (courant->numeroLigne == ligne && courant->ordre > ordre))
        {
            p->suivant = courant;
            pred->suivant = p;
            return listeP;
        }

        if (courant->numeroLigne == ligne && courant->ordre == ordre)
        {
            return listeP; // Dans ce cas, on ne réinsère une position qui était déjà dans la liste ...
        }
        pred = courant;
        courant = courant->suivant; // Si on ne vérifie aucune conditions, on passe à celui d'après...
    }

    // insertion en fin de liste, si on arrive ici c'est que p est tout au bout de ligne
    pred->suivant = p;
    return listeP;
}

char into_minsucule(char c)
{ // Je transforme les majuscules en minuscules et laisse les minuscules comme elles sont*
    if (c <= 'Z' && c >= 'A')
    {
        c += 'a' - 'A';
    }
    return c;
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

int ajouterOccurence(Index *index, char *mot, int ligne, int ordre, int phrase)
{

    Noeud *courant = index->racine, *pred = NULL;
    if (courant == NULL)
    {
        Noeud *n = init_Noeud(mot);
        n->listePositions = ajouterPosition(NULL, ligne, ordre, phrase);
        index->racine = n;
        index->nbMotsTotal++;
        index->nbMotsDistincts++;
        return 1;
    } // On gère le cas où l'arbre est vide

    while (courant != NULL)
    { // Si le mot existe déjà dans l'indexe on s'arrête quand on est dedans, sinon on s'arrête quand on est sur un feuille
        if (compare(courant->mot, mot) == -1)
        { // Dans ce cas, notre mot se trouve à droite de l'arbre
            pred = courant;
            courant = courant->droit;
        }

        else
        {
            if (compare(courant->mot, mot) == 1)
            { // A l'inverse dans ce cas, c'est que notre mot doit être dans la partie gauche
                pred = courant;
                courant = courant->gauche;
            }

            else
            { // Dans ce cas, les deux mots sont identiques, donc inutile de créér unnouveau noeud
                index->nbMotsTotal++;
                courant->nbOccurence++;
                courant->listePositions = ajouterPosition(courant->listePositions, ligne, ordre, phrase);
                return 1; // On eut directement mettre fin à la fonction dans cas.
            }
        }

    } // Ici, nous sommes arrivés au moment où pred est une feuille.

    Noeud *n = init_Noeud(mot);
    n->listePositions = ajouterPosition(NULL, ligne, ordre, phrase);
    index->nbMotsTotal++;
    index->nbMotsDistincts++;
    if (compare(pred->mot, mot) == 1)
    { // Alors on insère le nouveau noeud à gauche
        pred->gauche = n;
    }
    else
        pred->droit = n; // La cas d'égalité a déjà été traité plus haut
    return 1;
}

// Fonctions d'affichage pour le débogage
void afficherPositions(Position *liste)
{
    Position *courant = liste;
    while (courant != NULL)
    {
        printf("      (ligne:%d, ordre:%d, phrase:%d)",
               courant->numeroLigne, courant->ordre, courant->numeroPhrase);
        courant = courant->suivant;
        if (courant != NULL)
            printf(" -> ");
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
    printf("'%s' (occ:%d)\n", noeud->mot, noeud->nbOccurence);

    for (int i = 0; i < niveau; i++)
        printf("    ");
    printf("  Positions: ");
    afficherPositions(noeud->listePositions);

    afficherArbre(noeud->gauche, niveau + 1);
}

void afficherIndex(Index *index)
{
    printf("\n========== INDEX ==========\n");
    printf("Nombre total de mots: %d\n", index->nbMotsTotal);
    printf("Nombre de mots distincts: %d\n\n", index->nbMotsDistincts);
    afficherArbre(index->racine, 0);
    printf("===========================\n\n");
}

// Programme de test
int main()
{
    Index *index = init_Index();

    printf("Test 1: Ajout de différents mots\n");
    ajouterOccurence(index, "chat", 1, 1, 1);
    ajouterOccurence(index, "chien", 1, 2, 1);
    ajouterOccurence(index, "oiseau", 2, 1, 1);
    afficherIndex(index);

    printf("Test 2: Ajout d'occurrences du même mot\n");
    ajouterOccurence(index, "chat", 2, 3, 1);
    ajouterOccurence(index, "chat", 3, 1, 2);
    afficherIndex(index);

    printf("Test 3: Ajout d'un mot déjà existant (même position)\n");
    ajouterOccurence(index, "chat", 1, 1, 1); // Doublon
    afficherIndex(index);

    printf("Test 4: Construction d'un arbre plus complet\n");
    ajouterOccurence(index, "arbre", 4, 1, 1);
    ajouterOccurence(index, "maison", 4, 2, 1);
    ajouterOccurence(index, "enfant", 5, 1, 2);
    ajouterOccurence(index, "soleil", 5, 2, 2);
    afficherIndex(index);

    printf("Test 5: Vérification du comptage total\n");
    printf("Nombre total de mots indexés: %d\n", index->nbMotsTotal);
    printf("Nombre de mots distincts: %d\n", index->nbMotsDistincts);

    return 0;
}
