#TP04 - Pilotage d'un écran LCD en mode utilisateur.
######version 1.0

**== Participants : 2 ==**

   HOSPITAL Alexandra - 3401862 - <alexandra.hospital@etu.upmc.fr>
   TOUMLILT Ilyas     - 3261538 - <toumlilt.ilyas@gmail.com>

**== Description ==**
   Un seul fichier source est fournis pour ce TP, il s'agit de src/lcdRpi.c.
   Pour réaliser ce TP j'ai utilisé mon propre matériel, à savoir une RPi v2
   ( le code fournis fonctionne également pour une RPi v1 ), un LCD HD44780
   de version 16x2, contrairement à ceux du TP qui contiennent 4lignes.

   **GPIO Pins :**
   J'ai respecté le raccordement du TP, à savoir :
   -> RS: GPIO 7
   -> E : GPIO 8
   -> D4: GPIO 22
   -> D5: GPIO 23
   -> D6: GPIO 24
   -> D7: GPIO 25

   **GPIO Operations :**
   J'ai repris les fonctions écrites aux TMEs 2 et 3, pour le contrôl des GPIOs,
   à savoir la structure pour le map des registres, la fonction de configuration,
   de choix d'adresse de base par rapport à la version du RPi, l'initialisation
   en sortie des registres, ainsi que l'écriture des données dessus.

   **LCD's basic instructions :**
   La troisième partie du code contient des definitions de toutes les instructions
   de base du LCD à leur format binaire. Ces instructions sont tirées de la doc.

   **LCD's operations :**
   Cette partie contient les fonctions essentielles pour le control du LCD.
   binaryOR : un OR binaire entre deux chaines de taille 8.
   decimal2binary : converti un entier en chaine binaire de taille 8 ( max )
   lcd_strobe : génère un signal E.
   lcd_write4bits : écriture en mode 4bits.
   lcd_command : envoie d'une commande vers le LCD.
   lcd_data : envoi d'une donnée vers l'ecran LCD.
   lcd_init : execute les commandes d'initialisation.
   lcd_clear : clear display important avant ecriture.
   lcd_message : écriture d'un message sur l'ecran

   ** main :**
   Le programme prend en argument la chaîne à écrire sur l'ecran.
   
**== Fichiers inclus ==**

   EXO_1 :
   -> src/lcdRpi.c : Fichier présenté ci-dessus. 

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

   EXO 1 : lcdRpi.c - OK

**== Links ==**
  La doc linkée dans l'énoncé du TP est suffisante.
  J'ai utilisé le tuto Adafruit pour les branchements à la maison.