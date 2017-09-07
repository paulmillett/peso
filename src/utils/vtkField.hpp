
# ifndef VTKFIELD_H
# define VTKFIELD_H

# include <string>
# include <vector>


struct vtkField {

    // data
    int nx,ny,nz;
    int nxyz;
    double dx,dy,dz;
    std::vector<double> a;

    // methods
    vtkField();
    ~vtkField();
    void defineVectorSize(int,int,int);
    void readVTKFile(std::string filePath);
    double sumVals();
    double aveVals();

};

# endif  // VTKFIELD_H
