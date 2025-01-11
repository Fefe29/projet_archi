#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "minirisc.h"
#include "uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "harvey_platform.h"
#include "semphr.h"
#include "audio_server.h"
#include "samples.h"
#include "font.h"
#include "sprite.h"
#include "header.h"





uint32_t hue_to_color(unsigned int hue);


#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

#define BLACK 0xff000000
#define WHITE 0xffffffff
#define RED   0xfff00f00
#define GREEN 0xff00ff00
#define TRANSPARENT 0x00000000

extern const sprite_t soccer_ball;

// Déclaration de la fonction pour lire les événements de la souris
int lire_evenement_souris(mouse_data_t *event);
void dessiner_cadeau_noel(uint32_t *framebuffer, uint32_t width, uint32_t height, int x, int y, uint32_t couleur);
void traiter_evenement_clavier(uint32_t *couleur);

uint32_t framebuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
static SemaphoreHandle_t video_refresh_semaphore = NULL;

uint32_t make_color(uint8_t red, uint8_t green, uint8_t blue) {
    return ((uint32_t)red << 16) | ((uint32_t)green << 8) | (uint32_t)blue;
}

/* Hue must be between 0 and 1536 not included */
uint32_t hue_to_color(unsigned int hue)
{
	uint32_t r, g, b;
	if (hue < 256) {
		r = 255;
		g = hue;
		b = 0;
	} else if (hue < 512) {
		r = 511 - hue;
		g = 255;
		b = 0;
	} else if (hue < 768) {
		r = 0;
		g = 255;
		b = hue - 512;
	} else if (hue < 1024) {
		r = 0;
		g = 1023 - hue;
		b = 255;
	} else if (hue < 1280) {
		r = hue - 1024;
		g = 0;
		b = 255;
	} else if (hue < 1536) {
		r = 255;
		g = 0;
		b = 1535 - hue;
	} else {
		r = 0;
		g = 0;
		b = 0;
	}
	return 0xff000000 | (r << 16) | (g << 8) | b;
}


