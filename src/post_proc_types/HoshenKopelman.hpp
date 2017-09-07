
# ifndef HOSHENKOPELMAN_H
# define HOSHENKOPELMAN_H

# include "../base/PesoBase.hpp"
# include "../utils/vtkField.hpp"
# include "../utils/simInfo.hpp"


class HoshenKopelman : public PesoBase {

    private:

        vtkField c;
        std::string tagName;
        simInfo conc;

    public:

        HoshenKopelman();
        ~HoshenKopelman();
        void setupPostProc();
        void executePostProc();

};

# endif  // HOSHENKOPELMAN_H
