#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef enum{
    AVANCER, RECULER, TOURNER_GAUCHE, TOURNER_DROITE, ARRET_URGENCE
} Typemvt;

typedef enum{
    NORMALE, URGENTE
} Priorite;

typedef struct commande{
    Typemvt type;
    int duree;
    float vitesse;
    Priorite priorite;
} cmd;

typedef struct noeud {
    cmd commande;
    struct noeud *suiv;
} noeud;

typedef struct file {
    noeud* tete;
    noeud* queue;
} file;

file file_normale;
file file_urgent;

void initFile(file *f) {
    f->tete = NULL;
    f->queue = NULL;
}

void enfiler(file *f, cmd x) {
    noeud* nouveau = (noeud*) malloc(sizeof(noeud));
    nouveau->commande = x;
    nouveau->suiv = NULL;
    if (f->tete == NULL && f->queue == NULL) {
        f->tete = nouveau;
        f->queue = nouveau;
    } else {
        f->queue->suiv = nouveau;
        f->queue = nouveau;
    }
}

void ajouter_commande_normale(cmd c){
    enfiler(&file_normale, c);
}

void ajouter_commande_urgent(cmd c){
    enfiler(&file_urgent, c);
}

cmd defiler(file *f) {
    if (f->tete == NULL && f->queue == NULL) {
        printf("La file est vide!\n");
        cmd vide = {AVANCER, 0, 0, NORMALE};
        return vide;
    }
    noeud *temp = f->tete;
    cmd data = temp->commande;
    if (f->tete == f->queue) {
        f->tete = f->queue = NULL;
    } else {
        f->tete = f->tete->suiv;
    }
    free(temp);
    return data;
}

cmd prendre_prochaine_commande(){
    cmd commande;
    printf("Tapez votre commande\n");
    printf("Type mouvement (0=AVANCER,1=RECULER,2=TOURNER_GAUCHE,3=TOURNER_DROITE,4=ARRET_URGENCE): ");
    scanf("%d", &commande.type);
    printf("La duree (s): ");
    scanf("%d", &commande.duree);
    printf("La vitesse: ");
    scanf("%f", &commande.vitesse);
    printf("Priorite(0=NORMALE,1=URGENTE): ");
    scanf("%d", &commande.priorite);
    return commande;
}

bool capteur(){
    return rand() % 10 == 1;
}

void contourement(){
    if (capteur()) {
        printf("COMMANDES DE CONTOUREMENT\n");
        cmd arr = {ARRET_URGENCE, 3, 0, URGENTE};
        cmd rec = {RECULER, 2, 2, URGENTE};
        cmd drt = {TOURNER_DROITE, 4, 2, URGENTE};
        ajouter_commande_urgent(arr);
        ajouter_commande_urgent(rec);
        ajouter_commande_urgent(drt);
    }
}
long temps_total = 0;
void journaliser_commande(cmd c) {
    FILE *fichier = fopen("journal_robot.txt", "a");
    if (!fichier) {
        printf("Erreur d'ouverture du fichier journal !\n");
        return;
    }
    long heures   =  temps_total / 3600;
    long minutes  = (temps_total % 3600) / 60;
    long secondes =  temps_total % 60;
    fprintf(fichier, "[%02ld:%02ld:%02ld] ", heures, minutes, secondes);


    if (c.priorite == URGENTE)
        fprintf(fichier, "[URGENTE] ");
    else
        fprintf(fichier, "[NORMALE] ");
    switch (c.type) {
        case AVANCER:
            fprintf(fichier, "AVANCER");
            break;
        case RECULER:
             fprintf(fichier, "RECULER");
              break;
        case TOURNER_GAUCHE:
            fprintf(fichier, "TOURNER_GAUCHE");
            break;
        case TOURNER_DROITE:
            fprintf(fichier, "TOURNER_DROITE");
            break;
        case ARRET_URGENCE:
            fprintf(fichier, "ARRET_URGENCE");
             break;
    }
    fprintf(fichier, " | duree=%ds | vitesse=%.2f\n", c.duree, c.vitesse);
    fclose(fichier);
    temps_total += c.duree;
}




