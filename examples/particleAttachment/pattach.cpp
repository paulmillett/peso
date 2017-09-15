# include<fstream>
# include<iostream>
# include<sstream>
# include<iomanip>
# include<vector>
# include<cmath>
# include<string>
using namespace std;

void setCp(const int nx, const int ny, const int nz, const double dr, const double rad, vector<double>& r, vector<double>& cp);
void writeVtk(std::string tagName, std::vector<double>& field,int nx,int ny,int nz);
void writeParticleVtk(std::vector<double>& r, std::string tagname, double rad);

int main()
{
    const double rad = 5.0;
    const double dr = 1.0;
    vector<double> r;
    const int nx = 8.0*rad/dr;
    const int ny = 4.0*rad/dr;
    const int nz = 4.0*rad/dr;
    const int nxyz = nx*ny*nz;
    vector<double> c1;
    vector<double> c2;
    vector<double> cp;

    // initialize the vectors
    for (int k=0;k<9;k++)
        r.push_back(0.0);
    r[0] = 7.0;
    r[1] = 10.0;
    r[2] = 10.0;
    r[3] = 20.0;
    r[4] = 10.0;
    r[5] = 10.0;
    r[6] = 37.0;
    r[7] = 10.0;
    r[8] = 10.0;

    // particle conc field
    for (int k=0;k<nz;k++)
        for (int j=0;j<ny;j++)
            for (int i=0;i<nx;i++)
            {
                c1.push_back(0.0);
                c2.push_back(0.0);
                cp.push_back(0.0);
            }
    setCp(nx,ny,nz,dr,rad,r,cp);

    for (int i=0;i<nx;i++)
        for (int j=0;j<ny;j++)
            for (int k=0;k<nz;k++)
            {
                int ind = k*nx*ny + j*nx + i;
                double x = (double)i*dr;
                c1.at(ind) = (tanh((x-5.0)/0.75) - tanh((x-30.0)/0.75))/2.0;
                c2.at(ind) = 1.0 - c1.at(ind);
                c1.at(ind) *= 1.0 - cp.at(ind);
                c2.at(ind) *= 1.0 - cp.at(ind);
            }

    // write fields to vtk files
    std::string c1Tag = "c1";
    std::string c2Tag = "c2";
    std::string cpTag = "cp";
    std::string p = "particles";

    writeVtk(c1Tag,c1,nx,ny,nz);
    writeVtk(c2Tag,c2,nx,ny,nz);
    writeVtk(cpTag,cp,nx,ny,nz);
    writeParticleVtk(r,p,rad);
}



void setCp(const int nx, const int ny, const int nz, const double dr, const double rad, vector<double>& r, vector<double>& cp)
{
    for (int pp=0; pp<3; pp++) 
    {
        // width of region surrounding particles to probe:
        int wdth = int(ceil(2*rad+1));
        // particle's position:
        double x = r[pp*3+0];
        double y = r[pp*3+1];
        double z = r[pp*3+2];
        // grid point nearest particle (rounded down):
        int x0 = int(floor(x)/dr);
        int y0 = int(floor(y)/dr);
        int z0 = int(floor(z)/dr);
        // loop over region near particle:
        for (int k=0; k<wdth; k++) 
        {
            int kk = z0 - (wdth/2 - 1) + k;
            if (kk < 0) kk += nz;
            if (kk > nz-1) kk -= nz;
            for (int j=0; j<wdth; j++) 
            {
                int jj = y0 - (wdth/2 - 1) + j;
                if (jj < 0) jj += ny;
                if (jj > ny-1) jj -= ny;
                for (int i=0; i<wdth; i++) 
                {
                    int ii = x0 - (wdth/2 - 1) + i;
                    if (ii < 0) ii += nx;
                    if (ii > nx-1) ii -= nx;
                    if (nx == 1) ii = 0;
                    // calculate distance to point:
                    double rx = x - double(ii);
                    double ry = y - double(jj);
                    double rz = z - double(kk);
                    rx -= round(rx/(nx*dr))*nx*dr;
                    ry -= round(ry/(ny*dr))*ny*dr;
                    rz -= round(rz/(nz*dr))*nz*dr;
                    double r2 = rx*rx + ry*ry + rz*rz;
                    // assign spread function to grid:
                    double val = 1.0 - r2/(rad*rad);
                    if (val < 0.0) val = 0.0;
                    double val0 = cp.at(kk*nx*ny + jj*nx + ii);
                    cp.at(kk*nx*ny + jj*nx + ii) = max(val,val0);
                }
            }
        }
    }
}


void writeVtk(std::string tagName, std::vector<double>& field,int nx, int ny, int nz)
{
    ofstream outfile;
    std::string fileName = tagName+"_1.vtk";
    outfile.open(fileName, ios::out | ios::trunc);
    string d = "   ";
    outfile << "# vtk DataFile Version 3.1" << endl;
    outfile << "VTK file containing grid data" << endl;
    outfile << "ASCII" << endl;
    outfile << " " << endl;
    outfile << "DATASET STRUCTURED_POINTS" << endl;
    outfile << "DIMENSIONS" << d << nx << d << ny << d << nz << endl;
    outfile << "ORIGIN " << d << 0 << d << 0 << d << 0 << endl;
    outfile << "SPACING" << d << 1.0 << d << 1.0 << d << 1.0 << endl;
    outfile << " " << endl;
    outfile << "POINT_DATA " << (nx)*(ny)*(nz) << endl;
    outfile << "SCALARS " << tagName << " float" << endl;
    outfile << "LOOKUP_TABLE default" << endl;

    // write the data
    for (int k=0;k<nz;k++)
        for (int j=0;j<ny;j++)
            for (int i=0;i<nx;i++)
                outfile << fixed << setprecision(3) << field[k*nx*ny + j*nx + i] << endl;
}

void writeParticleVtk(std::vector<double>& r, std::string tagname, double rad)
{

    // -----------------------------------
    //  Define the file location and name:
    // -----------------------------------

    ofstream outfile;
    std::stringstream filenamecombine;
    filenamecombine << tagname << "_1.vtk";
    string filename = filenamecombine.str();
    outfile.open(filename.c_str(), ios::out | ios::trunc);

    // -----------------------------------
    //  Write the 'vtk' file header:
    // -----------------------------------

    string d = "   ";
    outfile << "# vtk DataFile Version 3.1" << endl;
    outfile << "VTK file containing particle data" << endl;
    outfile << "ASCII" << endl;
    outfile << " " << endl;
    outfile << "DATASET POLYDATA" << endl;
    outfile << " " << endl;
    outfile << "POINTS" << d << 3 << d << " float" << endl;

    // -----------------------------------
    //  Write the position data:
    // -----------------------------------

    for (int i=0; i<3; i++) {
        outfile << fixed << setprecision(3) << r[i*3+0] << d << r[i*3+1] << d << r[i*3+2] << endl;
    }   

    // -----------------------------------
    //  write the radius data
    // -----------------------------------

    outfile << "POINT_DATA\t" << d << 3 << endl;
    outfile << "SCALARS radius float\n";
    outfile << "LOOKUP_TABLE default\n";

    for (int i=0; i<3; i++) {
        outfile << fixed << setprecision(3) << rad << endl;
    }   

    // -----------------------------------
    //  Close the file:
    // -----------------------------------

    outfile.close();
}

