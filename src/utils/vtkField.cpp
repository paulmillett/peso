
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
    double sum;
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
    double sum;
    for (int i=0; i<nxyz; i++) {
        sum += a[i];
    }
    return sum/double(nxyz);
}



// -------------------------------------------------------------------------
// Read data from 'vtk' file:
// -------------------------------------------------------------------------

void vtkField::readVTKFile(std::string tagname, int tagnum)
{

    // -----------------------------------
    // Define the file location and name:
    // -----------------------------------

 	ifstream infile;
 	std::stringstream filenamecombine;
 	filenamecombine << "vtkoutput/" << tagname << "_" << tagnum << ".vtk";
 	string filename = filenamecombine.str();
 	infile.open(filename.c_str(), ios::in);

    // -----------------------------------
    // Read VTK file header:
    // -----------------------------------

    int nnxyz;
    int nnx,nny,nnz;
    int orx,ory,orz;
    int spx,spy,spz;

    std::string line;
    getline(infile,line);  // line 1
    getline(infile,line);  // line 2
    getline(infile,line);  // line 3
    getline(infile,line);  // line 4
    getline(infile,line);  // line 5
    infile >> line >> nnx >> nny >> nnz;  // DIMENSIONS
    infile >> line >> orx >> ory >> orz;  // ORIGIN
    infile >> line >> spx >> spy >> spz;  // SPACING
    getline(infile,line);
    infile >> line >> nnxyz;
    getline(infile,line);
    getline(infile,line);
    getline(infile,line);

    // -----------------------------------
    // Read data:
    // -----------------------------------

    for (int i=0; i<nnxyz; i++) {
        infile >> a[i];
    }

    // -----------------------------------
    // Close file:
    // -----------------------------------

    infile.close();

}
