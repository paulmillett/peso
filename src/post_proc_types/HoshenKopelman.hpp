
# ifndef HOSHENKOPELMAN_H
# define HOSHENKOPELMAN_H

# include "../base/PesoBase.hpp"
# include "../utils/vtkField.hpp"
# include <sstream>
# include <fstream>


class HoshenKopelman : public PesoBase {

    private:

        vtkField c;
        std::string tagName;
        std::ofstream outfile;

    public:

        HoshenKopelman();
        ~HoshenKopelman();
        void setupPostProc();
        void executePostProc();

};

# endif  // HOSHENKOPELMAN_H
