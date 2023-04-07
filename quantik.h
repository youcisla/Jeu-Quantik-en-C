#ifndef QUANTIK_H_INCLUDED
#define QUANTIK_H_INCLUDED

#define MAX_L 4
#define MAX_C 4
#define MAX_COUP 16
#define MAX_NOMJOUEUR 50
#define NB_PIECE 4


typedef enum {aucune=' ',cylindre='y',cube='u',cone='o',sphere='s'} pieces ;
typedef enum {joueur1=0,joueur2=1} joueurs;

typedef struct {
	pieces piece;
	int ligne;
	int colonne;
	joueurs joueur;
} coup;

typedef struct {
	pieces piece;
	int nombre;
} nombrePiece;

typedef struct {
    pieces grille[2][MAX_L][MAX_C];   // les pieces des 2 joueurs
    char nomjoueur[2][MAX_NOMJOUEUR]; // les noms des 2  (au plus MAX_NOMJOUEUR-1 caracteres)
    int nbcoupjoueur[2];// nb de coups des 2 joueurs
	int finit;
	joueurs gagnant;
	coup coups[MAX_COUP];
    nombrePiece piecesJoueur[2][NB_PIECE];
} partie;

/*
    Fonction qui lit une chaine de caracteres sur le flux d'entree.
    Les espaces du debut sont supprimes, ainsi que ceux de la fin.

    La lecture s'arrete apres 
    - un retour chariot
    - une fin de fichier
    Au plus `size` caracteres sont mis dans la chaine de destination.
    
    Parametres : 
       - dest, chaine de caracteres de destination
       - size, taille maximale de la chaine de destination
    Resultat: 
       La chaine de destination
*/
char* getNom(char* dest, int size);

// procedure d'affichage de la grille (parametres a modifier selon votre structure de partie)
void afficheGrille(partie mapartie);

// procedure d'initialisation de dune partie
void initialise(partie * mapartie);

// procedure de creer, initialiser et jouer une partie
void jouePartie(void);

void joueurJoue(joueurs joueur, partie * mapartie, int * ligne, int  * colonne);

int estCoupValide(joueurs joueur, pieces piece, partie mapartie, int ligne, int colonne);

int joueurAGagne(joueurs joueur, partie mapartie, int ligneJoue, int colonneJoue);

int joueurPossedePiece(joueurs joueur, partie mapartie, pieces piece);

void initCompteurPieces(nombrePiece * nombrepiece) ;

int testVictoire(nombrePiece * nombrepiece);

void ajouteCompteurPieces(nombrePiece * nombresPieces, pieces piece);

void retirePieceJoueur(partie * mapartie, joueurs joueur, pieces piece);

void enregistrerCoup(partie * mapartie, joueurs joueur, pieces piece, int ligne, int colonne);

void afficherListeCoups(partie mapartie);

#endif // QUANTIK_H_INCLUDED