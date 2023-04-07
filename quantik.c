#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "quantik.h"

/*
    fonction qui indique si le caractere en parametre
    est bien un objet du jeu
*/
int estPieceValide(char p) {
    int ret=0;
    if ((p==(char)cylindre) ||
        (p==(char)cube) ||
        (p==(char)cone) ||
        (p==(char)sphere) ) ret=1;
    return ret;
}

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
char* getNom(char* dest, int size) {
    int i = 0;
    int c = getchar();
    /* on saute les espaces du debut */
    while (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
        c = getchar();
    }
    while (i < size && c != '\n' && c != '\r' && c != EOF) {
        dest[i] = (char)c;
        i++;
        c = getchar();
    }
    /* on ajoute la fin de chaine et on enleve les espaces de la fin */
    do {
        dest[i] = '\0';
        i--;
    } while (i >= 0 && (dest[i] == ' ' || dest[i] == '\t'));
    /* on consomme le reste de la ligne */
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
    return dest;
}

/*
   affiche la grille de jeu
*/
void afficheGrille(partie mapartie)
{
    int i,j;
    char show;
    /* affichage de du titre */
    printf("%s %s %s %d %s %s %s %d %s","\nJoueur 1 [",mapartie.nomjoueur[joueur1],"] : ",
           mapartie.nbcoupjoueur[joueur1],
           " -- Joueur 2 [",mapartie.nomjoueur[joueur2],"] : ",
           mapartie.nbcoupjoueur[joueur2],"\n");

    printf("%s","\n  ");
    for (j=0;j<MAX_C;j++) {
        printf("%s%d",j==(MAX_C/2) ? " || " : " | ",j);
    }
    printf(" |\n");

    for(i=0;i<MAX_L;i++) {
        if (i==(MAX_L/2) ){
            printf(" ==");
            for (j=0;j<MAX_C;j++) {
                printf("%s",j==(MAX_C/2) ? "++===":"+===");
            }
            printf("+\n");
        } else {
            printf(" --");
            for (j=0;j<MAX_C;j++) {
                printf("%s",j==(MAX_C/2) ? "++---":"+---");
            }
            printf("+\n");
        }
        printf("%2d",i);
        for (j=0;j<MAX_C;j++) {
            show  = (mapartie.grille[joueur1][i][j] == aucune) ? toupper((char)mapartie.grille[joueur2][i][j]) : (char)mapartie.grille[joueur1][i][j];
            printf("%s%c",j==(MAX_C/2) ? " || ":" | ",show);
        }
        printf(" |\n");
    }
    printf(" --");
    for (j=0;j<MAX_C;j++) {
        printf("%s",j==(MAX_C/2) ? "++---":"+---");
    }
    printf("+\n");
}

/* fonction qui prend en paramètre une partie et un joueur, puis affiche les pièces restantes pour ce joueur. Il parcourt les pièces de ce joueur dans la partie et affiche pour chaque pièce le type de pièce, le nombre de pièces restantes et un point-virgule. Enfin, il affiche une nouvelle ligne pour finir. */
void afficherPiecesRestantes(partie mapartie, joueurs joueur) {
	printf("\n%s\n", "Pieces restante :");

	for(int i = 0; i < NB_PIECE; i++) {
		printf("%c %s %d%s ", mapartie.piecesJoueur[joueur][i].piece , ":" , mapartie.piecesJoueur[joueur][i].nombre, ";");		
	}
	printf("\n");
}

// fonction qui affiche l'historique des coups passés par les 2 joueurs
void afficherListeCoups(partie mapartie) {

	int nbCoups = mapartie.nbcoupjoueur[joueur1] + mapartie.nbcoupjoueur[joueur2];

	printf("\n%s\n", "Historique des coups :");
	for(int i = 0; i < nbCoups; i ++) {

		coup coup = mapartie.coups[i];
		char * joueur = mapartie.nomjoueur[coup.joueur];	

		printf("%s %s %c %s %d %s %d %s\n", joueur, ":", coup.piece, "ligne :", coup.ligne, "colonne :", coup.colonne, ";" );
	}
}

