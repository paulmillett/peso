#! /bin/sh

# make directory to store test data in
mkdir vtkoutput

# compile the code that generates the test data
g++ -std=c++11 pattach.cpp

# run the data generation code
cd vtkoutput
../a.out

# delete the executable
cd ..
rm a.out
