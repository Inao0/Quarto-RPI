# Quarto-RPI
Jeu du quarto pour la malette joyPi en utilisant la matrice de boutons comme entrée.

# Solutions techniques utilisées :
- Ncurses pour les menus
- GPIO pour la matrice de bouton ?
- SDL pour le plateau et les pions ?

# User story :
- L'utilisateur allume la machine
- Le RPI démarre avec le menu du quarto en plein écran
	options disponibles :
	1. nouvelle partie locale 
	2. *Nouvelle partie en ligne 2ème temps*
	3. Afficher le règles
	4. About
	5. *Bouton quitter temporaire pour le développement*

- Nouvelle partie locale
	1. *Entrer le nom des joueurs ?*
	2. Le premier joueur choisi une pièce
	3. Le deuxième joueur place la pièce et choisi la seconde
	4. Le premier joueur la place et choisi la troisième...
	5. etc.
	6. Lorsqu'un alignement de est détecter afficher un écran de victoire (*mettre en valeur l'alignement ?*)
	7. Boutons Rejouer ? / Menu principal

- Nouvelle partie en ligne
	1. Entrer le nom du joueur
	2. Proposer ou se mettre en attente ?
	3. Si proposer : Entrer l'addresse ip du joueur avec qui vous souhaiter jouer (client)
	4. Sinon on écoute pour une proposition de partie. (serveur)
	5. Tir au sort du premier joueur.
	6. Cf partie local
