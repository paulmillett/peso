
# include "HoshenKopelman.hpp"
using namespace std;



// -------------------------------------------------------------------------
// Constructor:
// -------------------------------------------------------------------------

HoshenKopelman::HoshenKopelman() : c(),conc()
{
    GetPot InParams("inputPeso.dat");
    tagName = InParams("HoshenKopelman/tagName","c1");
    conc.setTagName(tagName);
    conc.getSimInfo();
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
}



// -------------------------------------------------------------------------
// Execute the post-processor:
// -------------------------------------------------------------------------

void HoshenKopelman::executePostProc()
{
}
