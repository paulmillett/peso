
# include "ParticleAttachment.hpp"
# include <sstream>
# include <cstdlib>
using namespace std;



// -------------------------------------------------------------------------
// Constructor:
// -------------------------------------------------------------------------

ParticleAttachment::ParticleAttachment() : c1(),c2(),xi(),conc1(),conc2(),XI()
{
    GetPot InParams("inputPeso.dat");
    c1TagName = InParams("ParticleAttachment/c1TagName","c1");
    c2TagName = InParams("ParticleAttachment/c2TagName","c2");
    xiTagName = InParams("ParticleAttachment/xiTagName","cp");
    outFileName = InParams("ParticleAttachment/outFileName","pout.dat");
    conc1.setTagName(c1TagName);
    conc1.getSimInfo();
    conc2.setTagName(c2TagName);
    conc2.getSimInfo();
    XI.setTagName(xiTagName);
    XI.getSimInfo();

    // make sure c1,c2, and xi have the same number of files
    int c1_nf = conc1.vtkFiles.size();
    int c2_nf = conc2.vtkFiles.size();
    int xi_nf = XI.vtkFiles.size();
    if(c1_nf != c2_nf || c1_nf != xi_nf)
    {
        cout << "number of order parameters for each order parameter\n";
        cout << "do not match.\n";
        throw 1;
    }
}



// -------------------------------------------------------------------------
// Destructor:
// -------------------------------------------------------------------------

ParticleAttachment::~ParticleAttachment()
{
}



// -------------------------------------------------------------------------
// Set-up the post-processor:
// -------------------------------------------------------------------------

void ParticleAttachment::setupPostProc()
{
    c1.defineVectorSize(conc1.nx,conc1.ny,conc1.nz);
    c2.defineVectorSize(conc2.nx,conc2.ny,conc2.nz);
    xi.defineVectorSize(xi.nx,xi.ny,xi.nz);
}



// -------------------------------------------------------------------------
// Execute the post-processor:
// -------------------------------------------------------------------------

void ParticleAttachment::executePostProc()
{
}
