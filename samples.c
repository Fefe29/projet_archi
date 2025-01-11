#include <math.h>
#include <stdint.h>
#include "samples.h"  // Assurez-vous que cette déclaration est correcte

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SAMPLE_RATE 13000  // Fréquence d'échantillonnage en Hz
#define DURATION 1         // Durée de chaque note en secondes
#define AMPLITUDE 30000    // Amplitude maximale de l'onde sinusoïdale

// Fréquences des notes de la gamme majeure de Do
const float note_frequencies[] = {
    261.63,  // Do
    293.66,  // Ré
    329.63,  // Mi
    349.23,  // Fa
    392.00,  // Sol
    440.00,  // La
    493.88,  // Si
    523.25   // Do5
};

#define NUM_NOTES (sizeof(note_frequencies) / sizeof(note_frequencies[0]))

// Tableau pour stocker les données audio de chaque note
int16_t note_data[NUM_NOTES][SAMPLE_RATE * DURATION];

// Tableau pour stocker les structures des échantillons audio
sound_sample_t sound_samples[NUM_NOTES];

/**
 * Génère une onde sinusoïdale pour une fréquence donnée.
 */
void generate_note_wave(int16_t *buffer, float frequency, int sample_rate, int duration, int amplitude) {
    for (int i = 0; i < sample_rate * duration; i++) {
        buffer[i] = (int16_t)(amplitude * sin(2 * M_PI * frequency * i / sample_rate));
    }
}

/**
 * Génère les données audio pour toutes les notes de la gamme.
 */
void generate_scale() {
    for (int i = 0; i < NUM_NOTES; i++) {
        generate_note_wave(note_data[i], note_frequencies[i], SAMPLE_RATE, DURATION, AMPLITUDE);
        sound_samples[i].samples = note_data[i];
        sound_samples[i].nb_samples = SAMPLE_RATE * DURATION;
        sound_samples[i].frequency = SAMPLE_RATE;
    }
}
