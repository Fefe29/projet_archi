#include "harvey_platform.h"
#include "header.h"
#include "samples.h" // Assurez-vous d'avoir la déclaration de sound_sample_t
#include "audio_server.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>



/*Souris*/
int lire_evenement_souris(mouse_data_t *event) {
    // // Vérifiez si la FIFO contient un événement
    // if (!(SOURIS->SR & MOUSE_SR_FIFO_NOT_EMPTY)) {
    //     printf("FIFO vide\n");
    //     return -1; // La FIFO est vide, aucun événement à lire
    // }


    // Pour diagnostiquer et traiter le clic en fonction de son type
    switch (event->type) {
        case MOUSE_MOTION:
            // printf("Mouvement de la souris à la position : (%d, %d)\n", event->x, event->y);
            return 1;
        case MOUSE_BUTTON_LEFT_DOWN:
            printf("Clic gauche enfoncé à la position : (%d, %d)\n", event->x, event->y);
            return 2;
        case MOUSE_BUTTON_LEFT_UP:
            printf("Clic gauche relâché à la position : (%d, %d)\n", event->x, event->y);
            return 3;
        case MOUSE_BUTTON_MIDDLE_DOWN:
            printf("Clic molette enfoncé à la position : (%d, %d)\n", event->x, event->y);
            return 4;
        case MOUSE_BUTTON_MIDDLE_UP:
            printf("Clic molette relâché à la position : (%d, %d)\n", event->x, event->y);
            return 5;
        case MOUSE_BUTTON_RIGHT_DOWN:
            printf("Clic droit enfoncé à la position : (%d, %d)\n", event->x, event->y);
            return 6;
        case MOUSE_BUTTON_RIGHT_UP:
            printf("Clic droit relâché à la position : (%d, %d)\n", event->x, event->y);
            return 7;
        case MOUSE_WHEEL:
            printf("Défilement de la molette à la position : (%d, %d), quantite : (%d, %d)\n", 
                   event->x, event->y, event->amount_x, event->amount_y);
            return 8;
    }
    return 0; // Événement lu avec succès
}


void dessiner_boule_noel(uint32_t *framebuffer, uint32_t largeur, uint32_t hauteur, int16_t pos_x, int16_t pos_y, uint32_t couleur) {
    printf("Et une boule de plus!\n");
    uint32_t rayon = 10; // Rayon de la boule de Noël
    for (int32_t y = -rayon; y <= (int32_t)rayon; y++) {
        for (int32_t x = -rayon; x <= (int32_t)rayon; x++) {
            if (x * x + y * y <= (int32_t)(rayon * rayon)) {
                uint32_t pixel_x = pos_x + x;
                uint32_t pixel_y = pos_y + y;
                if (pixel_x < largeur && pixel_y < hauteur) {
                    framebuffer[pixel_y * largeur + pixel_x] = couleur;
                }
            }
        }
    }
}

void dessiner_cadeau_noel(uint32_t *framebuffer, uint32_t largeur, uint32_t hauteur, int16_t pos_x, int16_t pos_y, uint32_t couleur) {
    printf("Et un cadeau de plus!\n");

    uint32_t largeur_cadeau = 40; // Largeur du cadeau
    uint32_t hauteur_cadeau = 40; // Hauteur du cadeau
    uint32_t couleur_ruban= make_color(255, 255, 255); //couleur ruban
    // Dessiner la boîte du cadeau
    for (int32_t y = 0; y < (int32_t)hauteur_cadeau; y++) {
        for (int32_t x = 0; x < (int32_t)largeur_cadeau; x++) {
            uint32_t pixel_x = pos_x + x - largeur_cadeau / 2;
            uint32_t pixel_y = pos_y + y - hauteur_cadeau / 2;
            if (pixel_x < largeur && pixel_y < hauteur) {
                framebuffer[pixel_y * largeur + pixel_x] = couleur;
            }
        }
    }

    // Dessiner le ruban horizontal
    uint32_t ruban_largeur = 4; // Largeur du ruban
    for (int32_t y = (int32_t)(hauteur_cadeau / 2 - ruban_largeur / 2); y < (int32_t)(hauteur_cadeau / 2 + ruban_largeur / 2); y++) {
        for (int32_t x = 0; x < (int32_t)largeur_cadeau; x++) {
            uint32_t pixel_x = pos_x + x - largeur_cadeau / 2;
            uint32_t pixel_y = pos_y + y - hauteur_cadeau / 2;
            if (pixel_x < largeur && pixel_y < hauteur) {
                framebuffer[pixel_y * largeur + pixel_x] = couleur_ruban;
            }
        }
    }

    // Dessiner le ruban vertical
    for (int32_t y = 0; y < (int32_t)hauteur_cadeau; y++) {
        for (int32_t x = (int32_t)(largeur_cadeau / 2 - ruban_largeur / 2); x < (int32_t)(largeur_cadeau / 2 + ruban_largeur / 2); x++) {
            uint32_t pixel_x = pos_x + x - largeur_cadeau / 2;
            uint32_t pixel_y = pos_y + y - hauteur_cadeau / 2;
            if (pixel_x < largeur && pixel_y < hauteur) {
                framebuffer[pixel_y * largeur + pixel_x] = couleur_ruban;
            }
        }
    }
}

