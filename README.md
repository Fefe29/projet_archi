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

### 3. **Utilisation du mixeur audio (FreeRTOS)**
   - Le projet utilise le **port FreeRTOS** et son mixeur audio pour jouer un morceau de musique ("Petit Papa Noël"). Chaque note est jouée dans un thread dédié avec une gestion précise des durées, garantissant une synchronisation fluide.

### 4. **Architecture modulaire**
   - Le projet suit une architecture claire et modulaire :
     - **`main.c`** : Point d'entrée, gestion principale.
     - **`samples.c`** : Génération et stockage des échantillons audio.
     - **`fonctions_periph.c`** : Gestion des événements périphériques (clavier, souris).
     - **`audio_server.c`** : Gestion des sons avec le mixeur audio.

### 5. **Agilité dans la démarche**
   - Le développement a été réalisé de manière itérative, en testant et en validant chaque fonctionnalité avant de passer à la suivante :
     1. Initialisation des périphériques.
     2. Affichage du sapin avec animations.
     3. Gestion des événements souris et clavier.
     4. Intégration de la musique et des interruptions.

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
- **`fonctions_periph.c`** : Gestion des périphériques (souris, clavier).
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

### Débogage

Pour déboguer avec GDB :

```sh
make gdb2
```

---

## Références

- **Partition de Petit Papa Noël** :  
  [Partition simplifiée](https://apprendre-a-jouer-du-piano.com/petit-piano-noel-quand-tu-descendras-du-ciel/partition-de-petit-papa-noe%CC%88l/)

- **Tutoriel Mini-RISC** :  
  [Documentation officielle Mini-RISC](https://gitlab.ensta-bretagne.fr/mini-risc/)

---

Ce projet respecte l'ensemble des consignes et démontre une maîtrise des périphériques, des interruptions et de l'audio sur Mini-RISC.
