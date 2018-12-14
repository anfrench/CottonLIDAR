
------Boll Count-----
	This program is intended to count the number of bolls in a cotton filed using pcd files.
	It is capable of processing many files at a time. 
	It opens a main file that contains all of the file Names/locations that you want processed
	and works though them one at a time.
	the file names should be delimited by a new line "\n".

	The boll counting algorithm is based on euclidean clustering.
	All points within a defined radius of a point are considered the same cluster.
	points beyond the set radius are added to a different cluster.
	Also clusters are only considered if they are within a defined number of points.
	Both the radius and the number of points are inputs to this program.
	The program will ask for all variables at the beginning and will process all files
	the same way.

	----Variables----
	(float) Maximum Z 
	(float) Minimum Z
	(float) Maximum cluster points	
	(float) Minimum cluster points
	(float) Tolerance
	

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
		
		
		
		
		
		
		
		
		
		
		
		
		
