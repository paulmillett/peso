
# ifndef HOSHENKOPELMAN_H
# define HOSHENKOPELMAN_H

# include "../base/PesoBase.hpp"
# include "../utils/vtkField.hpp"
# include <sstream>
# include <fstream>
# include <string>


class HoshenKopelman : public PesoBase {

    private:

        vtkField c;
        std::string tagName;
        std::string hkTagName;
        std::string range;
        std::ofstream outfile;
        double cutoff;
        double phaseVol;
        double avgClusterVol;

        // methods
        int briansCodeStarts();
        void make_union(int x, int y, std::vector<int> &labels);
        void writeHK_VTK(int step);
        int find(int x, std::vector<int> &labels); 
        int new_cluster(int &max_label, std::vector<int> &labels);

    public:

        HoshenKopelman();
        ~HoshenKopelman();
        void setupPostProc();
        void executePostProc();

};

# endif  // HOSHENKOPELMAN_H
