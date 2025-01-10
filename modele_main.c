// #include <stdio.h>
// #include <string.h>
// #include "minirisc.h"
// #include "harvey_platform.h"
// #include "uart.h"
// #include "FreeRTOS.h"
// #include "task.h"
// #include "semphr.h"
// #include "audio_server.h"
// #include "samples.h"
// #include "font.h"
// #include "sprite.h"

// uint32_t hue_to_color(unsigned int hue);


// #define SCREEN_WIDTH  640
// #define SCREEN_HEIGHT 480

// #define BLACK 0xff000000
// #define WHITE 0xffffffff
// #define RED   0xfff00f00
// #define GREEN 0xff00ff00
// #define TRANSPARENT 0x00000000

// extern const sprite_t soccer_ball;

// uint32_t framebuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
// static SemaphoreHandle_t video_refresh_semaphore = NULL;


// void video_interrupt_handler()
// {
//     BaseType_t xHigherPriorityTaskWoken = pdFALSE;
// 	VIDEO->SR &= ~VIDEO_SR_SRF_P;
// 	xSemaphoreGiveFromISR(video_refresh_semaphore, &xHigherPriorityTaskWoken);
// 	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
// }

// void hello_task(void *arg)
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

// 	while (1) {
// 		xSemaphoreTake(video_refresh_semaphore, portMAX_DELAY);

// 		fill_rectangle(px, py, soccer_ball.width, soccer_ball.height, BLACK);

// 		top_msg_x_pos += top_msg_speed;
// 		if (top_msg_x_pos < -16 || top_msg_x_pos + 14*16 >= SCREEN_WIDTH) {
// 			top_msg_speed = -top_msg_speed;
// 		}
// 		bot_msg_x_pos += bot_msg_speed;
// 		if (bot_msg_x_pos < -8 || bot_msg_x_pos + 14*8 >= SCREEN_WIDTH) {
// 			bot_msg_speed = -bot_msg_speed;
// 		}

// 		font_16x32_draw_text(top_msg_x_pos, 64, " Hello, World! ", hue_to_color(hue1), BLACK);
// 		hue1 = (hue1 + 1) % 1536;

// 		font_8x16_draw_text(bot_msg_x_pos, SCREEN_HEIGHT - 64, " Hello, World! ", hue_to_color(hue2), BLACK);
// 		hue2 = (hue2 + 1) % 1536;

// 		int bump = 0;
// 		px += speed_x;
// 		if (px < 0) {
// 			px = -px;
// 			speed_x = -speed_x;
// 			bump = 1;
// 		} else if (px + soccer_ball.width > SCREEN_WIDTH) {
// 			px = 2*(SCREEN_WIDTH - soccer_ball.width) - px;
// 			speed_x = -speed_x;
// 			bump = 1;
// 		}
// 		py += speed_y;
// 		if (py < 0) {
// 			py = -py;
// 			speed_y = -speed_y;
// 			bump = 1;
// 		} else if (py + soccer_ball.height > SCREEN_HEIGHT) {
// 			py = 2*(SCREEN_HEIGHT - soccer_ball.height) - py;
// 			speed_y = -speed_y;
// 			bump = 1;
// 		}
// 		draw_sprite(px, py, &soccer_ball);

// 		draw_line(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GREEN);

// 		if (bump) {
// 			Mix_PlayChannel(-1, sound_samples[sample_num], MIX_MAX_VOLUME);
// 			sample_num = (sample_num + 1) % (NB_SOUND_SAMPLES - 4);
// 		}
// 	}
// }


// void init_video()
// {
// 	video_refresh_semaphore = xSemaphoreCreateBinary();

// 	VIDEO->WIDTH  = SCREEN_WIDTH;
// 	VIDEO->HEIGHT = SCREEN_HEIGHT;
// 	VIDEO->DMA_ADDR = framebuffer;
// 	memset(framebuffer, 0, sizeof(framebuffer));
// 	VIDEO->CR = VIDEO_CR_EN | VIDEO_CR_IE;
// 	minirisc_enable_interrupt(VIDEO_INTERRUPT);
// }


// int main()
// {
// 	init_uart();

// 	init_audio_mixer(4);
// 	init_video();

// 	xTaskCreate(hello_task, "hello", 1024, NULL, 1, NULL);

// 	vTaskStartScheduler();

// 	return 0;
// }

// /* Hue must be between 0 and 1536 not included */
// uint32_t hue_to_color(unsigned int hue)
// {
// 	uint32_t r, g, b;
// 	if (hue < 256) {
// 		r = 255;
// 		g = hue;
// 		b = 0;
// 	} else if (hue < 512) {
// 		r = 511 - hue;
// 		g = 255;
// 		b = 0;
// 	} else if (hue < 768) {
// 		r = 0;
// 		g = 255;
// 		b = hue - 512;
// 	} else if (hue < 1024) {
// 		r = 0;
// 		g = 1023 - hue;
// 		b = 255;
// 	} else if (hue < 1280) {
// 		r = hue - 1024;
// 		g = 0;
// 		b = 255;
// 	} else if (hue < 1536) {
// 		r = 255;
// 		g = 0;
// 		b = 1535 - hue;
// 	} else {
// 		r = 0;
// 		g = 0;
// 		b = 0;
// 	}
// 	return 0xff000000 | (r << 16) | (g << 8) | b;
// }


