
# ifndef AVEFIELD_H
# define AVEFIELD_H

# include "../base/PesoBase.hpp"
# include "../utils/vtkField.hpp"
# include <sstream>
# include <fstream>
# include <string>


class AveField : public PesoBase {

    private:

        vtkField c;
        std::string tagName;
        std::ofstream outfile;

    public:

        AveField();
        ~AveField();
        void setupPostProc();
        void executePostProc();

};

# endif  // AVEFIELD_H
