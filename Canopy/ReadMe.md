﻿
# Canopy

This program takes pcd files and prints the upper most points into a csv file.
It can process many files at the same time. It gets the pcd file names/ location from a file named "plots.txt". For each pcd file it opens it creates it's canopy and writes a csv file.

    ----variables----
    (int) Canopy Ratio

![Alt text](/Photos/Canopy.PNG)

## Feeding variables from a file

If you want to run the program without entering the variables each time:

* Create an empty file. The name doesn't mater.
* Enter the values you want to use for the variables in the order that they appear int the ReadMe file.
* Each variable should be separated with a new line.
* Run the program with the fallowing terminal command.
  * ./<\Program name> < \<Variable File name>

## Example

    Terminal:
    ./TestProgram < param.txt

### param.txt

    2.0
    5
    test test
    5.3