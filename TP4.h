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

Position *init_Position();
Position *init_Noeud();
Position *init_Index();