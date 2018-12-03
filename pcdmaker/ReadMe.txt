
------pclMaker-----
	This program makes a pcd tube structure. It uses the make branch function within pclLib/pclMaker.
	It was used for testing ways to get a clear model of a plant.

	----Variables----
	(int) Number of points.
	(float) Diameter.
	(float) Length.
	(float) Ring density.

	----Feeding variables from a file.----
	If you want to run the program without entering the variables each time:
	Create an empty file. The name doesn't mater.
	Enter the values you want to use for the variables in the order that they appear int the ReadMe file.
	Each variable should be separated with a new line.
	run the program with the fallowing terminal command. 
	
	./<Program name> < <Variable File name> 
	
	Example:
	
	
		Terminal:
		./TestProgram < param.txt
	
	
		ReadMe.txt:
		==============
		---TestProgram---
		xxxxxxxx
		xxxxxxxxx
		xxxxx
		----Variables----
		(float)	xxxxx
		(int)	xxxx
		(string)xxxxx
		(float)	xxxx
		
		---footer------
		xxxxxxx
		xxxx
		xxxxxx
		==============
		
		
		
		param.txt:
		==============
		2.0
		5
		test test
		5.3
		==============
		
		
		
