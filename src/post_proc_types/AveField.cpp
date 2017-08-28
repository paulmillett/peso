
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
    numFiles = InParams("AveField/numFiles",0);
    tagInterval = InParams("AveField/tagInterval",1);
    nx = InParams("AveField/nx",1);
    ny = InParams("AveField/ny",1);
    nz = InParams("AveField/nz",1);  
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
    c.defineVectorSize(nx,ny,nz);
}



// -------------------------------------------------------------------------
// Execute the post-processor:
// -------------------------------------------------------------------------

void AveField::executePostProc()
{
    int tagNum = 0;
    for (int f=0; f<numFiles; f++) {
        // read in current file data:
        if (f == 0) tagNum = 1;
        if (f  > 0) tagNum = tagInterval*f;
        c.readVTKFile(tagName,tagNum);
        // calculate summation:
        double cAve = c.aveVals();
        // write output:
        outputData(tagNum,cAve);
    }
}



// -------------------------------------------------------------------------
// Output data to file:
// -------------------------------------------------------------------------

void AveField::outputData(int step, double val)
{
 	ofstream outfile;
 	std::stringstream filenamecombine;
 	filenamecombine << "postoutput/" << tagName << "_" << "AveField.dat";
 	string filename = filenamecombine.str();
 	outfile.open(filename.c_str(), ios::out | ios::app);
    outfile << step << " " << val << endl;
}
