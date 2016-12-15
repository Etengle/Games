~~~~~~~~~~~~~~~~
Game: River Frog
~~~~~~~~~~~~~~~~

AUTHOR
======
	
	NTHU 
	Department of Mathematics
	Department of Computer Science
	Ph.D. Student
	Name: Yu Hsiang, Cheng
	Nickname: Slighten
	E-mail: slightencheng@gmail.com

INSTRUCTION
===========

		First, untar the archive "RiverFrog.tar.gz", then go into the "RiverFrog/".
		___________________________________
		|% tar -zxvf RiverFrog.tar.gz     |
		|% cd RiverFrog/ 		  |
		-----------------------------------
		The file "frog.c" is the source code of this game, and you can 
	just type following commands on your terminal (e.g. Cygwin):
		_____________________________
		|% make  		    |
		|% ./frog 		    |
		|(or ./frog.exe in Windows) |
		-----------------------------
	to play the game.

		As you execute the game, you will see the "main frame" like following:
		
		-------------------------------------------
		River Frog
                 		 made by Slighten
		-----------How to play-----------
		W, up arrow   : up
		S, down arrow : down
		A, left arrow : left
		D, right arrow: right
		Q, ctrl+c     : quit
		***Godfinger***
		O: speed down
		P: speed up

		>>>>> High Score: 11369 by Slighten <<<<<

		PRESS ANY KEY TO START
		D: EXTREME MODE
		S: CONFIGURE PARAMETERS MANUALLY
		--------------------------------------------

		As shown above, you may press arrow key or W, A, S, D to move, and Q to quit.
		You also can press O or P to change speed.
		Also, D to EXTREME mode (random wood & gap length), and S to configure the length 
		of map, wood & gap manually.

		The game will record highest score and count time passed.
		
		After playing, you may type following command:
		________________
		| % make clean |
		----------------
		to clean the score record , *.o, *.out, *.exe and the "frog". 

BUGS
====
		
		I have checked it detailedly, so it should have had bare bugs. 
	However, if you find some bugs, then I sincerely hope you can contact me by FB,
	e-mail, or just call me directly. Thanks for your help!

FAQs
====
	
	Q: How do I configure the map size, wood length, and gap length?
	A: In the main frame, click 'S' and you can configure these parameters manually.
	Q: Why don't you make a GUI version?
	A: I couldn't make it because I don't know where to begin, and handling this 
	   has taken much of my time and efforts to do it.
	   I'll try hard to understand how to GUI programming, and sincerely hope great 
	   TAs may give me a few instructions and help me through the difficulty.

CHANGELOG
=========

	NONE

TODO
====

	Study the GUI and write a GUI version.
	Maybe I will make an Android version in the Software Studio course. 

COPYRIGHT
=========
				
	Copyright (C) 2014 by Slighten. All rights reserved.
