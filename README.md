# Space Breaker

Space Breaker est un jeu de casse-briques développé en C utilisant la bibliothèque SDL2. Le joueur contrôle une raquette pour renvoyer une balle et détruire des briques. Le jeu propose plusieurs niveaux avec des difficultés croissantes.

## Fonctionnalités du jeu

- Contrôle de la raquette avec les touches fléchées gauche et droite.
- Balle rebondissante détruisant les briques.
- Plusieurs niveaux avec des configurations de briques variées.
- Effets sonores pour les interactions du jeu.

## Bibliothèques utilisées

Le jeu utilise les bibliothèques suivantes :

- **SDL2** : pour la gestion de la fenêtre, des événements et du rendu graphique.
- **SDL2_image** : pour le chargement des images.
- **SDL2_mixer** : pour la gestion des effets sonores et de la musique.
- **SDL2_ttf** : pour l'affichage de textes avec des polices TrueType.

## Installation des bibliothèques

Pour installer les bibliothèques nécessaires sur un système basé sur Debian/Ubuntu, exécutez les commandes suivantes :

```bash
sudo apt update
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
