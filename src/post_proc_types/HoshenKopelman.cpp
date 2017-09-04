
# include "HoshenKopelman.hpp"
using namespace std;



// -------------------------------------------------------------------------
// Constructor:
// -------------------------------------------------------------------------

HoshenKopelman::HoshenKopelman() : c()
{
    GetPot InParams("inputPeso.dat");
    tagName = InParams("HoshenKopelman/tagName","c1");
}



// -------------------------------------------------------------------------
// Destructor:
// -------------------------------------------------------------------------

HoshenKopelman::~HoshenKopelman()
{

}



// -------------------------------------------------------------------------
// Set-up the post-processor:
// -------------------------------------------------------------------------

void HoshenKopelman::setupPostProc()
{
    // see if data is read from file correctly:
    int ntag = 0;
    c.readVTKFile(tagName,ntag);
    //cout << c.a[0] << endl;
    //cout << c.a[32767] << endl;
}



// -------------------------------------------------------------------------
// Execute the post-processor:
// -------------------------------------------------------------------------

void HoshenKopelman::executePostProc()
{

}
