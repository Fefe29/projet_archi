#ifndef SAMPLES_H
#define SAMPLES_H

#include <stdint.h>

typedef struct {
    const int16_t *samples;  // Pointeur vers les données audio
    uint32_t nb_samples;     // Nombre total d'échantillons
    uint32_t frequency;      // Fréquence d'échantillonnage
} sound_sample_t;

// Fréquences des notes de la gamme
extern const float note_frequencies[];

// Structure des échantillons audio pour chaque note
extern sound_sample_t sound_samples[];

// Fonction pour générer une gamme complète
void generate_scale();

#endif /* SAMPLES_H */