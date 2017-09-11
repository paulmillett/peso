
# ifndef PESOEXECUTE_H
# define PESOEXECUTE_H

// ---------------------------------------------------------------------
// This is the Peso executioner class.  This class runs the post-processor.
// ---------------------------------------------------------------------

# include "PesoBase.hpp"
# include <vector>
using namespace std;

class PesoExecute {

    private:

        vector<PesoBase*> pesoapps;

    public:

        PesoExecute();
        ~PesoExecute();
        void createPesoObjects();
        void executePesoPostProc();

};

# endif  // PESOEXECUTE_H
