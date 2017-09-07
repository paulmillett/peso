
# ifndef AVEFIELD_H
# define AVEFIELD_H

# include "../base/PesoBase.hpp"
# include "../utils/vtkField.hpp"
# include "../utils/simInfo.hpp"


class AveField : public PesoBase {

    private:

        vtkField c;
        std::string tagName;
        simInfo conc;

    public:

        AveField();
        ~AveField();
        void setupPostProc();
        void executePostProc();

};

# endif  // AVEFIELD_H
