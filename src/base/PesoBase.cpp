
# include "PesoBase.hpp"

// -------------------------------------------------------------------------
// List of header files that need to be included...
// -------------------------------------------------------------------------

//# include ""

// -------------------------------------------------------------------------
// Factory method: this function returns an object determined
// by the string 'specifier':
// {Note: all of the returnable objects inherent from 'PesoBase'}
// -------------------------------------------------------------------------

PesoBase* PesoBase::PesoObjectFactory(string specifier)
{

    // ------------------------------------------------
    // 'GetPot' object containing input parameters:
    // ------------------------------------------------

    GetPot InParams("input.dat");

    // -----------------------------------
    // return the requested object:
    // -----------------------------------

    //if (specifier == "LBApp/") return new LBApp(InParams);

}
