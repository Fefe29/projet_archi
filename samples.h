#ifndef SOUND_SAMPLES_H
#define SOUND_SAMPLES_H

#include <stdint.h>

typedef struct {
    const int16_t *samples;  // Pointeur vers les données audio
    uint32_t nb_samples;     // Nombre total d'échantillons
    uint32_t frequency;      // Fréquence d'échantillonnage
} sound_sample_t;
extern const sound_sample_t sound_sample_beep;  // Déclaration externe

#endif /* SOUND_SAMPLES_H */
