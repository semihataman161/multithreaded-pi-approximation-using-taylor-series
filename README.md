# Description of the Code:
This multithreaded code in C computes the pi number using each compute node that computes the part of the Pi number.
Taylor Series Formula -> Pi= 4 × (+1/1 - 1/3 + 1/5 - 1/7 + 1/9 -...)

# Configurations to run the project:
1. Download the MSMPI(Microsoft Message Passing Interface) from
   https://learn.microsoft.com/en-us/message-passing-interface/microsoft-mpi
3. Configure C++ compiler to use MSMPI.

# How to run the code on the terminal:
mpiexec -n #numberOfProcessors #exeFileName [arg]

Example:
mpiexec -n 10 PiApproximationUsingTaylorSeries.exe