/*
  initialise la structure principale d'une partie
*/
void initialise(partie * mapartie) {

	mapartie->finit = 0;
	mapartie->gagnant = -1;
	mapartie->nbcoupjoueur[joueur1] = 0;
	mapartie->nbcoupjoueur[joueur2] = 0;
	
    int j,l,c;

    printf("%s\n","\n****** Quantik ******** \n");
    for (j=joueur1; j<=joueur2; j++) {
        for(l=0;l<MAX_L;l++) {
            for (c=0;c<MAX_C;c++) {
                mapartie->grille[j][l][c]=aucune;
            }
        }

        //chaque joueur a 2 cylindres, 2 sphères, 2 cylindres et 2 cubes à leur disposition pour jouer
		mapartie->piecesJoueur[j][0].piece = cone;
		mapartie->piecesJoueur[j][0].nombre = 2;

		mapartie->piecesJoueur[j][1].piece = sphere;
		mapartie->piecesJoueur[j][1].nombre = 2;

		mapartie->piecesJoueur[j][2].piece = cylindre;
		mapartie->piecesJoueur[j][2].nombre = 2;

		mapartie->piecesJoueur[j][3].piece = cube;
		mapartie->piecesJoueur[j][3].nombre = 2;

        printf("%s %d %s\n","Entrez le joueur ",j+1," : ");
        getNom(mapartie->nomjoueur[j], MAX_NOMJOUEUR);

        mapartie->nbcoupjoueur[j]=0;
    }
    printf("\n%s %s %s\n","--> Le joueur 1 [",mapartie->nomjoueur[joueur1],"] commence avec les blancs (affichage des pieces en minuscule)");
    printf("%s %s %s\n","--> Le joueur 2 [",mapartie->nomjoueur[joueur2],"] joue avec les noirs (affichage des pieces en majuscule)");
}

/*
   lance une partie
*/
void jouePartie(void)
{
    partie mapartie;

    /* initialise la partie */
    initialise(&mapartie);

    /* affiche la grille */
    afficheGrille(mapartie);


	int ligneJoue;
	int colonneJoue;
	
    // Pour chaque joueur, on appelle la fonction joueurJoue() pour effectuer son coup, on affiche ensuite la grille mise à jour avec la fonction afficheGrille(). Ensuite, on teste si le joueur a gagné avec la fonction joueurAGagne(). Si le joueur a gagné, on enregistre le gagnant dans la structure mapartie, on met fin à la partie et on sort de la boucle. Si le joueur n'a pas gagné, on passe au joueur suivant. Enfin, si aucun des joueurs n'a gagné et que le nombre de coups joués atteint le maximum, la partie est également finie.
    while(!mapartie.finit) {

		//joueur 1 joue
		joueurJoue(joueur1, &mapartie, &ligneJoue, &colonneJoue);
		afficheGrille(mapartie);

		//test si le joueur 1 a gagné
		if(joueurAGagne(joueur1, mapartie, ligneJoue, colonneJoue)) {
			mapartie.gagnant = joueur1;
			mapartie.finit = 1;
		} else {
			
			//joueur 2 joue
			joueurJoue(joueur2, &mapartie, &ligneJoue, &colonneJoue);
			afficheGrille(mapartie);

			//test si le joueur 2 a gagné
			if(joueurAGagne(joueur2, mapartie, ligneJoue, colonneJoue)) {
				mapartie.gagnant = joueur2;
				mapartie.finit = 1;
			}
			
			//test si plus de coup possible
			if(mapartie.nbcoupjoueur[joueur1] + mapartie.nbcoupjoueur[joueur2] >= MAX_COUP) {
				mapartie.finit = 1;
			}
		}
	}

    //détermine quel joueur a gagné la partie
	switch(mapartie.gagnant){
		case joueur1 : printf("\n\n%s%s\n\n", mapartie.nomjoueur[joueur1], " a gagné");break;
		case joueur2 : printf("\n\n%s%s\n\n", mapartie.nomjoueur[joueur2], " a gagné");break;
		default : printf("\n\n%s\n\n", "Match nul");break;
	}

	afficherListeCoups(mapartie);
}

/* fonction qui vérifie si un joueur possède une pièce spécifique dans la partie en cours. La fonction prend en entrée le joueur, la partie en cours et la pièce à vérifier. Elle parcourt ensuite les pièces du joueur pour trouver celle qui correspond à la pièce donnée. Si le joueur possède au moins une pièce de ce type, la fonction renvoie 1, sinon elle renvoie 0. */
int joueurPossedePiece(joueurs joueur, partie mapartie, pieces piece) {

    for(int i = 0; i < NB_PIECE; i ++) {
        if(mapartie.piecesJoueur[joueur][i].piece == piece) {

            if(mapartie.piecesJoueur[joueur][i].nombre > 0) {
                return 1;
            } else {
                return 0;
            }
        }
    }

    return 0;
}

