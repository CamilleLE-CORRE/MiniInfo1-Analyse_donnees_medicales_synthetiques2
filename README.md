# MiniInfo1-Analyse_donnees_medicales_synthetiques

Ce fichier détaille les commandes à exécuter pour tester notre code.
Le rapport se trouve dans le fichier Rapport_LE-CORRE_LEFEVRE.pdf du dossier.

Le projet a été réalisé en C et est composé de 11 fichiers :
- main.c : fichier principal, il utilise les fonctions des autres fichiers
- getData.c (et getData.h) : fonctions pour lire les fichiers patients.pengu et livestyle.pengu et stocker les données dans une structure de données
- sampling.c (et sampling.h) : fonctions pour échantilloner les données (les patients) en deux jeux de données distincts (entraînement et test)
- perceptron.c (et perceptron.h) : fonctions pour entraîner le perceptron, tester les données, calculer l'accuracy et afficher les résultats
- perceptronBonus.c (et perceptronBonus.h) : entraînement du perceptron a 7 dimensions, test et calcul de l'accuracy
- kmeans.c (et kmeans.h) : algorithme des k-means

Ainsi, pour tester le code, il suffit de télécharger ces fichiers puis exécuter les commandes suivantes dans un terminal :
gcc main.c getData.c sampling.c perceptron.c perceptronBonus.c kmeans.c -o programme -lm
./programme

Tous les "print" pour observer les résultats du fichier main.c ont été mis en commentaires pour ne pas surcharger l'affichage sur le terminal. Vous pouvez décommenter pour analyser les résultats fonction par fonction.