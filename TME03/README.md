#TP03 - Premier pilote
######version 1.0

**== Participants : 2 ==**

   HOSPITAL Alexandra - 3401862 - <alexandra.hospital@etu.upmc.fr>
   TOUMLILT Ilyas     - 3261538 - <toumlilt.ilyas@gmail.com>
   
**== Fichiers inclus ==**

   EXO_1 :
   -> src/module.c : Un module minimal se composant d'une fonction
      		     d'initialisation et d'une fonction de cleanup.
		     Fourni avec le TP.

   EXO_2 :
   -> src/module_param.c : Le meme module minimal avec prise en compte des
      			   arguments pour le nombre de LEDs et de BTNs.

   EXO_3 :
   -> src/ledbtn_lite.c : Module de test des opérations d'entrées sorties.

   EXO_4 :
   -> src/ledbtn.c : Accès aux GPIOs depuis les fonctions du pilote,
      		     ce driver gère les GPIOs depuis le mode user en utilisant 
		     les opérations d'entrée sortie.
		     Pour écrire vers les LEDs il suffit d'envoyer une chaine
		     binaire, par ex : 0101 pour 4 leds
		     Pour lire la valeur des BTNs il suffit du faire un read.
		     Les arguments sont importants pour ce module, et définissent
		     le nombre de LEDs et BTNs, le module ne s'insère pas si
		     au moins l'un des deux n'est pas présent. 

   RAPPORT :
   -> report/rapport.pdf : compte rendu de la semaine.

**== Directives de 'makefile' ==**

   all : monsieur fait tout.

   module: cross compile tous les modules du tp.   

   upload : charge l'exec sur le rpi.
   upload-home : upload chez moi.

   directories : création des répertoires temporaires s'ils n'existent pas déjà
   clean : Suppression de tous les fichiers binaires, objet et bibliothèques.
   cleanall : Suppression des répertoires et fichiers temporaires

**== Etat d'avancement ==**

   EXO 1 : Création et test d'un module Noyau            - OK
   EXO 2 : Ajout des paramètres au module                - OK
   EXO 3 : Module qui ne fait rien mais dans le Noyau    - OK
   EXO 4 : Accès aux GPIO depuis les fonctions du pilote - OK

**== Réponses ==**
   Pas de questions cette semaine =) 

**== Links ==**
- TP : https://www-soc.lip6.fr/trac/sesi-peri/wiki/SujetTP3-2015
- http://www.chicoree.fr/w/Compilation_crois%C3%A9e_d%27un_module_Linux_pour_Rasberry_Pi
- http://elinux.org/Raspberry_Pi_Kernel_Compilation
- http://stackoverflow.com/questions/17560989/insmod-failing-to-insert-a-really-simple-module
- http://linux.die.net/lkmpg/x44.html