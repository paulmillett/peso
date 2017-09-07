
# include "simInfo.hpp"
# include <fstream>
# include <iostream>
# include <string>
# include <sstream>
# include <vector>
# include <cstdlib> // for system
# include <stdlib.h> // for strtol
using namespace std;



// -------------------------------------------------------------------------
// Constructor:
// -------------------------------------------------------------------------

simInfo::simInfo()
{
    tagnameSet = false;
}



// -------------------------------------------------------------------------
// Destructor:
// -------------------------------------------------------------------------

simInfo::~simInfo()
{
}



// -------------------------------------------------------------------------
// set the tagname:
// -------------------------------------------------------------------------

void simInfo::setTagName(std::string tagname_in)
{
    tagname = tagname_in;
    tagnameSet = true;
}



// -------------------------------------------------------------------------
// gather info about simulation
// -------------------------------------------------------------------------

void simInfo::getSimInfo()
{
    if(tagnameSet)
    {
        // get list of files, num steps, and step interval
        getFileInfo();
        // get dimensions, spacing, and total number of nodes
        getVtkInfo(numSimSteps);
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

void simInfo::getFileInfo()
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

void simInfo::getVtkInfo(int tagnum)
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

    // establish sim box dimensions
    Lx = nx*dx;
    Ly = ny*dy;
    Lz = nz*dz;

}
