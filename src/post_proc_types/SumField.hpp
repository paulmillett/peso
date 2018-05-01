
# ifndef SUMFIELD_H
# define SUMFIELD_H

# include "../base/PesoBase.hpp"
# include "../utils/vtkField.hpp"
# include <sstream>
# include <fstream>
# include <string>


class SumField : public PesoBase {

    private:

        vtkField c;
        std::string tagName;
        std::ofstream outfile;

    public:

        SumField();
        ~SumField();
        void setupPostProc();
        void executePostProc();

};

# endif  // SUMFIELD_H
