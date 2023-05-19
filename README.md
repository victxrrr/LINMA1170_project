<h1 align="center">
  <br>
  LINMA1170 - Analyse numérique
  <br>
</h1>

<h4 align="center">Projet : Optimisation de forme d'un diapason

<p align="center">
  <a href="#contexte">Contexte</a> •
  <a href="#usage">Usage</a> •
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



```c
femProblem *femElasticityCreate(femGeo* theGeometry,double E, double nu, double rho, double g, femElasticCase iCase, femSolverType solver, femRenumType renum);
```

> **Note**
> Les normales aux surfaces sont orientées vers la concavité de la courbe frontière. 


```bash
$ make
$ ./myFem
```

## Auteurs

Ce répertoire a été entièrement implémenté par Félix Ruyffelaere et Victor Lepère dans le cadre du cours d'*Eléments Finis* dispensé par Pr. Vincent Legat et Pr. Jean-François Remacle.

## Ressources

* [zouLab](https://perso.uclouvain.be/vincent.legat/zouLab/epl1110.php?action=doc) - Site du cours

---

> GitHub [Projet](https://github.com/FelixRuy/Project-LEPL1110) &nbsp;&middot;&nbsp;
> [Félix Ruyffelaere](mailto:felix.ruyffelaere@student.) &nbsp;&middot;&nbsp;
> GitHub [@FelixRuy](https://github.com/FelixRuy) &nbsp;&middot;&nbsp;
> [Victor Lepère](mailto:victor.lepere@student.uclouvain.be) &nbsp;&middot;&nbsp;
> GitHub [@victxrrr](https://github.com/victxrrr)