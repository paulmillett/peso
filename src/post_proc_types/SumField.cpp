
# include "SumField.hpp"
# include <sstream>
using namespace std;



// -------------------------------------------------------------------------
// Constructor:
// -------------------------------------------------------------------------

SumField::SumField() : c()
{
    GetPot InParams("inputPeso.dat");
    tagName = InParams("SumField/tagName","c1");
    numFiles = InParams("SumField/numFiles",0);
    tagInterval = InParams("SumField/tagInterval",1);
    nx = InParams("SumField/nx",1);
    ny = InParams("SumField/ny",1);
    nz = InParams("SumField/nz",1);    
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
    c.defineVectorSize(nx,ny,nz);
}



// -------------------------------------------------------------------------
// Execute the post-processor:
// -------------------------------------------------------------------------

void SumField::executePostProc()
{
    int tagNum = 0;
    for (int f=0; f<numFiles; f++) {
        // read in current file data:
        if (f == 0) tagNum = 1;
        if (f  > 0) tagNum = tagInterval*f;
        c.readVTKFile(tagName,tagNum);
        // calculate summation:
        double cSum = c.sumVals();
        // write output:
        outputData(tagNum,cSum);
    }
}



// -------------------------------------------------------------------------
// Output data to file:
// -------------------------------------------------------------------------

void SumField::outputData(int step, double val)
{
 	ofstream outfile;
 	std::stringstream filenamecombine;
 	filenamecombine << "postoutput/" << tagName << "_" << "SumField.dat";
 	string filename = filenamecombine.str();
 	outfile.open(filename.c_str(), ios::out | ios::app);
    outfile << step << " " << val << endl;
}
