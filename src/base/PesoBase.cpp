
# include "PesoBase.hpp"

// -------------------------------------------------------------------------
// List of header files that need to be included...
// -------------------------------------------------------------------------

# include "../post_proc_types/SumField.hpp"
# include "../post_proc_types/AveField.hpp"
# include "../post_proc_types/HoshenKopelman.hpp"

// -------------------------------------------------------------------------
// Factory method: this function returns an object determined
// by the string 'specifier':
// {Note: all of the returnable objects inherent from 'PesoBase'}
// -------------------------------------------------------------------------

PesoBase* PesoBase::PesoObjectFactory(string specifier)
{

    // -----------------------------------
    // return the requested object:
    // -----------------------------------

    if (specifier == "SumField/") return new SumField();
    if (specifier == "AveField/") return new AveField();
    if (specifier == "HoshenKopelman/") return new HoshenKopelman();

    // if unexpected specifier, return a null pointer
    return nullptr;

}
