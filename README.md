# MiniInfo1-Analyse_donnees_medicales_synthetiques

Ce fichier détaille les commandes à exécuter pour tester notre code.
Le rapport se trouve dans le fichier pdf du dossier.

Le projet a été réalisé en C et est composé de 9 fichiers :
- main.c : fichier principal, il utilise les fonctions des autres fichiers
- getData.c (et getData.h) : fonctions pour lire les fichiers patients.pengu et livestyle.pengu et stocker les données dans une structure de données
- sampling.c (et sampling.h) : fonctions pour échantilloner les données (les patients) en deux jeux de données distincts (entraînement et test)
- perceptron.c (et perceptron.h) : fonctions pour entraîner le perceptron, tester les données, calculer l'accuracy et afficher les résultats
- k-means.c (et k-means.h) :

Ainsi, pour tester le code, il suffit de télécharger ces fichiers puis exécuter les commandes suivantes dans un terminal :
gcc main.c getData.c sampling.c perceptron.c k-means.c -o programme
./programme