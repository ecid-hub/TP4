#include "TP4.h"

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