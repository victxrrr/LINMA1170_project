<h1 align="center">
  <br>
  LINMA1170 - Analyse numérique
  <br>
</h1>

<h4 align="center">Projet : Optimisation de forme d'un diapason

<p align="center">
  <a href="#contexte">Contexte</a> •
  <a href="#usage">Usage</a> •
  <a href="#architecture">Architecture</a> •
  <a href="#auteurs">Auteurs</a> •
  <a href="#ressources">Ressources</a>
</p>

## Contexte

Il nous a été demandé de concevoir une pièce mécanique dont la fréquence propre correspond à une note donnée. La note de musique que nous avons choisie est le La de la sixième octave qui correspond à une fréquence de 1760Hz.

## Usage

L'algorithme d'optimisation de forme de diapason implémenté utilise l'interface C de la librairie LAPACK. *Sous Linux*, les commandes suivantes permettent d'installer l'ensemble des bibliothèques réquises. 
```bash
$ sudo apt-get install libblas-dev liblapack-dev liblapacke-dev
```
Pour tout autre système d'exploitation, le lecteur est invité à se renseigner auprès de la documentation du constructeur.

Le répertoire contient deux `main` principaux définis respectivement dans `optimize.c` et `mode.c`. Le premier script sert à usiner la forme de diapason en fonction des contraintes souhaitées. La fonction objective peut y être modifiée ainsi que l'algorithme employé et les bornes sur les valeurs des différents paramètres de design. Le facteur de maillage (constante `MESH_SIZE_FACTOR`) et la géométrie peuvent aussi être choisies. Nous proposons des valeurs par défaut pour illustrer le fonctionnement du code (méthode de Nelder-Mead + calcul de la quinte). Des valeurs commentées sont aussi données pour d'autres choix de géométries et d'algorithme.
Il est important de noter que le fichier `optimize.c` a été conçu pour le calcul et l'optimisation de fréquences propres uniquement (pour des raisons d'efficacité). Pour exécuter le fichier, la cible du Makefile correspondante est 
```bash
$ make
```
Pour afficher les modes de vibrations propres, l'utilisateur peut exécuter le fichier `mode.c` via la cible 
```bash
$ make mode
```
Les paramètres optimaux seront écrits dans un fichier du dossier `files` donné en argument de `optimize.c` qui sera ensuite lu par `mode.c`. Ce dernier prend donc deux arguments, le nombre de modes de vibration et calculer ainsi que le fichier contenant les paramètres de design. **Des jeux de paramètres optimaux précédemment calculés sont disponibles pour tester le calcul des fréquences propres et l'affichage des modes propres.**
> **Note**
> L'affichage des modes propres (post-processing) ne nécessitent pas la librairie LAPACKE. Seul le code de calcul la requiert.

## Architecture

Le dossier `geometries` contient les fichiers définissant les différentes géométries disponibles. Au sein de fichier `design.c`, un grand nombre de forme de diapasons sont définies sur lesquelles il est possible d'appliquer les algorithmes d'optimisations.

Dans `dichotomous.h` sont définies plusieurs algorithmes de résolution d'équations tels que la méthode de la bissection ou la méthode de la sécante. L'algorithme de Nelder-Mead est implémenté dans `nelderMead.h`.

## Auteurs

Ce répertoire a été entièrement implémenté par Alexandre Nicolas et Victor Lepère dans le cadre du cours d'*Analyse numérique* dispensé par Pr. Jean-François Remacle.

## Ressources

* [Moodle](https://moodle.uclouvain.be/course/view.php?id=5504) - Site du cours

---

> GitHub [Projet](https://github.com/victxrrr/LINMA1170_project) &nbsp;&middot;&nbsp;
> [Alexandre Nicolas](mailto:alexandre.nicolas@student.uclouvain.be) &nbsp;&middot;&nbsp;
> GitHub [@alxwncl](https://github.com/alxwncl) &nbsp;&middot;&nbsp;
> [Victor Lepère](mailto:victor.lepere@student.uclouvain.be) &nbsp;&middot;&nbsp;
> GitHub [@victxrrr](https://github.com/victxrrr)