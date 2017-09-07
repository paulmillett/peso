# ifndef SIMINFO_H
# define SIMINFO_H

# include <string>
# include <vector>


class simInfo {

    public:
        // general sim data
        std::string tagname;
        int nx,ny,nz;
        int nxyz;
        double dx,dy,dz;
        double Lx,Ly,Lz;
        int numSimSteps;
        int outputInterval;

        // list of vtkfiles
        std::vector<std::string> vtkFiles;

        // particle specific data
        int numPart;

        // methods
        simInfo();
        ~simInfo();
        void getSimInfo();
        void setTagName(std::string tagname_in);

    private:
        bool tagnameSet;
        void getFileInfo();
        void getVtkInfo(int tagnum);

};

# endif  // SIMINFO_H
