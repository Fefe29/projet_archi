uint32_t make_color(uint8_t red, uint8_t green, uint8_t blue);

// fonctions.h
void dessiner_boule_noel(uint32_t *framebuffer, uint32_t largeur, uint32_t hauteur, int16_t pos_x, int16_t pos_y, uint32_t couleur);
void dessiner_etoile_noel(uint32_t *framebuffer, uint32_t largeur, uint32_t hauteur, int16_t pos_x, int16_t pos_y, uint32_t couleur);

// void jouer_musique_noel(audio_device_t *audio_device, const sound_sample_t *sample, int channel, uint8_t volume);
