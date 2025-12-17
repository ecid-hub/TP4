#include "TP4.c"
#include <stdio_ext.h>

int main()
{
    int choix;
    char nom_fichier[256];
    Index *i = init_Index();
    int nb_mots = 0;
    bool fichier_charge = false; // Variable pour savoir si un fichier a été chargé

    do
    {
        printf("\n=== Menu === \n");
        printf("1. Charger un fichier\n");
        printf("2. Caracteristiques de l'index\n");
        printf("3. Afficher index\n");
        printf("4. Rechercher un mot\n");
        printf("5. Afficher les occurences d un mot\n");
        printf("6. Construire le texte a partir de l index\n");
        printf("7. Generer PlantUML\n");
        printf("8. Quitter\n");
        printf("Votre choix : ");
        __fpurge(stdin);
        choix = 0;
        scanf("%d", &choix);

        switch (choix)
        {
        case 1: // Charger un fichier
            printf("Entrez le nom du fichier : ");
            scanf("%s", nom_fichier);
            nb_mots = indexerFichier(i, nom_fichier);
            if (nb_mots == -1)
            {
                printf("Erreur lors de l'ouverture du fichier\n");
                break;
            }
            fichier_charge = true; // Marquer que le fichier est chargé
            printf("Fichier charge avec succes (%d mots).\n", nb_mots);
            break;

        case 2: // Caracteristiques de l index
            if (!fichier_charge)
            {
                printf("Veuillez d'abord charger un fichier.\n");
            }
            else
            {
                afficher_caracteristiques_Index(i);
            }
            break;

        case 3: // Affichage de l'index
            if (!fichier_charge)
            {
                printf("Veuillez d'abord charger un fichier.\n");
            }
            else
            {
                afficherIndex(i);
            }
            break;

        case 4: // Rechercher un mot
            if (!fichier_charge)
            {
                printf("Veuillez d'abord charger un fichier.\n");
            }
            else
            {
                char mot[200];
                printf("Saisissez le mot a chercher : ");
                scanf("%s", mot);
                rechercherEtAfficherMot(i, mot);
            }
            break;

        case 5: // Afficher les occurences d'un mot
            if (!fichier_charge)
            {
                printf("Veuillez d'abord charger un fichier.\n");
            }
            else
            {
                char mot[200];
                printf("Saisissez le mot dont vous souhaitez voir les occurences : ");
                scanf("%s", mot);
                afficherOccurencesMot(i, mot);
            }
            break;

        case 6: // Créer le fichier
            if (!fichier_charge)
            {
                printf("Veuillez d'abord charger un fichier.\n");
            }
            else
            {
                char fichier_sortie[200];
                printf("Saisissez le nom du fichier qui sera cree : ");
                scanf("%s", fichier_sortie);
                construireTexte(i, fichier_sortie);
            }
            break;

        case 7:
            FILE *handle = fopen("tree.plantuml", "w");
            if (handle == NULL)
            {
                printf("Erreur lors de la creation de 'tree.plantuml'\n");
                break;
            }
            renderPlantUML(i->racine, 1, handle);
            fclose(handle);
            printf("UML genere dans 'tree.plantuml'\n");
            break;

        case 8: // Quitter
            free_Index(&i);
            printf("Au revoir!\n");
            break;

        default:
            printf("Choix invalide, veuillez rentrer un choix valide\n");
            break;
        }
    } while (choix != 8);

    return 0;
}