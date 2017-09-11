
# ifndef VTKFIELD_H
# define VTKFIELD_H

# include <string>
# include <vector>


class vtkField {

    public:
        // simulation field info
        int nx,ny,nz;
        int nxyz;
        int numSimSteps;
        int outputInterval;
        double dx,dy,dz;

        // field data snapshot
        std::vector<double> a;

        // list of field vtkfiles for simulation
        std::vector<std::string> vtkFiles;

        // methods
        vtkField();
        ~vtkField();
        void readVTKFile(std::string filePath);
        void setTagName(std::string tagname_in);
        void getSimInfo();
        double sumVals();
        double aveVals();

    private:
        // field data tag name
        std::string tagname;

        bool tagnameSet;
        void getFileInfo();
        void getVtkInfo(int tagnum);
};

# endif  // VTKFIELD_H
