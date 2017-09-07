
# ifndef PARTICLEATTACHMENT_H
# define PARTICLEATTACHMENT_H

# include "../base/PesoBase.hpp"
# include "../utils/vtkField.hpp"
# include "../utils/simInfo.hpp"


class ParticleAttachment : public PesoBase {

    private:

        vtkField c1;
        vtkField c2;
        vtkField xi;
        std::string c1TagName;
        std::string c2TagName;
        std::string xiTagName;
        std::string outFileName;
        simInfo conc1;
        simInfo conc2;
        simInfo XI;

    public:

        ParticleAttachment();
        ~ParticleAttachment();
        void setupPostProc();
        void executePostProc();

};

# endif  // PARTICLEATTACHMENT_H
