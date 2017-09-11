
# include "vtkParticles.hpp"
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

vtkParticles::vtkParticles()
{
    tagnameSet = false;
}



// -------------------------------------------------------------------------
// Destructor:
// -------------------------------------------------------------------------

vtkParticles::~vtkParticles()
{
}



// -------------------------------------------------------------------------
// Read data from 'vtk' file:
// -------------------------------------------------------------------------

void vtkParticles::readVTKFile(std::string filePath)
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
    getline(infile,line);  // # vtk DataFile Version 3.1
    getline(infile,line);  // VTK file containing particle data
    getline(infile,line);  // ASCII
    getline(infile,line);  // 
    getline(infile,line);  // DATASET POLYDATA
    getline(infile,line);  // 
    getline(infile,line);  // POINTS    N float

    // -----------------------------------
    // Read data particle position data:
    // -----------------------------------

    for (int i=0; i<N; i++) 
    {
        infile >> r[i*3+0] >> r[i*3+1] >> r[i*3+2];
    }

    // -----------------------------------
    // Read particle radius data:
    // -----------------------------------

    getline(infile,line); // 
    getline(infile,line); // POINT_DATA  N
    getline(infile,line); // SCALARS radius float
    getline(infile,line); // LOOKUP_TABLE default

    for (int i=0; i<N; i++) 
    {
        infile >> rad[i];
    }

    // -----------------------------------
    // Close file:
    // -----------------------------------

    infile.close();

}



// -------------------------------------------------------------------------
// set the tagname:
// -------------------------------------------------------------------------

void vtkParticles::setTagName(std::string tagname_in)
{
    tagname = tagname_in;
    tagnameSet = true;
}



// -------------------------------------------------------------------------
// gather info about simulation
// -------------------------------------------------------------------------

void vtkParticles::getSimInfo()
{
    if(tagnameSet)
    {
        // get list of files, num steps, and step interval
        getFileInfo();
        // get dimensions, spacing, and total number of nodes
        getVtkInfo(numSimSteps);
        // define vector size
        for (int i=0; i<N; i++) 
        {
            for (int j=0; j<3; j++)
                r.push_back(0.0);
            rad.push_back(0.0);
        }
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

void vtkParticles::getFileInfo()
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
    outputInterval = numSimSteps/(numFiles-1);
}



// -------------------------------------------------------------------------
// get simulation info from a vtk simulation file
// -------------------------------------------------------------------------

void vtkParticles::getVtkInfo(int tagnum)
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
    // Read VTK file header to get number
    // of particles.
    // -----------------------------------

    if(infile.is_open())
    {
        std::string line;
        getline(infile,line);  // # vtk DataFile Version 3.1
        getline(infile,line);  // VTK file containing particle data
        getline(infile,line);  // ASCII
        getline(infile,line);  // DATASET POLYDATA
        getline(infile,line);  // 
        infile >> line >> N >> line; // number of particles
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
