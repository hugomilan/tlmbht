Here is how you can run the validation for the different equations:

1) Firstly, run the solver in the terminal with the following case files to get the results.
    1.1) Diffusion equation validation: ./tlmbht cdiff1Li.tlm
    1.2) Heat equation validation: ./tlmbht cheat1Li.tlm
    1.3) Pennes equation validation: ./tlmbht cpennes1Li.tlm

2) Open and run the Octave/Matlab script. The script will load the results, plots numerical and analytical results, and show differences and percentage errors. Open and run the following scripts accordingly to what you want to validate (if you script does not run, try 'clear all' to delete everything from your environment):
    2.1) Diffusion equation validation: vdiff1Li.m
    2.2) Heat equation validation: vheat1Li.m
    2.3) Pennes equation validation: vpennes1Li.m

Obs.: If you want to test different mesh sizes, you can create meshes using the file line_2BC.geo in gmsh.

Obs.2: You can also test different parameters in the case.tlm files. However, the changes made in the case are not linked to the Octave/Matlab script. If you do make any changes, remember to do the same in the Octave/Matlab script.
