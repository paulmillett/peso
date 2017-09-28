
# include <iostream>
# include <stdexcept>
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

}



// -------------------------------------------------------------------------
// Destructor:
// -------------------------------------------------------------------------

PesoExecute::~PesoExecute()
{
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

    for (size_t i=0; i<sections.size(); i++) {

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
            if(pesoapps[i] == nullptr)
            {
                cout << "\nOne or more [Sections] in the input file is not ";
                cout << "a valid peso app.\n";
                throw runtime_error("A runtime error occurred in PesoExecute::CreatePesoObjects");
            }
        }

    }

    // ------------------------------------------------
    // loop over executable objects, setting up each:
    // ------------------------------------------------

    for (size_t i=0; i<pesoapps.size(); i++) {
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

    for (size_t i=0; i<pesoapps.size(); i++) {
        pesoapps[i]->executePostProc();
    }

}
