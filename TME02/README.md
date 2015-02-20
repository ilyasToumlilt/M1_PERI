#TP02 - Outils de dev et GPIO
######version 1.1

**== Participants : 2 ==**

   HOSPITAL Alexandra - 3401862 - <alexandra.hospital@etu.upmc.fr>
   TOUMLILT Ilyas     - 3261538 - <toumlilt.ilyas@gmail.com>
   
**== Fichiers inclus ==**

   EXO_1 :
   -> src/hello_world.c : programme de prise en main, affiche simplement 
      			  un hello world sur la sortie standard.

   EXO_2 :
   -> src/lab1.c : Fait clignoter 10 fois la led sur GPIO04
      		   pour une période de fréquence passée en paramètre
		   ( 1Hz par défaut si pas de paramètre ).
   EXO_3 :
   -> src/lab2.c : Fait clignoter 10 fois les 4 LEDs

   EXO_4 :
   -> src/lab3.c : Configure les deux GPIOs des deux BTN en entrée puis 
      		   affiche leurs valeurs dans une boucle d'échantillonnage.

   EXO_5 :
   -> src/gpio_setup.c : configurations mémoire, mmap / munmap.
   -> src/gpio_config.c : initialisation des GPIO ( champ FSEL ), pour les I/O.
   -> src/gpio_value.c : implémentations des fonctions d'I/O depuis les GPIO.
   -> **POINT FORT** : la configuration marche pour les deux version de RPi
      	    	   il détecte la version et configure l'adresse de base
		   en fonction de la révision.

   EXO_6 :
   -> src/lab4.c : Alors pour nous amuser on utilisera deux LEDs et les deux BTNs
      		   pour implémenter un automate déterministe qui se base 
 		   sur les états des noeuds.
 		   Chaque bouton s'occupe d'allumer/éteindre une LED.
 		   Le programme s'arrête quand on appuie sur les deux BTNS.

**== Directives de 'makefile' ==**

   all : monsieur fait tout.

   hello_world: cc + ld exo1
   lab1: cc + ld exo2
   lab2: cc + ld exo3
   lab3: cc + ld exo4
   lab4: cc + ld exo6

   upload : charge les executables sur la RPi.
   upload-home : upload chez moi.

   directories : création des répertoires temporaires s'ils n'existent pas déjà
   clean : Suppression de tous les fichiers binaires, objet et bibliothèques.
   cleanall : Suppression des répertoires et fichiers temporaires

**== Etat d'avancement ==**

   EXO 1 : Hello World                  - OK
   EXO 2 : Controle de GPIO en sortie   - OK
   EXO 3 : Contrôler plusieurs LEDs     - OK
   EXO 4 : Contrôle des GPIOs en entrée - OK
   EXO 5 : Manipulation des registres   - OK
   EXO 6 : Amusez-vous !                - OK

**== Réponses ==**

   EXO_02 :
   -> On observe que plus on augmente la fréquence ( réduit la période en param )
      plus vite la led clignote.

   EXO_04 :
   -> On observe que l'entrée est effectivemment égale à 0 quand on appuie 
      sur le bouton et 1 sinon ( Pull Up ). Que la fréquence fait qu'on a 
      plusieurs 0 quand on appuie.
   -> Fréquence d'échantillonnage minimale si on appuie 10 fois par seconde,
      sera le double, c'est à dire 20Hz 
   -> Pour écrire un programme qui échantillonne avec une fréquance égale à 100Hz,
      il faut faire un sleep de 100 millisecondes entre chaque deux lectures.
      ( usleep(100*1000) dans le code, en microsecondes ). 