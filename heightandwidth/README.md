# Shape

* Opens a point cloud. crops it into a given number of layers. goes down the rows and finds the max height and width.
* prints values in a csv file. opens next file.
* gets file names from a file named "plots.txt".

#
    ----variables----
    (int) plots per file
    (int) number of layers
    (float) Maximum Height
    (float) Minimum Height
    (int)   Outlier Removal-Points to consider
    (float) Outlier Removal-Standard deviation multiplier

    for int i =0 to number of plots per file
    {
        (float) Upper bounds for plot i
        (float) Lower bounds for plot i
    }

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

    5    <- n
    5
    10.0
    -2.0
    30
    2.0
    3.0 <- add n upper and n1
    1.0 <- n lower bounds. n1
    5.3 n2
    3.3 n2
    ect n
    ect n
