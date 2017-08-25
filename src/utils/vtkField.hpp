
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
    void readVTKFile(std::string,int);

};

# endif  // VTKFIELD_H
