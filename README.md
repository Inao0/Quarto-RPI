# Quarto-RPI
Jeu du quarto pour la malette joyPi en utilisant la matrice de boutons comme entrée.

# Solutions techniques utilisées :
-> Ncurses pour les menus
-> GPIO pour la matrice de bouton ?
-> SDL pour le plateau et les pions ?

# User story :
-> L'utilisateur allume la machine
-> Le RPI démarre avec le menu du quarto en plein écran
	options disponibles :
	- nouvelle partie locale 
	- *Nouvelle partie en ligne 2ème temps*
	- Afficher le règles
	- About
	*- Bouton quitter temporaire pour le développement*

-> Nouvelle partie locale
	*-> Entrer le nom des joueurs ?*
	-> Le premier joueur choisi une pièce
	-> Le deuxième joueur place la pièce et choisi la seconde
	-> Le premier joueur la place et choisi la troisième...
	-> etc.
	-> Lorsqu'un alignement de est détecter afficher un écran de victoire (*mettre en valeur l'alignement ?*)
	-> Boutons Rejouer ? / Menu principal

-> Nouvelle partie en ligne
	-> Entrer le nom du joueur
	-> Proposer ou se mettre en attente ?
	-> Si proposer : Entrer l'addresse ip du joueur avec qui vous souhaiter jouer (client)
	-> Sinon on écoute pour une proposition de partie. (serveur)
	-> Tir au sort du premier joueur.
	-> Cf partie local
