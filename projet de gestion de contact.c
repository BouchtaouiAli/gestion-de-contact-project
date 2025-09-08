// BIBLIOTHEQUES NECESSAIRES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// CONSTANTES DU PROGRAMME

#define MAX_CONTACTS 50
#define MAX_NAME 30
#define MAX_PHONE 15
#define MAX_EMAIL 40

// DEFINITION DE LA STRUCTURE CONTACT

typedef struct {
    char nom[MAX_NAME];
    char telephone[MAX_PHONE];
    char email[MAX_EMAIL];
} Contact;

// VARIABLES GLOBALES

Contact contacts[MAX_CONTACTS];
int nombre_contacts = 0;

// CODES DE COULEURS POUR LA CONSOLE

#define COLOR_WHITE   0
#define COLOR_BLUE    1
#define COLOR_GREEN   2
#define COLOR_CYAN    3
#define COLOR_RED     4
#define COLOR_MAGENTA 5
#define COLOR_YELLOW  6
#define COLOR_WHITE   7
#define COLOR_GRAY    8

// FONCTIONS UTILITAIRES

void set_color(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
int valider_telephone(char* telephone) {
    int longueur = strlen(telephone);
    if (longueur != 10) {
        return 0;
    }
    if (strncmp(telephone, "06", 2) != 0 && strncmp(telephone, "07", 2) != 0) {
        return 0;
    }
    for (int i = 0; i < longueur; i++) {
        if (telephone[i] < '0' || telephone[i] > '9') {
            return 0;
        }
    }
    return 1;
}
int valider_email(char* email) {
    int longueur = strlen(email);
    int at_found = 0;
    int dot_found = 0;
    int at_position = -1;
    if (longueur < 5) {
        return 0;
    }
    for (int i = 0; i < longueur; i++) {
        if (email[i] == '@') {
            if (at_found == 1) {
                return 0;
            }
            at_found = 1;
            at_position = i;
        }
    }
    if (at_found == 0) {
        return 0;
    }
    if (at_position == 0) {
        return 0;
    }
    for (int i = at_position + 1; i < longueur; i++) {
        if (email[i] == '.') {
            dot_found = 1;
            break;
        }
    }
    if (dot_found == 0) {
        return 0;
    }
    return 1;
}
void saisir_chaine(char* buffer, int taille, const char* invite, int (*validateur)(char*)) {
    int valide = 0;
    do {
        set_color(COLOR_CYAN);
        printf("%s", invite);
        set_color(COLOR_WHITE);
        fgets(buffer, taille, stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        valide = validateur(buffer);
        if (!valide) {
            set_color(COLOR_RED);
            if (strstr(invite, "Telephone")) {
                printf("[ERREUR] Numero invalide ! Doit commencer par 06 ou 07 et avoir 10 chiffres.\n");
            } else if (strstr(invite, "Email")) {
                printf("[ERREUR] Email invalide ! Format attendu : exemple@domaine.com\n");
            }
            set_color(COLOR_WHITE);
        }
    } while (!valide);
}
void saisir_chaine_simple(char* buffer, int taille, const char* invite) {
    set_color(COLOR_CYAN);
    printf("%s", invite);
    set_color(COLOR_WHITE);

    fgets(buffer, taille, stdin);
    set_color(COLOR_WHITE);
    buffer[strcspn(buffer, "\n")] = 0;
}
int rechercher_contact(char* search_value, int critere, int* index) {
    for (int i = 0; i < nombre_contacts; i++) {
        int trouve = 0;
        switch (critere) {
            case 1:
                if (strcmp(contacts[i].nom, search_value) == 0) trouve = 1;
                break;
        }
        if (trouve) {
            *index = i;
            return 1;
        }
    }
    return 0;
}
void clear_screen() {
    system("cls");
}
void wait_key() {
    set_color(COLOR_GRAY);
    printf("\nAppuyez sur Entree pour continuer...");
    set_color(COLOR_WHITE);
    getchar();
}
void show_title() {
    set_color(COLOR_CYAN);
    printf("===============================================\n");
    printf("        GESTIONNAIRE DE CONTACTS\n");
    printf("===============================================\n\n");
    set_color(COLOR_WHITE);
}
void show_menu() {
    set_color(COLOR_BLUE);
    printf("+---------------------------------------------+\n");
    printf("|                MENU PRINCIPAL               |\n");
    printf("+---------------------------------------------+\n");
    set_color(COLOR_WHITE);
    printf("| 1. Ajouter un contact                      |\n");
    printf("| 2. Afficher tous les contacts              |\n");
    printf("| 3. Rechercher un contact                   |\n");
    printf("| 4. Modifier un contact                     |\n");
    printf("| 5. Supprimer un contact                    |\n");
    set_color(COLOR_RED);
    printf("| 0. Quitter                                 |\n");
    set_color(COLOR_BLUE);
    printf("+---------------------------------------------+\n");
    set_color(COLOR_YELLOW);
    printf("\nVotre choix : ");
    set_color(COLOR_WHITE);
}
// FONCTION POUR AJOUTER UN CONTACT

void add_contact() {
    clear_screen();
    show_title();
    if (nombre_contacts >= MAX_CONTACTS) {
        set_color(COLOR_RED);
        printf("[ERREUR] Carnet plein !\n");
        set_color(COLOR_WHITE);
        wait_key();
        return;
    }
    set_color(COLOR_GREEN);
    printf("--- AJOUT D'UN CONTACT ---\n\n");
    set_color(COLOR_WHITE);
    saisir_chaine_simple(contacts[nombre_contacts].nom, MAX_NAME, "Nom : ");
    saisir_chaine(contacts[nombre_contacts].telephone, MAX_PHONE,
                  "Telephone (06xxxxxxxx ou 07xxxxxxxx) : ", valider_telephone);
    saisir_chaine(contacts[nombre_contacts].email, MAX_EMAIL,
                  "Email (exemple@domaine.com) : ", valider_email);
    nombre_contacts++;
    set_color(COLOR_GREEN);
    printf("\n[SUCCES] Contact ajoute !\n");
    set_color(COLOR_WHITE);
    wait_key();
}
// FONCTION POUR AFFICHER TOUS LES CONTACTS

void show_all_contacts() {
    clear_screen();
    show_title();
    if (nombre_contacts == 0) {
        set_color(COLOR_YELLOW);
        printf("[INFO] Aucun contact.\n");
        set_color(COLOR_WHITE);
        wait_key();
        return;
    }
    set_color(COLOR_GREEN);
    printf("--- LISTE DES CONTACTS ---\n\n");
    set_color(COLOR_WHITE);
    for (int i = 0; i < nombre_contacts; i++) {
        set_color(COLOR_BLUE);
        printf("Contact %d:\n", i + 1);
        set_color(COLOR_WHITE);
        printf("  Nom      : %s\n", contacts[i].nom);
        printf("  Telephone: %s\n", contacts[i].telephone);
        printf("  Email    : %s\n", contacts[i].email);
        set_color(COLOR_GRAY);
        printf("  -------------------------\n");
        set_color(COLOR_WHITE);
    }
    set_color(COLOR_MAGENTA);
    printf("\nTotal : %d contact(s)\n", nombre_contacts);
    set_color(COLOR_WHITE);
    wait_key();
}
// FONCTION POUR RECHERCHER UN CONTACT

void search_contact() {
    clear_screen();
    show_title();
    if (nombre_contacts == 0) {
        set_color(COLOR_RED);
        printf("[INFO] Aucun contact.\n");
        set_color(COLOR_WHITE);
        wait_key();
        return;
    }
    set_color(COLOR_GREEN);
    printf("--- RECHERCHE DE CONTACT ---\n\n");
    set_color(COLOR_WHITE);
    set_color(COLOR_YELLOW);
    printf("=== CRITERES DE RECHERCHE ===\n");
    set_color(COLOR_WHITE);
    printf("1. Rechercher par nom\n");

    printf("\n");
    int critere_choix;
    
    printf("\n");
    printf("Votre choix : ");
    scanf("%d", &critere_choix);
    getchar();
    char search_value[MAX_NAME];
    int index = -1;
    switch (critere_choix) {
        case 1:
            saisir_chaine_simple(search_value, MAX_NAME, "Nom a rechercher : ");
            break;
        default:
            set_color(COLOR_RED);
            printf("[ERREUR] Choix invalide !\n");
            set_color(COLOR_WHITE);
            wait_key();
            return;
    }
    if (rechercher_contact(search_value, critere_choix, &index)) {
        set_color(COLOR_GREEN);
        printf("\n[SUCCES] Contact trouve !\n");
        set_color(COLOR_WHITE);
        printf("  Nom      : %s\n", contacts[index].nom);
       
    } else {
        set_color(COLOR_RED);
        printf("\n[ERREUR] Contact non trouve.\n");
        set_color(COLOR_WHITE);
    }
    wait_key();
}
// FONCTION POUR MODIFIER UN CONTACT

void modify_contact() {
    clear_screen();
    show_title();
    if (nombre_contacts == 0) {
        set_color(COLOR_RED);
        printf("[INFO] Aucun contact.\n");
        set_color(COLOR_WHITE);
        wait_key();
        return;
    }
    set_color(COLOR_GREEN);
    printf("--- MODIFICATION DE CONTACT ---\n\n");
    set_color(COLOR_WHITE);
    printf("\n");
    set_color(COLOR_YELLOW);
    printf("=== CRITERES DE RECHERCHE ===\n");
    set_color(COLOR_WHITE);
    printf("1. Rechercher par nom\n");
    printf("\n");
    int critere_choix;
    printf("Votre choix  : ");
    scanf("%d", &critere_choix);
    getchar();
    char search_value[MAX_NAME];
    int index = -1;
    switch (critere_choix) {
        case 1:
            saisir_chaine_simple(search_value, MAX_NAME, "Nom du contact a modifier : ");
            break;

        default:
            set_color(COLOR_RED);
            printf("[ERREUR] Choix invalide !\n");
            set_color(COLOR_WHITE);
            wait_key();
            return;
    }
    if (rechercher_contact(search_value, critere_choix, &index)) {
        set_color(COLOR_GREEN);
        printf("\n[SUCCES] Contact trouve !\n");
        set_color(COLOR_WHITE);
        printf("  Nom      : %s\n", contacts[index].nom);
        set_color(COLOR_YELLOW);
        printf("\n=== QUE VOULEZ-VOUS MODIFIER ? ===\n");
        set_color(COLOR_WHITE);
        printf("1. Modifier le nom\n");
        printf("2. Modifier le telephone\n");
        printf("3. Modifier l'email\n");
        printf("0. Annuler la modification\n\n");
        int modification_choix;
        printf("Votre choix (0-3) : ");
        scanf("%d", &modification_choix);
        getchar();
        switch (modification_choix) {
            case 1:
                saisir_chaine_simple(contacts[index].nom, MAX_NAME, "Nouveau nom : ");
                printf("\n[SUCCES] Nom modifie !\n");
                break;
            case 2:
                saisir_chaine(contacts[index].telephone, MAX_PHONE,
                              "Nouveau telephone (06xxxxxxxx ou 07xxxxxxxx) : ", valider_telephone);
                printf("\n[SUCCES] Telephone modifie !\n");
                break;
            case 3:
                saisir_chaine(contacts[index].email, MAX_EMAIL,
                              "Nouvel email (exemple@domaine.com) : ", valider_email);
                printf("\n[SUCCES] Email modifie !\n");
                break;
            case 0:
                printf("\n[INFO] Modification annulee.\n");
                break;
            default:
                set_color(COLOR_RED);
                printf("\n[ERREUR] Choix invalide !\n");
                set_color(COLOR_WHITE);
                break;
        }
    } else {
        set_color(COLOR_RED);
        printf("\n[ERREUR] Contact non trouve.\n");
        set_color(COLOR_WHITE);
    }
    wait_key();
}
// FONCTION POUR SUPPRIMER UN CONTACT

void delete_contact() {
    clear_screen();
    show_title();
    if (nombre_contacts == 0) {
        set_color(COLOR_RED);
        printf("[INFO] Aucun contact.\n");
        set_color(COLOR_WHITE);
        wait_key();
        return;
    }
    set_color(COLOR_GREEN);
    printf("--- SUPPRESSION DE CONTACT ---\n\n");
    set_color(COLOR_WHITE);
    set_color(COLOR_CYAN);
    printf("=== CONTACTS EXISTANTS ===\n");
    set_color(COLOR_WHITE);
    for (int i = 0; i < nombre_contacts; i++) {
        printf("%d. %s (%s) - %s\n", i + 1, contacts[i].nom, contacts[i].telephone, contacts[i].email);
    }
    printf("\n");
    char search_value[MAX_NAME];
    saisir_chaine_simple(search_value, MAX_NAME, "Nom du contact a supprimer : ");
    int index = -1;
    if (rechercher_contact(search_value, 1, &index)) {
        set_color(COLOR_GREEN);
        printf("\n[SUCCES] Contact trouve !\n");
        set_color(COLOR_WHITE);
        printf("  Nom      : %s\n", contacts[index].nom);
        printf("  Telephone: %s\n", contacts[index].telephone);
        printf("  Email    : %s\n", contacts[index].email);
        set_color(COLOR_YELLOW);
        printf("\nSupprimer ce contact ? (o/n) : ");
        set_color(COLOR_WHITE);
        char confirm;
        scanf(" %c", &confirm);
        getchar();
        if (confirm == 'o' || confirm == 'O') {
            for (int i = index; i < nombre_contacts - 1; i++) {
                contacts[i] = contacts[i + 1];
            }
            nombre_contacts--;
            printf("\n[SUCCES] Contact supprime !\n");
        } else {
            printf("\n[INFO] Suppression annulee.\n");
        }
    } else {
        set_color(COLOR_RED);
        printf("\n[ERREUR] Contact non trouve.\n");
        set_color(COLOR_WHITE);
    }
    wait_key();
}
// FONCTION PRINCIPALE (POINT D'ENTREE)

int main() {
    int choice;
    do {
        clear_screen();
        show_title();
        show_menu();
        set_color(COLOR_WHITE);
        scanf("%d", &choice);
        set_color(COLOR_WHITE);
        getchar();
        switch (choice) {
            case 1:
                add_contact();
                break;
            case 2:
                show_all_contacts();
                break;
            case 3:
                search_contact();
                break;
            case 4:
                modify_contact();
                break;
            case 5:
                delete_contact();
                break;
            case 0:
                printf("\nMerci d'avoir utilise le programme !\n");
                break;
            default:
                printf("\n[ERREUR] Choix invalide !\n");
                wait_key();
                break;
        }
    } while (choice != 0);
    return 0;
}