// Fonction utilitaire pour tracer un segment entre deux points (algorithme de Bresenham)
void tracer_segment(uint32_t *framebuffer, uint32_t largeur, uint32_t hauteur, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t couleur) {
    int16_t dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int16_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int16_t err = dx + dy, e2; // erreur cumulée

    while (1) {
        if (x0 >= 0 && (uint32_t)x0 < largeur && y0 >= 0 && (uint32_t)y0 < hauteur) {
            framebuffer[y0 * largeur + x0] = couleur;
        }
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void dessiner_etoile_noel(uint32_t *framebuffer, uint32_t largeur, uint32_t hauteur, int16_t pos_x, int16_t pos_y, uint32_t couleur) {
    printf("Une étoile simple pour le sapin !\n");

    // Dimensions de l'étoile
    uint32_t taille = 10; // Taille de l'étoile

    // Coordonnées des sommets de l'étoile (5 branches régulières)
    int16_t points[10][2];
    for (int i = 0; i < 10; i++) {
        float angle = 3.14159 * i / 5.0; // Diviser le cercle en 10 points
        float rayon = (i % 2 == 0) ? taille : (taille / 2); // Alternance entre branches extérieures et intérieures
        points[i][0] = pos_x + (int16_t)(rayon * cos(angle));
        points[i][1] = pos_y + (int16_t)(rayon * sin(angle));
    }

    // Relier les points pour former l'étoile
    for (int i = 0; i < 10; i++) {
        int16_t x0 = points[i][0];
        int16_t y0 = points[i][1];
        int16_t x1 = points[(i + 1) % 10][0];
        int16_t y1 = points[(i + 1) % 10][1];
        tracer_segment(framebuffer, largeur, hauteur, x0, y0, x1, y1, couleur);
    }
}



/*Clavier*/

// Tableau de correspondance entre lettres et couleurs
const char* couleurs[] = {
    "Blanc",    // A
    "Rouge",    // B
    "Orange",   // C
    "Jaune",    // D
    "Vert",     // E
    "Cyan",     // F
    "Bleu",     // G
    "Violet",   // H
    "Rose",     // I
    "Marron",   // J
    "Gris",     // K
    "Turquoise",// L
    "Beige",    // M
    "Noir"      // N
};

void traiter_evenement_clavier(uint32_t *couleur) {

    // Lire les données du registre DATA
    uint32_t data = KEYBOARD->DATA;

    // Extraire les informations de l'événement
    int key_code = KEYBOARD_KEY_CODE(data); // Code du symbole
    int pressed = data & KEYBOARD_DATA_PRESSED;

    // Vérifiez si la touche correspond à une lettre

    // Si une touche est pressée et correspond à une lettre
    if (pressed && key_code >= 'a' && key_code <= 'z') {
        // Associer une couleur à la lettre (A -> Blanc, N -> Noir, etc.)
        switch (key_code) {
            case 'a': *couleur = make_color(255, 255, 255); break; // Blanc
            case 'b': *couleur = make_color(255, 0, 0); break;     // Rouge
            case 'c': *couleur = make_color(255, 165, 0); break;   // Orange
            case 'd': *couleur = make_color(255, 255, 0); break;   // Jaune
            case 'e': *couleur = make_color(0, 255, 0); break;     // Vert
            case 'f': *couleur = make_color(0, 255, 255); break;   // Cyan
            case 'g': *couleur = make_color(0, 0, 255); break;     // Bleu
            case 'h': *couleur = make_color(128, 0, 128); break;   // Violet
            case 'i': *couleur = make_color(255, 192, 203); break; // Rose
            case 'j': *couleur = make_color(165, 42, 42); break;   // Marron
            case 'k': *couleur = make_color(128, 128, 128); break; // Gris
            case 'l': *couleur = make_color(64, 224, 208); break;  // Turquoise
            case 'm': *couleur = make_color(245, 222, 179); break; // Beige
            case 'n': *couleur = make_color(255, 69, 0); break;    // Orange Rouge
            case 'o': *couleur = make_color(210, 105, 30); break;  // Chocolat
            case 'p': *couleur = make_color(75, 0, 130); break;    // Indigo
            case 'q': *couleur = make_color(255, 20, 147); break;  // Deep Pink
            case 'r': *couleur = make_color(34, 139, 34); break;   // Vert Forêt
            case 's': *couleur = make_color(70, 130, 180); break;  // Bleu Acier
            case 't': *couleur = make_color(244, 164, 96); break;  // Sable
            case 'u': *couleur = make_color(119, 136, 153); break; // Gris Ardoise
            case 'v': *couleur = make_color(72, 61, 139); break;   // Bleu Nuit
            case 'w': *couleur = make_color(255, 228, 196); break; // Bisque
            case 'x': *couleur = make_color(0, 250, 154); break;   // Vert Printemps
            case 'y': *couleur = make_color(173, 216, 230); break; // Bleu Clair
            case 'z': *couleur = make_color(0, 0, 0); break;       // Noir
            default: break;
        }
        printf("Touche %c appuyée, couleur modifiée.\n", (char)key_code);
    }
}

bool traiter_evenement_espace() {
    // Lire les données du registre DATA
    uint32_t data = KEYBOARD->DATA;

    // Extraire les informations de l'événement
    int key_code = KEYBOARD_KEY_CODE(data); // Code du symbole
    int pressed = data & KEYBOARD_DATA_PRESSED;

    // Vérifiez si la touche espace est pressée
    if (pressed && key_code == ' ') {
        printf("Touche espace appuyée.\n");
        return true;
    }

    return false;
}

