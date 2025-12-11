#include "TP4.h"

int main() {
    int choix;
    char nom_fichier[256];  
    Index *i = init_Index(); 
    int nb_mots = 0;
    int fichier_charge = 0;  // Variable pour savoir si un fichier a été chargé
    
    do {
        printf("\n=== Menu === \n");
        printf("1. Charger un fichier\n");
        printf("2. Caracteristiques de l'index\n");
        printf("3. Afficher index\n");
        printf("4. Rechercher un mot\n");
        printf("5. Afficher les occurences d un mot\n");
        printf("6. Construire le texte a partir de l index\n");
        printf("7. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        
        switch(choix) {
            case 1: // Charger un fichier 
                printf("Entrez le nom du fichier : ");
                scanf("%s", nom_fichier);  
                nb_mots = indexerFichier(i, nom_fichier);
                fichier_charge = 1;  // Marquer que le fichier est chargé
                printf("Fichier charge avec succes (%d mots).\n", nb_mots);
                break;
                
            case 2: // Caracteristiques de l index 
                if (!fichier_charge) {
                    printf("Veuillez d'abord charger un fichier.\n");
                } else {
                    afficher_caracteristiques_Index(i);  
                }
                break;
                
            case 3: // Affichage de l'index
                if (!fichier_charge) {
                    printf("Veuillez d'abord charger un fichier.\n");
                } else {
                    // afficherIndex(i);
                    printf("Fonction pas encore implementee.\n");
                }
                break; 
                
               case 4: // Rechercher un mot
    if (!fichier_charge) {
        printf("Veuillez d'abord charger un fichier.\n");
    } else { 
        char mot[200];
        printf("Saisissez le mot a chercher : ");
        scanf("%s", mot);
        rechercherEtAfficherMot(i, mot);
    }
    break;
                
            case 5: // Afficher les occurences d'un mot  
                if (!fichier_charge) {
                    printf("Veuillez d'abord charger un fichier.\n");
                } else { 
                    char mot[200];
                    printf("Saisissez le mot dont vous souhaitez voir les occurences : ");
                    scanf("%s", mot);  
                    afficherOccurencesMot(i, mot);
                }
                break; 
                
            case 6: // Créer le fichier 
                if (!fichier_charge) {
                    printf("Veuillez d'abord charger un fichier.\n");
                } else { 
                    char fichier_sortie[200];
                    printf("Saisissez le nom du fichier qui sera cree : ");
                    scanf("%s", fichier_sortie);  
                    construireTexte(i, fichier_sortie);
                }
                break; 
                
            case 7: // Quitter
                printf("Au revoir!\n");
                break;
                
            default:
                printf("Choix invalide, veuillez rentrer un choix valide\n");
                break; 
        }
    } while (choix != 7);
    
    return 0;
}