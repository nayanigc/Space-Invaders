GEORGE CLEMENT Nayani U21402914
RACERLYN Yersenia U21516548

RAPPORT DE PROJET SYSTEME

Dans l'élaboration de notre projet, nous avons reparti les taches de la manière suivante : 
	- L'une d'entre nous s'est occupée des fonctions du fichiers load et main 
	- L'autre s'est occupée des fonctions du fichier menu et Makefile
	- Puis ensemble nous avons codé les fonctions du fichier game

Comment fonctionne notre jeu?
	On commence par choisir le mod que l'on souhaite jouer. puis on lance le premier niveau et si on gagne on passe au niveau suivant et ainsi de suite. Cependant si on perd (niveau de vie égale à 0 ou collision avec un vaisseau ennemi) le jeu s'arrête.
La collision entre 2 vaisseaux ennemi n'entraine pas la disparition de ceux-ci (nous n'avons pas traité ce cas).

Nous lançons le jeu niveau par niveau car nous avons oublié de prendre en compte le fichier déroulement.
Cependant cela marche tout aussi bien sans celui-ci.

L'un de nos principal problème est la gestion de la memoire (liste des vaisseaux et des shots) , on aurait pu utiliser des listes chainées sauf qu'à la place nous avons utilisé des tableaux. On ajoute et on enlève des éléments très souvent donc ce n'est pas très pratique.

De plus, dans un premier temps, pour faire le menu, nous avons parcourru le répertoire "mods" de façon récursive, sauf que cela ne permettait pas de recupérer facilement la structure mod que nous avons créer afin de lancer le jeu. Nous avons donc alléger le code en parcourrant directement chaque répertoire du répertoire "mods".

Pour finir, le plus dur à faire a été la fonction play qui nous a pris plus de trois bonne semaine.

On a réussit à faire en sorte que les vaisseaux ennemis puissent se déplacer selon ce qui est décrit dans le fichier vaisseaux (cycle de déplacement) et tirer vers le bas. Notre vaisseau lui il tire vers le haut et se déplace uniquement de gauche à droite.
On a aussi fait en sorte que s'il y a collision entre le vaisseau ennemie et le notre le jeu s'arrête.
Le niveau de vie de chaque vaisseaux diminue lorsqu'il est touché par un shot.


