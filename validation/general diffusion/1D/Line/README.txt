To run this validation, do the following:

1) Firstly, run the solver in the terminal to get the calculation:
./tlmbht

2) Open and run 'plate_4BC_10766e.m' in Octave/Matlab. This will load the output data. If you get errors when loading the data, try 'clear all' (this will delete all your variables).

3) Open and run 'Validation2D.m' in Octave/Matlab. This will show plots and comparision with the analytical model.

Obs.: If you want to test different mesh sizes, you can create the meshes using gmsh and the file 'plate_4BC.geo'.