/* vérifie si un coup est valide dans le jeu en effectuant différentes vérifications telles que la validité de la pièce, la présence de pièces déjà jouées dans la ligne/colonne/zone, etc. Si toutes les conditions sont remplies, le coup est considéré comme valide et renvoie 1, sinon, renvoie 0. */
int estCoupValide(joueurs joueur, pieces piece, partie mapartie, int ligne, int colonne) {

	//index de l'adversaire
	int autreJoueur;
	if(joueur == 1) {
		autreJoueur = 0;
	} else {
		autreJoueur = 1;
	}

	//test si le character existe
	if(!estPieceValide(piece)) {
		printf("\n%s\n", "Piece invalide");
		return 0;
	}

	//test si le joueur a assez de piece
	if(!joueurPossedePiece(joueur, mapartie, piece)){
		printf("\n%s\n", "Pieces non possédé");
		return 0;
	}

	//test si index valide
	if(ligne < 0 || ligne >= MAX_L) {
		printf("\n%s\n", "Index ligne invalide");
		return 0;
	}
	if(colonne < 0 || colonne >= MAX_C) {
		printf("\n%s\n", "Index colonne invalide");
		return 0;
	}
	
	//test si place pour joueur
	if(mapartie.grille[0][ligne][colonne] != aucune || mapartie.grille[0][ligne][colonne] != aucune) {
		printf("\n%s\n", "Case deja pleine");
		return 0;
	}

	//test ligne
	for(int i = 0; i < MAX_L; i++) {
		if(mapartie.grille[autreJoueur][ligne][i] == piece) {
			printf("\n%s\n", "Piece deja joué dans la ligne");
			return 0;
		}
	}

	//test colonne
	for(int j = 0; j < MAX_C; j++) {
		if(mapartie.grille[autreJoueur][j][colonne] == piece) {
			printf("\n%s\n", "Piece deja joué dans la colonne");
			return 0;
		}
	}

	//test zone
	int tailleZone = 2;
	
	int minl = ligne/tailleZone * tailleZone;
	int minc = colonne/tailleZone * tailleZone;
		
	for(int l = minl; l < minl+tailleZone; l++) {
		for(int c = minc; c < minc+tailleZone; c++) {
			if(mapartie.grille[autreJoueur][l][c] == piece) {
				printf("\n%s\n", "Piece deja joué dans la zone");
				return 0;
			}
		}
	}
	
	//piece jouable
	return 1;
}

/* permet à un joueur de jouer un coup en saisissant une pièce et une position de la grille, puis vérifie si le coup est valide en appelant la fonction "estCoupValide". Si le coup est valide, la pièce est retirée des pièces du joueur et le coup est enregistré dans la partie. */
void joueurJoue(joueurs joueur, partie * mapartie, int * ligne, int  * colonne) {


	printf("\n%s%s\n", "-> Au tour de ",mapartie->nomjoueur[(int)joueur]);
	afficherPiecesRestantes(*mapartie, joueur);

	char charPiece;
	int jeuCorrect = 0;

	while(!jeuCorrect) {
		printf("\n%s", "piece joué : ");
		scanf(" %c", &charPiece);
		
		printf("\n%s", "ligne : ");
		scanf("%d", ligne);
		
		printf("\n%s", "colonne : ");
		scanf("%d", colonne);
		printf("\n");
		
		if(estCoupValide(joueur, charPiece, * mapartie, *ligne, *colonne)) {
			jeuCorrect = 1;
		} else {
			printf("%s\n", "---coup invalide---");
		}
	}

	retirePieceJoueur(mapartie, joueur, charPiece);

	mapartie->grille[joueur][*ligne][*colonne] = charPiece;
	mapartie->nbcoupjoueur[joueur] ++;

	enregistrerCoup(mapartie, joueur, charPiece, *ligne, *colonne);
}

/* Cette fonction retire une pièce du jeu d'un joueur donné dans une partie. Elle recherche la pièce dans les pièces du joueur et décrémente son nombre de 1. */
void retirePieceJoueur(partie * mapartie, joueurs joueur, pieces piece) {
	for(int i = 0; i < NB_PIECE; i++) {
		if(mapartie->piecesJoueur[joueur][i].piece == piece) {
			mapartie->piecesJoueur[joueur][i].nombre --;
			return;
		}
	}
}

