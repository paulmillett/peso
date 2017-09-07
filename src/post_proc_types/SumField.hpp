
# ifndef SUMFIELD_H
# define SUMFIELD_H

# include "../base/PesoBase.hpp"
# include "../utils/vtkField.hpp"
# include "../utils/simInfo.hpp"


class SumField : public PesoBase {

    private:

        vtkField c;
        simInfo conc;
        std::string tagName;

    public:

        SumField();
        ~SumField();
        void setupPostProc();
        void executePostProc();

};

# endif  // SUMFIELD_H
