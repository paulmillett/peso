/**************************************************************************************/
/* 	                                                                                */
/*		PESO Code                                                                       */
/*		Authors: Prof. Paul Millett & members of the Millett Research Group             */
/* 	                                                                                */
/* 	Affiliation: University of Arkansas, Dept. of Mechanical Engineering            */
/*		Contact: pmillett@uark.edu, 479-575-2473                                        */
/* 	                                                                                */
/**************************************************************************************/

# include "PesoExecute.hpp"


int main(int argc, char **argv) {

    //	----------------------------------------------------------
    //	Create object that executes PESO simulation:
    //	----------------------------------------------------------

    PesoExecute currentJob;

    //	----------------------------------------------------------
    //	Create post-processor objects:
    //	----------------------------------------------------------

    currentJob.createPesoObjects();

    //	----------------------------------------------------------
    //	Execute post-processors:
    //	----------------------------------------------------------

    currentJob.executePesoPostProc();

}
