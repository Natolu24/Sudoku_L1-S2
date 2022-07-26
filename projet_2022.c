/*
> PROJET FINAL SUDOKU

> GROUPE :
- AGUESSE Nathan L1-Z 21001877
- PHOMMAHAXAY Caroline L1-Z 21005223
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#define N 9


int sans_conflit(int grille[N][N]) {
    int i,x,y;
    // Parcours de chaque ligne et colonne
    for (i = 0; i<N; i++) {
        // Parcours de chaque élément
        for (x = 0; x<N; x++) {
            // Parcours des autres élément APRES x (1 2 3 4 5 -> 2 3 4 5 / 3 4 5 -> 4 5)
            for (y = x+1; y<N; y++) { // y = x+1
                // Parcours de la ligne
                if (grille[i][x] != 0) {
                    if (grille[i][x] == grille[i][y]) return 1;
                }
                // Parcours de la colonne
                if (grille[x][i] != 0) {
                    if (grille[x][i] == grille[y][i]) return 1;
                }
            }
        }
    }
    return 0;
}


int sans_conflit_voisines(int grille[N][N]) {
    int i,j,x,y;
    //Parcours de chaque zones
    for (i = 0; i<N; i+=3) {
        for (j = 0; j<N; j+=3) {
            //Parcours de chaque élément
            for (x = 0; x<N; x++) {
                // Parcours des autres élément APRES x
                for (y = x+1; y<N; y++) { // y = x+1
                    // 'N/3' permet de se déplacer dans les lignes, et 'N%3' permet de se déplacer dans le colonnes
                    if (grille[i+x/3][j+x%3] != 0) {
                        if (grille[i+x/3][j+x%3] == grille[i+y/3][j+y%3]) return 1;
                    }
                }
            }
        }
    }
    return 0;
}

// On prédéfinie cette fonction, car 'charger' et 'lire_grille' s'apelle entre elle, donc il faut qu'elles soient toute les deux définie avant l'autre
void charger(int grille[N][N], char *nom_fichier);

void lire_grille(char *nom_fichier, int grille[N][N]) {
    FILE* fichier = fopen(nom_fichier, "r");
    int c, x = 0, y = 0;
    if (fichier != NULL) {
        while (c != EOF) {
            c = fgetc(fichier);
            if (c >= 49 && c <= 57) { // si on trouve un chiffre
                grille[x][y] = c-48; // on l'affecte a la grille
                y++; // on passe a l'élément suivant
            }
            else if (c == 95) { // si on tombe sur un '_'
                grille[x][y] = 0; // on affecte 0
                y++; // élément suivant
            }
            if (y == N) { // si on est arrivé a la fin de la ligne actuelle
                y = 0; // on reset y
                x++; // et on passe a la ligne suivante
            }
        }
    }
    fclose(fichier);
    if (sans_conflit(grille) == 1 || sans_conflit_voisines(grille) == 1) { // si erreur de conflit
        printf("\nErreur, le fichier n'est pas une grille de sudoku valide,\nVeuillez en chargez une autre...\n\n"); // Msg d'erreur
        charger(grille, nom_fichier); // on doit charger un autre fichier
    }
}


void afficher_grille(int grille[N][N]) {
    // Pleins de conditions et de printf, pour arriver a un affichage décent de la grille
    printf("\n\\ Y 0 1 2   3 4 5   6 7 8\n");
    printf("X |-------|-------|-------|\n");
    for (int x=0;x<N;x++) {
        if (x == 3||x == 6) printf("  |-------|-------|-------|\n%d | ", x);
        else printf("%d | ", x);
        for (int y=0;y<N;y++) {
            if (grille[x][y] != 0) printf("%d ", grille[x][y]);
            else printf("  ");
            if (y == 2||y == 5||y == 8) printf("| ");
        }
        printf("\n");
    }
    printf("  |-------|-------|-------|\n\n");
}


void ecrire_grille(int grille[N][N], char *nom) {
    char nom_fichier[75]; // futur nom de la sauvegarde
    char nom_tmp[50]; // nom que l'on va attribuer le nom d'origine
    sprintf(nom_tmp, "%s", nom); // on attribue d'abord le nom du fichier actuelle
    nom_tmp[strlen(nom_tmp)-4] = '\0'; // on enleve le '.txt'
    // SI le fichier actuelle était une sauvegarde, on enleve le '_saveX'
    if (nom_tmp[strlen(nom_tmp)-6]=='_' && nom_tmp[strlen(nom_tmp)-5]=='s' && nom_tmp[strlen(nom_tmp)-4]=='a' && nom_tmp[strlen(nom_tmp)-3]=='v' && nom_tmp[strlen(nom_tmp)-2]=='e') nom_tmp[strlen(nom_tmp)-6] = '\0';
    // De plus, si la sauvegarde dépasse 9 (nombre a 2 chiffres), il faut enlever un caractère depla : ('save_56' par exemple)
    if (nom_tmp[strlen(nom_tmp)-7]=='_' && nom_tmp[strlen(nom_tmp)-6]=='s' && nom_tmp[strlen(nom_tmp)-5]=='a' && nom_tmp[strlen(nom_tmp)-4]=='v' && nom_tmp[strlen(nom_tmp)-3]=='e') nom_tmp[strlen(nom_tmp)-7] = '\0';
    FILE* fichier;
    int i,x,y;
    for (i=1; i<100; i++) { // On parcours tout les '_saveX' de '_save1' à '_save99'
        sprintf(nom_fichier, "%s_save%d.txt", nom_tmp, i); // on attribue le nom de la sauvegarde a nom_fichier
        fichier = fopen(nom_fichier, "r"); // On ouvre le fichier temporairement seulement pour savoir si un fichier sauvegarde de ce nom existe déjà
        if (fichier == NULL) break; // S'il n'existe pas de fichier sauvegarde de ce nom, on sort de la boucle
        fclose(fichier);
    }
    fclose(fichier);
    fichier = fopen(nom_fichier, "w"); // Maintenant on peut ouvrir le fichier sauvegarde, en mode écriture, qui va donc créer un fichier
    for (x=0; x<N; x++) { // On parcours la grille
        for (y=0; y<N; y++) {
            // Et on attribue chaque chiffre
            if (grille[x][y] == 0) fputs("_ ", fichier); // En faisant attention de bien mettre un '_' lorsque c'est 0
            else fprintf(fichier, "%d ", grille[x][y]);  // Cas normal (on met toujours "%d " avec bien un espace pour rester en règle avec le format)
        }
        fputs("\n", fichier); // Et on fais attention a sauter a la ligne lorsqu'on arrive donc a la fin de chaque ligne
    }
    printf("\nVotre partie a etait bien sauvegarder,\nNom de la sauvegarde : %s\n", nom_fichier); // Msg de confirmation de sauvegarde
    fclose(fichier);
}


void afficher_liste_fichiers() {
    DIR* d;
    struct dirent *dir;
    d = opendir("."); // On ouvre le dossier ou le code se trouve
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            char nom[260];
            sprintf(nom, "%s", dir->d_name); // Puis pour chaque fichier, on attribue le nom de celui-ci a 'nom'
            if (nom[strlen(nom)-3] == 't' && nom[strlen(nom)-2] == 'x' && nom[strlen(nom)-1] == 't') { // On affiche que les fichier '.txt' pour éviter que le joueur voient des fichiers indésirables
                // Si le fichier est une sauvegarde, on l'affiche un peu différement pour que le joueur le remarque plus facilement
                if (nom[strlen(nom)-10]=='_' && nom[strlen(nom)-9]=='s' && nom[strlen(nom)-8]=='a' && nom[strlen(nom)-7]=='v' && nom[strlen(nom)-6]=='e') printf("-");
                // En faisant encore attention au sauvergarde a 2 chiffres ('_saveXX')
                else if (nom[strlen(nom)-11]=='_' && nom[strlen(nom)-10]=='s' && nom[strlen(nom)-9]=='a' && nom[strlen(nom)-8]=='v' && nom[strlen(nom)-7]=='e') printf("-");
                printf("> %s\n", nom); // Et on affiche le fichier
            }
        }
        closedir(d);
    }
}


void ajouter (int grille[N][N]){
    int x, y, nombre;
    printf("\nOu voulez-vous faire des changements? (Position X et Y)\n");
    scanf("%d %d", &x, &y); // On prend la position X Y
    if ((x>=0 && x<=8)&&(y>=0 && y<=8)){ // Si les position sont correctes
        printf("\nQuel nombre voulez vous mettre? \n");
        scanf("%d", &nombre); // On prend le chiffre
        if (nombre>=0 && nombre<=9){ // Si le chiffre est correcte
            if (grille[x][y] == 0) grille[x][y] = nombre; // S'il n'y a bien aucun chiffre sur cette case
            else printf("Un chiffre est deja sur cette case !\n"); // Sinon on affiche un msg d'erreur
        }
        else{
            printf("Veuillez mettre un chiffre correcte \n"); // Msg d'erreur
        }
    }
    else{
        printf("\nVeuillez choisir un chiffre entre 0 et 9, correspondant a la case voulu. \n"); // Msg d'erreur
    }
    if (sans_conflit(grille) == 1 || sans_conflit_voisines(grille) == 1) { // Si le chiffre que l'on a mit n'est pas en accord avec les règle du sudoku
        printf("Il y a deja ce chiffre dans cette ligne/colonne/zone !"); // Msg d'erreur
        grille[x][y] = 0; // Et on l'enleve donc
    }
}


void effacer (int grille[N][N], char *nom_fichier) {
    int x, y;
    char nom_tmp[50];
    int grille_origin[N][N]; // Pour avoir la grille d'origine
    sprintf(nom_tmp, "%s", nom_fichier); // On prend le nom du fichier actuelle que l'on met dans 'nom_tmp'
    // SI le nom est une sauvegarde a 1 chiffre, on enleve le '_saveX' tout en gardant '.txt'
    if (nom_tmp[strlen(nom_tmp)-10]=='_' && nom_tmp[strlen(nom_tmp)-9]=='s' && nom_tmp[strlen(nom_tmp)-8]=='a' && nom_tmp[strlen(nom_tmp)-7]=='v' && nom_tmp[strlen(nom_tmp)-6]=='e') {
        nom_tmp[strlen(nom_tmp)-10] = '.';
        nom_tmp[strlen(nom_tmp)-9] = 't';
        nom_tmp[strlen(nom_tmp)-8] = 'x';
        nom_tmp[strlen(nom_tmp)-7] = 't';
        nom_tmp[strlen(nom_tmp)-6] = '\0';
    }
    // SI le nom est une sauvegarde a 2 chiffre, on enleve le '_saveXX' tout en gardant '.txt'
    else if (nom_tmp[strlen(nom_tmp)-11]=='_' && nom_tmp[strlen(nom_tmp)-10]=='s' && nom_tmp[strlen(nom_tmp)-9]=='a' && nom_tmp[strlen(nom_tmp)-8]=='v' && nom_tmp[strlen(nom_tmp)-7]=='e') {
        nom_tmp[strlen(nom_tmp)-11] = '.';
        nom_tmp[strlen(nom_tmp)-10] = 't';
        nom_tmp[strlen(nom_tmp)-9] = 'x';
        nom_tmp[strlen(nom_tmp)-8] = 't';
        nom_tmp[strlen(nom_tmp)-7] = '\0';
    }
    // Ainsi on a bien obtenue le nom de la grille d'origine
    printf("\nOu voulez-vous effacer? (Position X et Y)\n");
    scanf("%d %d", &x, &y); // On prend les positions X Y ou le joueur veut effacer
    lire_grille(nom_tmp, grille_origin); // On lit la grille d'origine
    if (grille_origin[x][y] == 0) { // S'y aucun chiffre n'était la de base sur la grille d'origine
        grille[x][y] = 0; // On peut effacer
    }
    else {
        printf("\nVeuillez ne pas effacer un chiffre de base !\n"); // Msg d'erreur
    }
}


void charger(int grille[N][N], char *nom_fichier) {
    afficher_liste_fichiers(); // On affiche la liste des fichiers
    printf("\nQuel fichier voulait vous charger ?\n");
    scanf("%s", nom_fichier); // On prend le nom du fichier que le joueur veut charger
    lire_grille(nom_fichier, grille); // Et on le lit dans la grille
}


void quitter (int grille[N][N], char *nom){
    char validation;
    printf("Voulez-vous sauvegarder votre partie avant de quitter ? [o/n] \n");
    scanf("%c", &validation);
    scanf("%c", &validation); // Double scanf a cause d'un bug qui ne prend pas en compte le premier scanf
    if (validation == 'o'){ // On sauvegarde puis on quitte
        ecrire_grille(grille, nom);
        exit(0);
    }
    else if (validation == 'n'){ // On quitte juste
        exit(0);  
    }
    else{
        printf("Votre choix n'est pas bon. \n "); // Sinon on ne fait rien si le joueur n'a pas mit un choix correcte
    }
}


void choisir (char *choix){
    printf("\nQue faire?\n");
    printf("a = permet d'ajouter une valeur. \ne = permet d'effacer une case. \nc = permet de charger une partie. \ns = permet de sauvergarder une partie. \nq = permet de quitter une partie. \n");
    scanf("%c", choix);
    scanf("%c", choix); // Double scanf a cause d'un bug qui ne prend pas en compte le premier scanf
    if (1 != (*choix == 'a' || *choix == 'e' || *choix == 'c' || *choix == 's' || *choix == 'q')){ // Si le joueur ne met pas un choix correcte
        printf("Veuillez mettre quelque chose de possible\n"); // Msg d'erreur
        choisir(choix); // Il doit recommencer
    }
}


void action (char choix, int grille[N][N], char *nom_fichier){
    switch(choix) {
        case 'a': // Ajouter
            ajouter(grille);
            break;
        case 'e': // Effacer
            effacer(grille, nom_fichier);
            break;
        case 'c': // Charger
            charger(grille, nom_fichier);
            break;
        case 's': // Sauvegarder
            ecrire_grille(grille, nom_fichier);
            break;
        case 'q': // Quitter
            quitter(grille, nom_fichier);
            break;
    }
}   


int check_fin(int grille[N][N]) {
    // On parcours la liste, si on tombe sur une case sans chiffre on retourne 1, sinon 0
    for (int x=0; x<N; x++) {
        for (int y=0; y<N; y++) {
            if (grille[x][y] == 0) return 1;
        }
    }
    return 0;
}



int main() {
    int grille[N][N]; // Définie la grille
    char nom_fichier[50]; // Défini le nom du fichier actuelle (change a chaque fois que l'on charge un nouveau fichier)
    char choix = 'c'; // Défini choix (initialisé a 'c', car l'on doit tout d'abord charger un fichier pour jouer)
    action(choix, grille, nom_fichier); // Pour cela, on apelle action directement (avec choix = 'c')
    while (check_fin(grille) == 1) { // Tant qu'on a pas fini le sudoku
        afficher_grille(grille); // On affiche la grille
        choisir(&choix); // On demande le choix du joueur
        action(choix, grille, nom_fichier); // On effectue le choix du joueur
    }
    afficher_grille(grille); // On affiche la grille qui est fini
    printf("\n[BRAVO] - Vous avez terminez le sudoku !\n"); // Msg de félicitations
    printf("\nVoullez-vous sauvegarder = 's', ou quitter = 'q', ou alors rejouer = 'r' ?\n");
    scanf("%c", &choix);
    scanf("%c", &choix); // Double scanf a cause d'un bug qui ne prend pas en compte le premier scanf
    switch(choix) {
        case 's': // Si le joueur veut sauvegarder
            ecrire_grille(grille, nom_fichier);
            printf("Voullez-vous maintenant rejouer = 'r' ?"); // Mais lui demande quand même s'il veut rejouer
            scanf("%c", &choix);
            scanf("%c", &choix); // Double scanf a cause d'un bug qui ne prend pas en compte le premier scanf
            if (choix == 'r') main();
            break;
        case 'r': // Si le joueur veut rejouer
            main();
            break;
    }
    // Pas besoin de faire un cas pour 'q', car dans tout les cas le programme s'arrête maintenant
    return 0;
}