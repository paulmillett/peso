
# include "vtkField.hpp"
# include <iostream>
# include <math.h>
# include <iomanip>
# include <fstream>
# include <sstream>
# include <cstdlib> // for system
# include <stdlib.h> // for strtol
using namespace std;



// -------------------------------------------------------------------------
// Constructor:
// -------------------------------------------------------------------------

vtkField::vtkField()
{
    tagnameSet = false;
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
    double sum = sumVals();
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

    for (int i=0; i<nxyz; i++) 
    {
        infile >> a[i];
    }

    // -----------------------------------
    // Close file:
    // -----------------------------------

    infile.close();

}



// -------------------------------------------------------------------------
// set the tagname:
// -------------------------------------------------------------------------

void vtkField::setTagName(std::string tagname_in)
{
    tagname = tagname_in;
    tagnameSet = true;
}



// -------------------------------------------------------------------------
// gather info about simulation
// -------------------------------------------------------------------------

void vtkField::getSimInfo()
{
    if(tagnameSet)
    {
        // get list of files, num steps, and step interval
        getFileInfo();
        // get dimensions, spacing, and total number of nodes
        getVtkInfo(numSimSteps);
        // define vector size
        for (int i=0; i<nxyz; i++) a.push_back(0.0);
    }
    else
    {
        cout << "must set tagname before calling the getSimInfo method!\n";
        throw 1;
    }
}



// -------------------------------------------------------------------------
// get simulation info from vtkouput directory
// -------------------------------------------------------------------------

void vtkField::getFileInfo()
{
    // save a list of all the vtk files to a file
    std::string cmd = "ls -1v vtkoutput/";
    std::string star = "*";
    std::string pipe2File = " > files.txt";
    std::string fullcmd = cmd+tagname+star+pipe2File;
    std::system(fullcmd.c_str());

    // read the list of vtk files into vector
    ifstream vtk;
    vtk.open("files.txt");
    std::string f;
    if(vtk.is_open())
    {   
        while(vtk >> f)
        {   
            vtkFiles.push_back(f);
        }   
    }   
    else
    {
        cout << "failed to open file.txt\n";
        throw 1; // use library exeptions later
    }
    vtk.close();

    // delete temporary file
    fullcmd = "rm files.txt";
    std::system(fullcmd.c_str());

    // get numSimSteps and the outputInterval
    size_t numFiles = vtkFiles.size();
    std::string& lf = vtkFiles.at(numFiles-1);
    size_t underScore = lf.find("_");
    size_t dot = lf.find(".");
    std::string steps = lf.substr(underScore+1,dot-1);
    numSimSteps = strtol(steps.c_str(),NULL,10);
    if(numFiles > 1)
        outputInterval = numSimSteps/(numFiles-1);
    else
        outputInterval = numSimSteps;
}



// -------------------------------------------------------------------------
// get simulation info from a vtk simulation file
// -------------------------------------------------------------------------

void vtkField::getVtkInfo(int tagnum)
{

    // -----------------------------------
    // Define the file location and name:
    // -----------------------------------

    ifstream infile;
    std::stringstream filenamecombine;
    filenamecombine << "vtkoutput/" << tagname << "_" << tagnum << ".vtk";
    std::string filename = filenamecombine.str();
    infile.open(filename.c_str(), ios::in);

    // -----------------------------------
    // Read VTK file header:
    // -----------------------------------

    if(infile.is_open())
    {
        int orx,ory,orz;
        std::string line;
        getline(infile,line);  // line 1
        getline(infile,line);  // line 2
        getline(infile,line);  // line 3
        getline(infile,line);  // line 4
        getline(infile,line);  // line 5
        infile >> line >> nx >> ny >> nz;  // DIMENSIONS
        infile >> line >> orx >> ory >> orz;  // ORIGIN
        infile >> line >> dx >> dy >> dz;  // SPACING
        getline(infile,line);
        infile >> line >> nxyz; // total number of nodes
    }
    else
    {
        cout << "could not open vtkfile to get sim info!\n";
        throw 1;
    }

    // -----------------------------------
    // Close file:
    // -----------------------------------

    infile.close();
}
