
C++ Project

Platform game


Project is done in C++ with Allegro library for graphics and sound. 

The objective of game is to find out the way to the chest, and open it to go to upper level.

Using teleport or key you can unlock the way where you couldn’t go otherwise.

Map is loaded from separate text file formed of numbers that represent the tiles to which screen is divided. Map loader is build that way so adding new maps will add new levels to the game with no need of changing the code.


Screen display size: 640x480 pixels.

Main play is moving and jumping. The idea of executing player’s steps are as follows: 
before the proper game begins, there is an initial map displayed on the screen and this is completed by drawing a map composed of tiles into a buffer. When the player appears and makes each step, an entire new map, including the player’s new position, is redrawn. The effect of the player’s travelling is achieved by a subsequent appearances of new, constantly updated displays.


HOW A MAP IS CREATED:

A map file provides the source. It contains numbers which the programme reads and stores in a 2-dimensional array. The numbers define the types of the tiles (20x20 pixels) and they are arranged in a fashion that corresponds to the tiles arrangement displayed on the screen. 

1 – empty tile

2 – ground tile (the player can not pass through it)

3 – block tile (the player can be at the same position, but if he jumps, he lands one tile upper, that is, on the surface of the
block)

4 – chest

5 – teleportation gate tile (by pressing enterkey, the players teleports to the another gate positioned somewhere else on the same map)

6 – door (a tile which can be passed only under a certain condition (opening by a key))

7 – key


The player is displayed with stripes. Appropriate player sprites are drawn depending on several conditions, like: if player is falling, or moving.


Steps are achieved by pressing arrow keys. Jumping is achieved by pressing space key and is possible only while standing on a stable block. If the player initially stands on a block, moves forward and there is no block tile beneath his feet, he falls. This is obtained by a method “fall()” from the class “character”. Jumping down from a block is achieved by pressing down the down arrow key. 

Bool afterjump() method – measures the character field: “time” elapsed counting from a jump, in order to display a flame sprite which should appear as a trace of the jumping player. 