void video_interrupt_handler()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	VIDEO->SR &= ~VIDEO_SR_SRF_P;
	xSemaphoreGiveFromISR(video_refresh_semaphore, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void affichage_dyn(void *arg)
{
    (void)arg;
    unsigned int hue1 = 0;
	int top_msg_x_pos = 0;
	int top_msg_speed = 2;



    // Activer les interruptions pour la souris et le clavier
    MOUSE->CR |= MOUSE_CR_IE;
    KEYBOARD->CR |= KEYBOARD_CR_IE;

    mouse_data_t event;
    uint32_t couleur = make_color(255, 215, 0); // Couleur initiale de la boule

    while (1) {
        // xSemaphoreTake(video_refresh_semaphore, portMAX_DELAY);

		// Attendre que la vidéo soit prête
		while (!(VIDEO->SR & 0x2)) {}
		VIDEO->SR = 0x2;

		if (MOUSE->SR & MOUSE_SR_FIFO_NOT_EMPTY) {
			event = MOUSE->DATA;
			 // Gestion des événements souris
        	if (lire_evenement_souris(&event) == 2) {
            	dessiner_boule_noel(framebuffer, VIDEO->WIDTH, VIDEO->HEIGHT, event.x, event.y, couleur);
        	}
			else if (lire_evenement_souris(&event) == 6) {
				dessiner_cadeau_noel(framebuffer, VIDEO->WIDTH, VIDEO->HEIGHT, event.x, event.y, couleur);
			}
			else if (lire_evenement_souris(&event) == 4) {
				dessiner_etoile_noel(framebuffer, VIDEO->WIDTH, VIDEO->HEIGHT, event.x, event.y, couleur);
			}
		}

       

        // Gestion des événements clavier
        traiter_evenement_clavier(&couleur);

		top_msg_x_pos += top_msg_speed;
		if (top_msg_x_pos < -16 || top_msg_x_pos + 14*16 >= SCREEN_WIDTH) {
			top_msg_speed = -top_msg_speed;
		}
		// bot_msg_x_pos += bot_msg_speed;
		// if (bot_msg_x_pos < -8 || bot_msg_x_pos + 14*8 >= SCREEN_WIDTH) {
		// 	bot_msg_speed = -bot_msg_speed;
		// }

		font_16x32_draw_text(top_msg_x_pos, 64, " Joyeux Noel la team! ", hue_to_color(hue1), BLACK);
		hue1 = (hue1 + 1) % 1536;

		// Dessiner le rectangle pour la légende
		uint32_t rect_x = SCREEN_WIDTH - 30; // Position x à droite de l'écran
		uint32_t rect_y = 10; // Position y en haut de l'écran
		uint32_t rect_width = 20;
		uint32_t rect_height = 20;
		for (uint32_t y = 0; y < rect_height; y++) {
		    for (uint32_t x = 0; x < rect_width; x++) {
		        uint32_t pixel_x = rect_x + x;
		        uint32_t pixel_y = rect_y + y;
		        if (pixel_x < SCREEN_WIDTH && pixel_y < SCREEN_HEIGHT) {
		            framebuffer[pixel_y * SCREEN_WIDTH + pixel_x] = couleur;
		        }
		    }
		}

		// font_8x16_draw_text(bot_msg_x_pos, SCREEN_HEIGHT - 64, " Joyeux Noel la team! ", hue_to_color(hue2), BLACK);
		// hue2 = (hue2 + 1) % 1536;

    }
}


void init_video()
{
	video_refresh_semaphore = xSemaphoreCreateBinary();

	VIDEO->WIDTH  = SCREEN_WIDTH;
	VIDEO->HEIGHT = SCREEN_HEIGHT;
	VIDEO->DMA_ADDR = framebuffer;
	// memset(framebuffer, 0, sizeof(framebuffer));// efface l'ecran
	VIDEO->CR = VIDEO_CR_EN | VIDEO_CR_IE;
	minirisc_enable_interrupt(VIDEO_INTERRUPT);

    VIDEO->RED_MASK = RED;
    VIDEO->GREEN_MASK = GREEN;
    VIDEO->BLUE_MASK = WHITE;

    // Fill the framebuffer with the red background
    for (uint32_t i = 0; i < VIDEO->WIDTH * VIDEO->HEIGHT; i++) {
        framebuffer[i] = VIDEO->RED_MASK;
    }

    // Triangle
    uint32_t tree_color = VIDEO->GREEN_MASK;
    uint32_t center = VIDEO->WIDTH / 2;
    uint32_t height = VIDEO->HEIGHT / 2;
    // 1er triangle
    for (uint32_t y = 0; y < height / 2; y++) {
        for (uint32_t x = center - y / 1.5; x <= center + y / 1.5; x++) {
            framebuffer[y * VIDEO->WIDTH + x] = tree_color;
        }
    }
    // 2eme triangle
    for (uint32_t y = height / 2; y < height; y++) {
        for (uint32_t x = center - (y - height / 3) / 1.1; x <= center + (y - height / 3) / 1.1; x++) {
            framebuffer[y * VIDEO->WIDTH + x] = tree_color;
        }
    }
    // 3eme triangle
    for (uint32_t y = height / 2; y < height * 0.8 * 2; y++) {
        for (uint32_t x = center - (y - height / 2) / 1.1; x <= center + (y - height / 2) / 1.1; x++) {
            framebuffer[y * VIDEO->WIDTH + x] = tree_color;
        }
    }

    // Tronc
    uint32_t couleur_tronc = make_color(139, 69, 19); // marron
    uint32_t largeur_tronc = VIDEO->WIDTH / 10;
    uint32_t hauteur_tronc = VIDEO->HEIGHT / 10;
    uint32_t debut_tronc_x = center - largeur_tronc / 2;
    uint32_t debut_tronc_y = height * 1.6;

    for (uint32_t y = debut_tronc_y; y < debut_tronc_y + hauteur_tronc; y++) {
        for (uint32_t x = debut_tronc_x; x < debut_tronc_x + largeur_tronc; x++) {
            framebuffer[y * VIDEO->WIDTH + x] = couleur_tronc;
        }
    }

}

// Audio
void init_audio() {
    init_audio_mixer(4);  // Initialisation du mixeur audio
    printf("Initialisation de l'audio effectuée\n");
	printf("Veuillez patienter pendant le chargement de la musique et de l'affichage");
}
void thread_musique(void *arg) {
	//Aide à la composition ;)
	//Do: 0
	//Ré: 1
	//Mi: 2
	//Fa: 3
	//Sol: 4
	//La: 5
	//Si: 6
	//etc pour passer à l'octave du dessus.

    // Séquence de notes (indices des notes dans sound_samples)
    const int morceau[] = {
        0, 
		3, 3, 3, 4,
		3, 4, 5,
		5, 5, 5, 6,
		5, 4,
		3, 3, 3, 3, 2, 1,
		0, 0, 0,
		3, 3, 3, 4, 4, 
		3, 1, 
		3, 3, 3,  4,
		3, 3, 4,
		5, 5, 5, 6,



  
    };
	// Do – Fa – Fa – Fa – Sol – Fa – Fa – Sol – La – La – La – Si – La – Sol – Fa – Fa – Fa – Fa – Mi – Ré – Do – Do – Do – Fa – Fa – Fa – Sol – Sol – Fa
    const int durees[] = {
        500,
		500, 500, 500, 500,  // Durée des notes en ms
        1500, 250, 250, 
		500, 500, 500, 500,
        1500, 500,
		750, 250, 250, 250, 250, 250, 
		1000, 500,
		500, 500, 500, 500,
		1500, 250, 250,
		500, 500, 500, 500,
    };

    const int nombre_notes = sizeof(morceau) / sizeof(morceau[0]);

    while (1) {
        for (int i = 0; i < nombre_notes; i++) {
            printf("Note %d : fréquence %.2f Hz\n", morceau[i], note_frequencies[morceau[i]]);
            int channel = Mix_PlayChannel(-1, &sound_samples[morceau[i]], MIX_MAX_VOLUME);

            // Attendre la durée de la note
            vTaskDelay(pdMS_TO_TICKS(durees[i]));
        }

        // Petite pause entre les répétitions du morceau
        vTaskDelay(pdMS_TO_TICKS(4000));
    }
}


int main()
{
	init_uart();


	init_audio();
	init_video();

	// Générer les données audio
    generate_scale();

	xTaskCreate(affichage_dyn, "hello", 1024, NULL, 1, NULL);
	xTaskCreate(thread_musique, "Musique", 1024, NULL, configMAX_PRIORITIES - 1, NULL);



	vTaskStartScheduler();

	return 0;
}







