
----Turn Table Decoder----
	This program takes a file of lms4000 scans delimited by a new line.
	The program reads the distance values and projects them onto a rotation.
	The output is a pcd File.
	Compile with c++11 
	no point cloud libraries needed.

	----variables----
	(string) Input File Name.
	(string) Output File Name.
	(float ) Distance from the lidar to center of turntable. 
	(float ) height offset.
	(float ) Scale.
	(float ) Degrees of tilt.
	(float ) How far the lidar is off of horizon.
	(int   ) Steps per revolution.
	(int   ) Scans Per Step?
	

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
		
		
		
