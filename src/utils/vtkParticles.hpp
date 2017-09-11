
# ifndef VTKPARTICLES_H
# define VTKPARTICLES_H

# include <string>
# include <vector>


class vtkParticles {

    public:
        // data
        int N;
        int numSimSteps;
        int outputInterval;

        // particle data snapshot
        std::vector<double> r;
        std::vector<double> rad;

        // list of field vtkfiles for simulation
        std::vector<std::string> vtkFiles;

        // methods
        vtkParticles();
        ~vtkParticles();
        void readVTKFile(std::string filePath);
        void setTagName(std::string tagname_in);
        void getSimInfo();

    private:
        // field data tag name
        std::string tagname;

        bool tagnameSet;
        void getFileInfo();
        void getVtkInfo(int tagnum);
};

# endif  // VTKPARTICLES_H
