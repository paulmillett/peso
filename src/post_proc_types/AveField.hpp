
# ifndef AVEFIELD_H
# define AVEFIELD_H

# include "../base/PesoBase.hpp"
# include "../utils/vtkField.hpp"


class AveField : public PesoBase {

    private:

        vtkField c;
        std::string tagName;

    public:

        AveField();
        ~AveField();
        void setupPostProc();
        void executePostProc();

};

# endif  // AVEFIELD_H