void afficherCommande(cmd c) {
    printf("[Type: ");
    switch (c.type) {
        case AVANCER:printf("AVANCER"); break;
        case RECULER:printf("RECULER"); break;
        case TOURNER_GAUCHE:printf("TOURNER_GAUCHE"); break;
        case TOURNER_DROITE:printf("TOURNER_DROITE"); break;
        case ARRET_URGENCE:printf("ARRET_URGENCE"); break;
    }

    printf("| Duree=%ds", c.duree);
    printf("| Vitesse=%.2f", c.vitesse);
    printf("| Priorite= ");
    if (c.priorite == URGENTE)
        printf("URGENTE");
    else
        printf("NORMALE");

    printf("] ");
}

void execution(file *file_normale, file *file_urgent){
    while ((file_urgent->tete != NULL) || (file_normale->tete != NULL)) {

        cmd c;
        if (file_urgent->tete != NULL) {
            printf("\nCOMMANDE URGENTE:\n");
            c = defiler(file_urgent);
            journaliser_commande(c);
        } else if ((file_normale->tete != NULL)&& (file_urgent->tete == NULL)) {
            printf("\nCOMMANDE NORMALE:\n");
            c = defiler(file_normale);
            journaliser_commande(c);
            contourement();
        }


        afficherCommande(c);
    }
}




typedef struct arbre {
    cmd commande;
    bool obstacle;
    struct arbre *gauche;
    struct arbre *droite;
} arbre;



arbre* creer_noeud(cmd c, bool obstacle) {
    arbre *n = malloc(sizeof(arbre));
    n->commande = c;
    n->obstacle = obstacle;
    n->gauche = NULL;
    n->droite = NULL;
    return n;
}
arbre* construire_arbre() {
    int existe;

    printf("Ajouter une commande dans l'arbre ? (1=oui, 0=non): ");
    scanf("%d", &existe);

    if (!existe) return NULL;
    cmd c;
    c=prendre_prochaine_commande();

    bool obstacle = capteur();

    if (obstacle)
        printf("Obstacle détecté dand la branche \n");
    else
        printf("Pas d'obstacle dans la branche \n");

    arbre *root = creer_noeud(c, obstacle);

    if (obstacle)
        return root;

    printf("\nBranche gauche:\n");
    root->gauche = construire_arbre();
    printf("\nBranche droite:\n");
    root->droite = construire_arbre();
    return root;
}

void chemins_OK(arbre *n, cmd chemin[], int k) {
    if (!n) return;

    if (n->obstacle == true)
        return;

    chemin[k] = n->commande;

    if (!n->gauche && !n->droite) {
        printf("\nChemin valide : ");
        for (int i = 0; i <= k; i++)
            afficherCommande(chemin[i]);
        printf("\n");
        return;
    }

    chemins_OK(n->gauche, chemin, k+1);
    chemins_OK(n->droite, chemin, k+1);
}


int main() {
    initFile(&file_normale);
    initFile(&file_urgent);
    srand(time(NULL));

    int choix;
    do {
        printf("\n1. Ajouter une commande dans les files\n");
        printf("2. Faire l'execution\n");
        printf("3. Construire un arbre et afficher chemins sans obstacle\n");
        printf("4. Quitter\n");
        printf("Choix : ");
        scanf("%d", &choix);
        cmd x;
        switch (choix) {
            case 1:
                x=prendre_prochaine_commande();
                if (x.priorite == URGENTE)
                    ajouter_commande_urgent(x);
                else
                    ajouter_commande_normale(x);
                break;
            case 2:
                execution(&file_normale, &file_urgent);
                printf("Execution terminee\n");
                break;
            case 3:{
                printf("\nConstruction de l'arbre:\n");
                arbre *racine = construire_arbre();

                printf("Chemins sans obstacle:\n");
                cmd chemin[200];
                chemins_OK(racine, chemin, 0);
                break;
            }
        }
    } while (choix != 4);
    return 0;
}
































