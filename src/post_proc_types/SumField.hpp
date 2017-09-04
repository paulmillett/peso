
# ifndef SUMFIELD_H
# define SUMFIELD_H

# include "../base/PesoBase.hpp"
# include "../utils/vtkField.hpp"


class SumField : public PesoBase {

    private:

        vtkField c;
        std::string tagName;
        int tagInterval;
        int numFiles;
        int nx,ny,nz;

    public:

        SumField();
        ~SumField();
        void setupPostProc();
        void executePostProc();
        void outputData(int,double);

};

# endif  // SUMFIELD_H
