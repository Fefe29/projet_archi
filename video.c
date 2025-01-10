
// #include <stdint.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include "souris.h"


// #define VIDEO_BASE 0x22020000


// typedef struct {
//     volatile uint32_t CR;
//     volatile uint32_t SR;
//     volatile uint32_t DMA_ADDR;
//     volatile uint32_t WIDTH;
//     volatile uint32_t HEIGHT;
//     volatile uint32_t BPP;
//     volatile uint32_t PITCH;
//     volatile uint32_t RED_MASK;
//     volatile uint32_t GREEN_MASK;
//     volatile uint32_t BLUE_MASK;   
// } video_device;

// #define VIDEO ((volatile video_device*) VIDEO_BASE)

// void width_puts(const uint32_t val)
// {   
//     VIDEO->WIDTH = val;
// }

// void height_puts(const uint32_t val)
// {   
//     VIDEO->HEIGHT = val;
// }

// uint32_t make_color(uint8_t red, uint8_t green, uint8_t blue) {
//     return ((uint32_t)red << 16) | ((uint32_t)green << 8) | (uint32_t)blue;
// }

// void init_video(uint32_t *framebuffer) {
//     width_puts(640);
//     height_puts(480);
    
//     VIDEO->DMA_ADDR = (uint32_t)framebuffer;
//     VIDEO->CR = 1;
//     VIDEO->RED_MASK = make_color(255, 0, 0); // red
//     VIDEO->GREEN_MASK = make_color(0, 255, 0); // green
//     VIDEO->BLUE_MASK = make_color(0, 0, 255); // blue

//     // Fill the framebuffer with the red background
//     for (uint32_t i = 0; i < VIDEO->WIDTH * VIDEO->HEIGHT; i++) {
//         framebuffer[i] = VIDEO->RED_MASK;
//     }

//     // Triangle
//     uint32_t tree_color = VIDEO->GREEN_MASK;
//     uint32_t center = VIDEO->WIDTH / 2;
//     uint32_t height = VIDEO->HEIGHT / 2;
//     // 1er triangle
//     for (uint32_t y = 0; y < height / 2; y++) {
//         for (uint32_t x = center - y / 1.5; x <= center + y / 1.5; x++) {
//             framebuffer[y * VIDEO->WIDTH + x] = tree_color;
//         }
//     }
//     // 2eme triangle
//     for (uint32_t y = height / 2; y < height; y++) {
//         for (uint32_t x = center - (y - height / 3) / 1.1; x <= center + (y - height / 3) / 1.1; x++) {
//             framebuffer[y * VIDEO->WIDTH + x] = tree_color;
//         }
//     }
//     // 3eme triangle
//     for (uint32_t y = height / 2; y < height * 0.8 * 2; y++) {
//         for (uint32_t x = center - (y - height / 2) / 1.1; x <= center + (y - height / 2) / 1.1; x++) {
//             framebuffer[y * VIDEO->WIDTH + x] = tree_color;
//         }
//     }

//     // Tronc
//     uint32_t couleur_tronc = make_color(139, 69, 19); // marron
//     uint32_t largeur_tronc = VIDEO->WIDTH / 10;
//     uint32_t hauteur_tronc = VIDEO->HEIGHT / 10;
//     uint32_t debut_tronc_x = center - largeur_tronc / 2;
//     uint32_t debut_tronc_y = height * 1.6;

//     for (uint32_t y = debut_tronc_y; y < debut_tronc_y + hauteur_tronc; y++) {
//         for (uint32_t x = debut_tronc_x; x < debut_tronc_x + largeur_tronc; x++) {
//             framebuffer[y * VIDEO->WIDTH + x] = couleur_tronc;
//         }
//     }
    
//     //Boule de noel test
//     uint32_t rayon = 40; // Rayon de la boule de Noël
//     uint32_t pos_x = 50;
//     uint32_t pos_y = 50;
//     printf("Dessin de la boule de Noël en (%ld, %ld)\n", pos_x, pos_y);
//     for (uint32_t y = -rayon; y <= rayon; y++) {
//         printf("1");
//         for (uint32_t x = -rayon; x <= rayon; x++) {
//             printf("2");
//             if (x * x + y * y <= rayon * rayon) {
//                 printf("3");
//                 uint32_t pixel_x = pos_x + x;
//                 uint32_t pixel_y = pos_y + y;
//                 if (pixel_x < VIDEO->WIDTH && pixel_y < VIDEO->HEIGHT) {
//                     framebuffer[pixel_y * VIDEO->WIDTH + pixel_x] = make_color(218,112,3);
//                     printf("4");
//                 }
//             }
//         }
//     }
// }

// int main()
// {   
//     init_uart();
//     printf("Démarrage\n");
//     uint32_t *framebuffer = (uint32_t *)malloc(640 * 480 * sizeof(uint32_t));
//     if (framebuffer == NULL) {
//         return -1; // Handle allocation failure
//     }

//     init_video(framebuffer);


//     // // Activer les interruptions pour la souris
//     // SOURIS->CR |= IE;

//     // evenement_souris event;
//     // uint32_t couleur_boule = make_color(255, 215, 0); // or

//     // Boucle infinie pour garder l'image affichée et gérer les événements souris
//     while (1) {
//         // if (lire_evenement_souris(&event) == 0) {
//         //     if (event.event_type == LEFT_BUTTON_DOWN) {
//         //         dessiner_boule_noel(framebuffer, VIDEO->WIDTH, VIDEO->HEIGHT, event.pos_x, event.pos_y, couleur_boule);
//         //     }
//         // }
//         while (!(VIDEO->SR & 0x2)) {}
//         VIDEO->SR = 0x2;
//     }

//     free(framebuffer);
//     return 0;
// }
