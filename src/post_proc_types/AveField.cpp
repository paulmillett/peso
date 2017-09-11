
# include "AveField.hpp"
# include <sstream>
using namespace std;



// -------------------------------------------------------------------------
// Constructor:
// -------------------------------------------------------------------------

AveField::AveField() : c()
{
    GetPot InParams("inputPeso.dat");
    tagName = InParams("AveField/tagName","c1");
}



// -------------------------------------------------------------------------
// Destructor:
// -------------------------------------------------------------------------

AveField::~AveField()
{
}



// -------------------------------------------------------------------------
// Set-up the post-processor:
// -------------------------------------------------------------------------

void AveField::setupPostProc()
{
    c.setTagName(tagName);
    c.getSimInfo();
}



// -------------------------------------------------------------------------
// Execute the post-processor:
// -------------------------------------------------------------------------

void AveField::executePostProc()
{
    // open file for writing output
    ofstream outfile;
    std::stringstream filenamecombine;
    filenamecombine << "postoutput/" << tagName << "_" << "AveField.dat";
    string filename = filenamecombine.str();
    outfile.open(filename.c_str(), ios::out);
    if(!outfile.is_open())
    {
        cout << "could not open " << filename << " for writing";
        cout << " sum post-processor output!\n";
        throw 1;
    }

    // write output file header
    outfile << "step," << tagName << "ave\n";

    // run post processor on all vtk files
    int tagNum = 0;
    for (size_t f=0; f<c.vtkFiles.size(); f++) 
    {
        // read in current file data:
        c.readVTKFile(c.vtkFiles.at(f));
        // calculate summation:
        double cAve = c.aveVals();
        // write output:
        tagNum = c.outputInterval*f;
        outfile << tagNum << "," << cAve << endl;
    }

    // close output file
    outfile.close();
}
