
# include "PesoExecute.hpp"
# include "../utils/GetPot"
# include <cstdlib>
using namespace std;

// -------------------------------------------------------------------------
// Constructor:
// -------------------------------------------------------------------------

PesoExecute::PesoExecute()
{

    // -----------------------------------
    // make output directories:
    // -----------------------------------

    std::system("mkdir -p postoutput");        // make output directory
    std::system("exec rm -rf postoutput/*");   // remove any existing files

}



// -------------------------------------------------------------------------
// Destructor:
// -------------------------------------------------------------------------

PesoExecute::~PesoExecute()
{
    // MPI::Finalize();
}



// -------------------------------------------------------------------------
// Create the Peso post-processor objects, and store them in a vector:
// -------------------------------------------------------------------------

void PesoExecute::createPesoObjects()
{

    // ------------------------------------------------
    // 'GetPot' object containing input parameters:
    // ------------------------------------------------

    GetPot InParams("inputPeso.dat");

    // ------------------------------------------------
    // make vector of input section names:
    // ------------------------------------------------

    vector<string> sections = InParams.get_section_names();

    // ------------------------------------------------
    // determine which sections are executable 'apps':
    // ------------------------------------------------

    for (int i=0; i<sections.size(); i++) {

        // ---------------------------------------------
        // get string that stores value of "section/app"
        // ---------------------------------------------

        string currentSec = sections[i] + "app";
        const char* currentSecChar = currentSec.c_str();
        string appFlag = InParams(currentSecChar,"false");

        // ---------------------------------------------
        // if "app = true", make a new object:
        // ---------------------------------------------

        if (appFlag == "true") {
            pesoapps.push_back(PesoBase::PesoObjectFactory(sections[i]));
        }

    }

    // ------------------------------------------------
    // loop over executable objects, initializing each:
    // ------------------------------------------------

    for (int i=0; i<pesoapps.size(); i++) {
        pesoapps[i]->setupPostProc();
    }

}



// -------------------------------------------------------------------------
// Execute the post-processors:
// -------------------------------------------------------------------------

void PesoExecute::executePesoPostProc()
{

    // -----------------------------------
    // execute the postprocessors:
    // -----------------------------------

    for (int i=0; i<pesoapps.size(); i++) {
        pesoapps[i]->executePostProc();
    }

}
