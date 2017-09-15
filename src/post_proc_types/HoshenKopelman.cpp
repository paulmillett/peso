
# include "HoshenKopelman.hpp"
using namespace std;



// -------------------------------------------------------------------------
// Constructor:
// -------------------------------------------------------------------------

HoshenKopelman::HoshenKopelman() : c()
{
    GetPot InParams("inputPeso.dat");
    tagName = InParams("HoshenKopelman/tagName","c1");

    // open file for writing output
    outfile;
    std::stringstream filenamecombine;
    filenamecombine << "postoutput/" << tagName << "_" << "HoshenKopelman.dat";
    string filename = filenamecombine.str();
    outfile.open(filename.c_str(), ios::out | ios::trunc);
    if(!outfile.is_open())
    {
        cout << "could not open " << filename << " for writing!";
        throw 1;
    }

    // write output file header
    /* outfile << "step,numOnInterface,fracOnInterface\n"; */
}



// -------------------------------------------------------------------------
// Destructor:
// -------------------------------------------------------------------------

HoshenKopelman::~HoshenKopelman()
{
    // close output file
    outfile.close();
}



// -------------------------------------------------------------------------
// Set-up the post-processor:
// -------------------------------------------------------------------------

void HoshenKopelman::setupPostProc()
{
    c.setTagName(tagName);
    c.getSimInfo();
}



// -------------------------------------------------------------------------
// Execute the post-processor:
// -------------------------------------------------------------------------

void HoshenKopelman::executePostProc()
{
}