/* Fonction qui vérifie si un joueur a gagné la partie en vérifiant s'il a placé une pièce
qui complète une ligne, une colonne ou une zone du tableau de jeu.
Parametres d'entrée : 
- joueur Le joueur pour lequel on vérifie s'il a gagné.
- mapartie La partie en cours.
- ligne La ligne sur laquelle la pièce a été placée.
- colonne La colonne sur laquelle la pièce a été placée.
Resultat : 
- Renvoie 1 si le joueur a gagné, 0 sinon. */
int joueurAGagne(joueurs joueur, partie mapartie, int ligne, int colonne) {

	nombrePiece nombresPieces[NB_PIECE];
		
	//test ligne
	initCompteurPieces(nombresPieces);
	
	for(int j = 0; j < MAX_C; j++) {
		ajouteCompteurPieces(nombresPieces, mapartie.grille[joueur1][ligne][j]);
		ajouteCompteurPieces(nombresPieces, mapartie.grille[joueur2][ligne][j]);
	}

	if(testVictoire(nombresPieces)) {
		printf("%s","ligne");
		return 1;
	}

	//test colonne
	initCompteurPieces(nombresPieces);
	
	for(int i = 0; i < MAX_L; i++) {
		ajouteCompteurPieces(nombresPieces, mapartie.grille[joueur1][i][colonne]);
		ajouteCompteurPieces(nombresPieces, mapartie.grille[joueur2][i][colonne]);
	}

	if(testVictoire(nombresPieces)) {
		printf("%s","colonne");
		return 1;
	}

	//test zone
	initCompteurPieces(nombresPieces);
	
	int tailleZone = 2;
	
	int minl = ligne/tailleZone * tailleZone;
	int minc = colonne/tailleZone * tailleZone;
		
	for(int l = minl; l < minl+tailleZone; l++) {
		for(int c = minc; c < minc+tailleZone; c++) {
			ajouteCompteurPieces(nombresPieces, mapartie.grille[joueur1][l][c]);
			ajouteCompteurPieces(nombresPieces, mapartie.grille[joueur2][l][c]);
		}
	}

	if(testVictoire(nombresPieces)) {
		printf("%s","zone");
		return 1;
	}

	return 0;
}

/* initialise un tableau de structure "nombrePiece" avec les noms des pièces et un compteur initialisé à zéro pour chaque pièce.
 */
void initCompteurPieces(nombrePiece *nombrepiece) {
	nombrepiece[0].piece = cylindre;
	nombrepiece[0].nombre = 0;

	nombrepiece[1].piece = cube;
	nombrepiece[1].nombre = 0;
	
	nombrepiece[2].piece = cone;
	nombrepiece[2].nombre = 0;

	nombrepiece[3].piece = sphere;
	nombrepiece[3].nombre = 0;
}

/* test si un joueur a gagné une partie de jeu en vérifiant si toutes les pièces dans une ligne, une colonne ou une zone sont identiques. Elle prend en entrée un pointeur vers une structure nombrePiece qui contient le nombre de chaque pièce et retourne 1 si le joueur a gagné et 0 sinon. */
int testVictoire(nombrePiece * nombrepiece) {

	for(int i = 0; i < NB_PIECE; i++) {
		if(nombrepiece[i].nombre != 1) {
			return 0;
		}
	}

	return 1;
}

/* permet d'incrémenter le compteur d'une pièce dans un tableau de structures de type "nombrePiece", en parcourant le tableau à la recherche de la pièce correspondante et en augmentant son compteur d'une unité si elle est trouvée. */
void ajouteCompteurPieces(nombrePiece * nombresPieces, pieces piece) {
	for(int i = 0; i < NB_PIECE; i++) {
		if(nombresPieces[i].piece == piece) {
			nombresPieces[i].nombre ++;
		}
	}
}

/* Cette fonction enregistre le coup joué dans la partie en mettant à jour le tableau de coups avec le joueur, la pièce, la ligne et la colonne du coup. L'index du coup est calculé à partir du nombre total de coups joués par les deux joueurs. */	
void enregistrerCoup(partie * mapartie, joueurs joueur, pieces piece, int ligne, int colonne) {
	int indexCoup = mapartie->nbcoupjoueur[joueur1] + mapartie->nbcoupjoueur[joueur2] -1;

	mapartie->coups[indexCoup].joueur = joueur;
	mapartie->coups[indexCoup].piece = piece;
	mapartie->coups[indexCoup].ligne = ligne;
	mapartie->coups[indexCoup].colonne = colonne;
}