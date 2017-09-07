
# include "vtkField.hpp"
# include <iostream>
# include <math.h>
# include <iomanip>
# include <fstream>
# include <string>
# include <sstream>
# include <stdlib.h>
using namespace std;



// -------------------------------------------------------------------------
// Constructor:
// -------------------------------------------------------------------------

vtkField::vtkField()
{

}



// -------------------------------------------------------------------------
// Destructor:
// -------------------------------------------------------------------------

vtkField::~vtkField()
{

}



// -------------------------------------------------------------------------
// Sum the values of the field:
// -------------------------------------------------------------------------

void vtkField::defineVectorSize(int x, int y, int z)
{
    nx = x;
    ny = y;
    nz = z;
    nxyz = nx*ny*nz;
    for (int i=0; i<nxyz; i++) a.push_back(0.0);
}



// -------------------------------------------------------------------------
// Sum the values of the field:
// -------------------------------------------------------------------------

double vtkField::sumVals()
{
    double sum = 0.0;
    for (int i=0; i<nxyz; i++) {
        sum += a[i];
    }
    return sum;
}



// -------------------------------------------------------------------------
// Average the values of the field:
// -------------------------------------------------------------------------

double vtkField::aveVals()
{
    double sum = 0.0;
    for (int i=0; i<nxyz; i++) {
        sum += a[i];
    }
    return sum/double(nxyz);
}



// -------------------------------------------------------------------------
// Read data from 'vtk' file:
// -------------------------------------------------------------------------

void vtkField::readVTKFile(std::string filePath)
{

    // -----------------------------------
    // Open vtk file:
    // -----------------------------------

    ifstream infile;
    infile.open(filePath.c_str(), ios::in);
    if(!infile.is_open())
    {
        cout << "could not open " << filePath << " for reading!\n";
        throw 1;
    }

    // -----------------------------------
    // Read VTK file header:
    // -----------------------------------

    std::string line;
    getline(infile,line);  // line 1
    getline(infile,line);  // line 2
    getline(infile,line);  // line 3
    getline(infile,line);  // line 4
    getline(infile,line);  // line 5
    getline(infile,line);  // line 6
    getline(infile,line);  // line 7
    getline(infile,line);  // line 8
    getline(infile,line);  // line 9
    getline(infile,line);  // line 10
    getline(infile,line);  // line 11
    getline(infile,line);  // line 12

    // -----------------------------------
    // Read data:
    // -----------------------------------

    for (int i=0; i<nxyz; i++) {
        infile >> a[i];
    }

    // -----------------------------------
    // Close file:
    // -----------------------------------

    infile.close();

}
