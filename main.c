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
    unsigned int hue2 = 768;

	int top_msg_x_pos = 0;
	int bot_msg_x_pos = 0;
	int px = 0;
	int py = 0;
	int sample_num = 0;

	int speed_x = 7;
	int speed_y = 7;
	int top_msg_speed = 2;
	int bot_msg_speed = 3;


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

// void affichage_dyn(void *arg)
// {
// 	(void)arg;
// 	int top_msg_x_pos = 0;
// 	int bot_msg_x_pos = 0;
// 	int px = 0;
// 	int py = 0;
// 	unsigned int hue1 = 0;
// 	unsigned int hue2 = 768;
// 	int sample_num = 0;

// 	int speed_x = 7;
// 	int speed_y = 7;
// 	int top_msg_speed = 2;
// 	int bot_msg_speed = 3;

//     // Activer les interruptions pour la souris
//     MOUSE->CR |= MOUSE_CR_IE;

//     mouse_data_t event;
//     uint32_t couleur_boule = make_color(255, 215, 0); // or

 

// 	while (1) {

// 		xSemaphoreTake(video_refresh_semaphore, portMAX_DELAY);

//         while (!(VIDEO->SR & 0x2)) {}
//         VIDEO->SR = 0x2;

//         /// SOURIS ///
//         if (lire_evenement_souris(&event) == 2) {
//             {
//                 dessiner_boule_noel(framebuffer, VIDEO->WIDTH, VIDEO->HEIGHT, event.x, event.y, couleur_boule);
//             }
//         }


// 		// fill_rectangle(px, py, soccer_ball.width, soccer_ball.height, BLACK);

// 		top_msg_x_pos += top_msg_speed;
// 		if (top_msg_x_pos < -16 || top_msg_x_pos + 14*16 >= SCREEN_WIDTH) {
// 			top_msg_speed = -top_msg_speed;
// 		}
// 		bot_msg_x_pos += bot_msg_speed;
// 		if (bot_msg_x_pos < -8 || bot_msg_x_pos + 14*8 >= SCREEN_WIDTH) {
// 			bot_msg_speed = -bot_msg_speed;
// 		}

// 		font_16x32_draw_text(top_msg_x_pos, 64, " Joyeux Noel la team! ", hue_to_color(hue1), BLACK);
// 		hue1 = (hue1 + 1) % 1536;

// 		font_8x16_draw_text(bot_msg_x_pos, SCREEN_HEIGHT - 64, " Joyeux Noel la team! ", hue_to_color(hue2), BLACK);
// 		hue2 = (hue2 + 1) % 1536;

//         // Le ballon de foot///

// 		// int bump = 0;
// 		// px += speed_x;
// 		// if (px < 0) {
// 		// 	px = -px;
// 		// 	speed_x = -speed_x;
// 		// 	bump = 1;
// 		// } else if (px + soccer_ball.width > SCREEN_WIDTH) {
// 		// 	px = 2*(SCREEN_WIDTH - soccer_ball.width) - px;
// 		// 	speed_x = -speed_x;
// 		// 	bump = 1;
// 		// }
// 		// py += speed_y;
// 		// if (py < 0) {
// 		// 	py = -py;
// 		// 	speed_y = -speed_y;
// 		// 	bump = 1;
// 		// } else if (py + soccer_ball.height > SCREEN_HEIGHT) {
// 		// 	py = 2*(SCREEN_HEIGHT - soccer_ball.height) - py;
// 		// 	speed_y = -speed_y;
// 		// 	bump = 1;
// 		// }
// 		// draw_sprite(px, py, &soccer_ball);

// 		// draw_line(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GREEN); // pour print la ligne transversale

        

// 		// if (bump) {
// 		// 	Mix_PlayChannel(-1, sound_samples[sample_num], MIX_MAX_VOLUME);
// 		// 	sample_num = (sample_num + 1) % (NB_SOUND_SAMPLES - 4);
// 		// }
//         /////////////////////////
// 	}
// }


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

int main()
{
	init_uart();

	// init_audio_mixer(4);
	init_video();

	xTaskCreate(affichage_dyn, "hello", 1024, NULL, 1, NULL);

	vTaskStartScheduler();

	return 0;
}







