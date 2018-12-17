
# Resampling

* An Example from pointclouds.org
* Smooths the point cloud. [Origonal here](http://pointclouds.org/documentation/tutorials/resampling.php#moving-least-squares)

* Modified to take in variables.
  
#

    ----Variables----
    (String) file Name In
    (String) file Name Out
    (float ) Search Radius

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