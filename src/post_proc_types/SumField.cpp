
# include "SumField.hpp"
# include <sstream>
using namespace std;



// -------------------------------------------------------------------------
// Constructor:
// -------------------------------------------------------------------------

SumField::SumField() : c(),conc()
{
    GetPot InParams("inputPeso.dat");
    tagName = InParams("SumField/tagName","c1");
    conc.setTagName(tagName);
    conc.getSimInfo();
}



// -------------------------------------------------------------------------
// Destructor:
// -------------------------------------------------------------------------

SumField::~SumField()
{

}



// -------------------------------------------------------------------------
// Set-up the post-processor:
// -------------------------------------------------------------------------

void SumField::setupPostProc()
{
    c.defineVectorSize(conc.nx,conc.ny,conc.nz);
}



// -------------------------------------------------------------------------
// Execute the post-processor:
// -------------------------------------------------------------------------

void SumField::executePostProc()
{
    // open file for writing output
    ofstream outfile;
    std::stringstream filenamecombine;
    filenamecombine << "postoutput/" << tagName << "_" << "SumField.dat";
    string filename = filenamecombine.str();
    outfile.open(filename.c_str(), ios::out);
    if(!outfile.is_open())
    {
        cout << "could not open " << filename << " for writing";
        cout << " sum post-processor output!\n";
        throw 1;
    }

    // write output file header
    outfile << "step," << tagName << "sum\n";

    // run post processor on all vtk files
    int tagNum = 0;
    for (size_t f=0; f<conc.vtkFiles.size(); f++) 
    {
        // read in current file data:
        if (f == 0) tagNum = 1;
        if (f  > 0) tagNum = conc.outputInterval*f;
        c.readVTKFile(conc.vtkFiles.at(f));
        // calculate summation:
        double cSum = c.sumVals();
        // write output:
        outfile << tagNum << "," << cSum << endl;
    }

    // close output file
    outfile.close();
}
