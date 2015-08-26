#include "util.h"
#include "ihm.h"


int main ( int argc, char *argv[] )
{

    static const char* database_filename = "data.db";
    char *mot, *nom_joueur;
    short choix, nombre_tours, numero_tour, score = 0;
    short numero_mot_courant = 0;
    LISTE_MOT *liste_mots;
    TABLEAU_SCORE *tableau_scores_1, *tableau_scores_2,  *tableau_scores_3;

    db_creer_si_existepas(database_filename);

    tableau_scores_1 = db_get(database_filename, 3);
    tableau_scores_2 = db_get(database_filename, 5);
    tableau_scores_3 = db_get(database_filename, 10);

    rand_init();

    liste_mots = manage_arguments(argc, argv);

    do
    {
        choix = menu();

        if (choix == 1)
        {
            score = 0;
            if (liste_mots == NULL) {
                mot = demander_mot();
                jeu(mot);
            }
            else
            {
                nombre_tours = choisir_niveau();
                for (numero_tour = 0 ; numero_tour < nombre_tours ; numero_tour++)
                {
                    mot = liste_mots->mots[numero_mot_courant];
                    numero_mot_courant++;
                    if (numero_mot_courant >= liste_mots->nombre)
                    {
                        // On n'a pas assez de mot: on mélange et on recommence
                        melanger_mots(liste_mots);
                        numero_mot_courant = 0;
                    }
                    score += jeu(mot);
                }

                printf("Votre score total est %d\n\n", score);
                printf("Votre nom: ");
                nom_joueur = saisir_chaine();
                printf("\n");

                switch (nombre_tours)
                {
                    case 3:
                    {
                        ajouter_score(tableau_scores_1, score, nom_joueur);
                        break;
                    }
                    case 5:
                    {
                        ajouter_score(tableau_scores_2, score, nom_joueur);
                        break;
                    }
                    case 10:
                    {
                        ajouter_score(tableau_scores_3, score, nom_joueur);
                        break;
                    }
                }
            }
        }
        else if (choix == 2)
        {
            scores(tableau_scores_1, tableau_scores_2, tableau_scores_3);
        }
    } while (choix != 3);

    db_set(database_filename, 3, tableau_scores_1);
    db_set(database_filename, 5, tableau_scores_2);
    db_set(database_filename, 10, tableau_scores_3);

    printf("\nBye.\n");

    return 0;
}

