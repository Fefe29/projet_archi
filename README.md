# Mini-RISC Samples

## Description du projet

Ce projet met en œuvre plusieurs fonctionnalités démontrant l'utilisation des périphériques du Mini-RISC, tout en respectant les consignes données. Il inclut un **mixeur audio**, des éléments graphiques pour l'affichage, ainsi qu'une **interactivité via clavier et souris**. Le tout est présenté sous la forme d'une application ludique avec des animations et la musique de **"Petit Papa Noël"**.


## Points d'intéret

### 1. **Utilisation des périphériques**
   - **Contrôleur vidéo** : Le projet utilise l'écran pour afficher des messages dynamiques et des graphismes festifs, comme un sapin, des boules, des cadeaux et une étoile.
   - **Clavier** : Les événements clavier permettent de changer la couleur des graphismes et d'interrompre la musique avec la touche espace.
   - **Souris** : Les clics de souris permettent d'ajouter des éléments visuels (boules, cadeaux, étoile).

### 2. **Gestion des interruptions**
   - Les interruptions sont utilisées pour la gestion des événements clavier et souris. Par exemple, la musique peut être interrompue à tout moment en appuyant sur la barre d'espace, ce qui démontre une bonne intégration des interruptions matérielles.

### 3. **Utilisation des sémaphores**
   Les sémaphores sont utilisées pour synchroniser les opérations d'affichage avec les interruptions vidéo, garantissant ainsi un rafraîchissement fluide de l'écran.

   - **Création** :
     Une sémaphore binaire est créée dans la fonction `init_video` pour gérer les interruptions liées au contrôleur vidéo :
     ```c
     video_refresh_semaphore = xSemaphoreCreateBinary();
     ```

   - **Signalement** :
     Lorsqu'une interruption vidéo survient, la fonction `video_interrupt_handler` libère la sémaphore pour indiquer qu'une nouvelle image peut être affichée :
     ```c
     void video_interrupt_handler() {
         BaseType_t xHigherPriorityTaskWoken = pdFALSE;
         VIDEO->SR &= ~VIDEO_SR_SRF_P;
         xSemaphoreGiveFromISR(video_refresh_semaphore, &xHigherPriorityTaskWoken);
         portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
     }
     ```

   - **Attente** :
     Bien que la synchronisation via sémaphore soit prévue, elle est actuellement commentée dans la fonction `affichage_dyn`. Pour une synchronisation complète, cette ligne peut être activée :
     ```c
     // xSemaphoreTake(video_refresh_semaphore, portMAX_DELAY);
     ```

   Cette mécanique garantit que chaque opération d'affichage attend la fin d'une interruption vidéo avant de se lancer, évitant ainsi les artefacts visuels.


### 4. **Utilisation du mixeur audio (FreeRTOS)**
   - Le projet utilise le **port FreeRTOS** et son mixeur audio pour jouer un morceau de musique ("Petit Papa Noël"). Chaque note est jouée dans un thread dédié avec une gestion précise des durées, garantissant une synchronisation fluide.

### 5. **Architecture modulaire**
   - Le projet suit l'architecture suivante :
     - **`main.c`** : Point d'entrée, gestion principale.
     - **`samples.c`** : Génération et stockage des échantillons audio.
     - **`fonctions_periph.c`** : Gestion des événements périphériques (clavier, souris).
     - **`audio_server.c`** : Gestion des sons avec le mixeur audio.

### 6. **Agilité dans la démarche**
   - Le développement a été réalisé de manière itérative, en testant et en validant chaque fonctionnalité avant de passer à la suivante :
     1. Initialisation des périphériques.
     2. Affichage du sapin avec animations.
     3. Gestion des événements souris et clavier.
     4. Intégration de la musique et des interruptions.

### 7. **Deux threads**
    - Un pour l'affichage
    - Un pour la musique

---

## Fonctionnalités principales

1. **Graphismes dynamiques** :
   - Animation d'un message défilant ("Joyeux Noël la team!") sur l'écran.
   - Affichage d'un sapin avec des décorations interactives.

2. **Musique festive** :
   - Lecture de **"Petit Papa Noël"** avec des notes précises et synchronisées.
   - Possibilité d'interrompre la musique en appuyant sur la barre d'espace.

3. **Interactivité utilisateur** :
   - **Clavier** : Modification des couleurs via les touches alphabétiques, interruption de la musique avec espace.
   - **Souris** : Ajout d'éléments visuels en fonction des clics.

---

## Structure du projet

- **`.vscode/settings.json`** : Paramètres de l'environnement de développement.
- **`audio_server.c`** / **`audio_server.h`** : Gestion du mixeur audio.
- **`fonctions_periph.c`** : Gestion des périphériques (souris, clavier) + ajout des fonctions utilitaires de dessins des boules, étoiles, cadeaux etc.
- **`font.c`** / **`font.h`** : Affichage de texte sur l'écran.
- **`samples.c`** / **`samples.h`** : Données audio et génération des notes.
- **`main.c`** : Point d'entrée du programme.
- **`Makefile`** : Automatisation de la compilation.
- **`README.md`** : Documentation du projet.

---

## Compilation et exécution

### Compilation

Pour compiler le projet, utilisez le `Makefile` fourni :

```sh
make all
```

### Exécution

Pour exécuter le projet sur l'émulateur Mini-RISC :

```sh
make exec
```


## Utilisation

Une fois le programme lancer avec make exec, une fenètre s'affiche. Elle n'est pas active tant que la musique n'a pas démarrée (chargement du programme). Une fois que la musique est lancée, vous pouvez:
- **arrêter la musique** en pressant la barre d'espace de votre clavier (ne pas hésiter à appuyer longtemps si la musique ne s'arrête pas)
- **dessiner une boule de Noel** en faisant un clic gauche avec votre souris à l'endroit où vous voulez la placer
- **dessiner un cadeau** en faisant un clic droit sur votre souris avec votre souris à l'endroit où vous voulez le placer
- **dessiner une étoile** en faisant un clic molette avec votre souris avec votre souris à l'endroit où vous voulez la placer
- **changer la couleur** de vos prochains dessins en tapant sur une touche alphabétique de votre clavier (chaque touche correspondant à une couleur). La couleur choisie s'affiche dans le rectangle en haut à droite de la page d'affichage
---

## Ce qui peut être amélioré

- **Mettre les interruptions sur des threads propres**
- **Utiliser sprite pour générer l'image du sapin de Noel**

## Références

- **Partition de Petit Papa Noël** :  
  [Partition simplifiée](https://apprendre-a-jouer-du-piano.com/petit-piano-noel-quand-tu-descendras-du-ciel/partition-de-petit-papa-noe%CC%88l/)

- **Tutoriel Mini-RISC** :  
  [Documentation officielle Mini-RISC](https://gitlab.ensta-bretagne.fr/mini-risc/)

---


## Remarques complémentaires

   - Pour toute demande complémentaire, n'hésitez pas à m'envoyer un mail à felicien.moquet@ensta.fr.
   - N'hésitez pas à stopper le son avec la barre d'espace. Cela casse un peu les oreilles à la longue ;)


