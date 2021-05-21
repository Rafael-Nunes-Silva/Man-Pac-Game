# Man-Pac-Game
A mix of PAC-MAN and Bomberman made as a project to learn C, where the player(P) moves through the map and and collects bombs(@) to kill the enemies(&) withou getting killed.

Controls:
Movement:
 W
ASD
B - Bombs

How to make a map:

Example:
5 10
##########
#,,,@#P#&#
#,####,#,#
#,,,,,,,,#
##########

- the first number refers to the map height
- the second number refers to the map width

the map's file name should be as "Map_0.txt", where the 0 is the number of the map wich you'll use to select the map at the start of the game.

Symbols:
# => walls
, => walkable space
P => player
& => enemy
@ => bomb
