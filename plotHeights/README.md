
	------plot Heights-----
	This program calculates the average height and standard deviation of point clouds canopy.
	It can process several files at once.
	All file names that you would like to process should be contained in a file called "plots.txt".
	Each of the files that are processed are first cropped by height.
	Next the cloud is than cropped into several smaller clouds and cropped separately. 
	The pieces that each cloud is coped into is defined by another input file called "Bounds.csv" .
	Each line in bounds contains all information that is necessary to crop each section.
	The format is:
			(string:ID),(char Dimension1),(float Upper1),(floatlower1),(char Dimension2),(float Upper2),(floatlower2)   
	Example:
		1,x,12.048,11.032,y,31,19

	For each file processed it will output a file. The output file will be named file Name_Heights.csv
	where (file Name) was the original file name excluding it's extension(.pcd). 

	----Variables----
	(float)	maximum height.
	(float)	minimum height.
	(int)	Canopy ratio.
	

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
		
		
		
