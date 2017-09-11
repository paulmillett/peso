
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
    c.setTagName(tagName);
    c.getSimInfo();
}



// -------------------------------------------------------------------------
// Execute the post-processor:
// -------------------------------------------------------------------------

void HoshenKopelman::executePostProc()
{
}
