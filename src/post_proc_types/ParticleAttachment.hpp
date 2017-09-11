
# ifndef PARTICLEATTACHMENT_H
# define PARTICLEATTACHMENT_H

# include "../base/PesoBase.hpp"
# include "../utils/vtkField.hpp"
# include "../utils/vtkParticles.hpp"
# include <vector>


class ParticleAttachment : public PesoBase {

    private:

        vtkField c1;
        vtkField c2;
        vtkField xi;
        vtkParticles p;
        std::string c1TagName;
        std::string c2TagName;
        std::string xiTagName;
        std::string partTagName;
        std::string outFileName;
        bool deleteTypeInfo;
        std::vector<int> particleType;
        void writeParticleTypeData(std::string filePath);
        void deleteParticleTypeData(std::string filePath);
        double c1thresh;
        double c2thresh;
        double cpthresh;

        // methods
        int calcNumOnInterface();

    public:

        ParticleAttachment();
        ~ParticleAttachment();
        void setupPostProc();
        void executePostProc();
        void test();

};

# endif  // PARTICLEATTACHMENT_H