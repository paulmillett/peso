
# ifndef PESOBASE_H
# define PESOBASE_H

# include "../utils/GetPot"
# include <string>
using namespace std;

// ---------------------------------------------------------------------
// This is the base class for the Peso post-processor project.  All
// application classes in the Peso project inherent from this class.
// ---------------------------------------------------------------------

class PesoBase {

public:
    
    // -------------------------------------------------------------------
    // Define factory method that creates objects of PesoBase sub-classes:
    // -------------------------------------------------------------------

    static PesoBase* PesoObjectFactory(string specifier);

    // -------------------------------------------------------------------
    // All sub-classes must define the below pure virtual functions:
    // -------------------------------------------------------------------

    virtual void setupPostProc() = 0;
    virtual void executePostProc() = 0;

};

# endif  // PESOBASE_H
