#include "samples.h"
#include <math.h>  // Pour utiliser la fonction sin()

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SAMPLE_RATE 24000  // Fréquence d'échantillonnage en Hz
#define FREQUENCY 440      // Fréquence de la note en Hz (440 Hz = La4)
#define DURATION 1         // Durée du son en secondes
#define AMPLITUDE 30000    // Amplitude maximale de l'onde

int16_t beep_data[(SAMPLE_RATE * DURATION)] = {
    [0 ... (SAMPLE_RATE * DURATION) - 1] = 0  // Initialisation pour compiler
};

// Générer les données audio
void generate_sine_wave() {
    for (int i = 0; i < SAMPLE_RATE * DURATION; i++) {
        beep_data[i] = (int16_t)(AMPLITUDE * sin(2 * M_PI * FREQUENCY * i / SAMPLE_RATE));
    }
}



const sound_sample_t sound_sample_beep = {
    .samples = beep_data,
    .nb_samples = sizeof(beep_data) / sizeof(beep_data[0]),
    .frequency = 24000  // Fréquence en Hz
};