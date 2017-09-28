
# include "ParticleAttachment.hpp"
# include <cstdlib>
# include <stdlib.h> // for system
# include <cmath>
# include <iomanip>
using namespace std;



// -------------------------------------------------------------------------
// Constructor:
// -------------------------------------------------------------------------

ParticleAttachment::ParticleAttachment() : c1(),c2(),xi(),p()
{
    GetPot InParams("inputPeso.dat");
    c1TagName = InParams("ParticleAttachment/c1TagName","c1");
    c2TagName = InParams("ParticleAttachment/c2TagName","c2");
    xiTagName = InParams("ParticleAttachment/xiTagName","cp");
    partTagName = InParams("ParticleAttachment/partTagName","particles");
    outFileName = InParams("ParticleAttachment/outFileName","particleAttachment.dat");
    generateTypeInfo = InParams("ParticleAttachment/generateTypeInfo",0);
    c1thresh = InParams("ParticleAttachment/c1thresh",0.5);
    c2thresh = InParams("ParticleAttachment/c2thresh",0.5);
    cpthresh = InParams("ParticleAttachment/cpthresh",0.1);

    // open file for writing output
    std::stringstream filenamecombine;
    filenamecombine << "postoutput/" << outFileName;
    string filename = filenamecombine.str();
    outfile.open(filename.c_str(), ios::out | ios::trunc);
    if(!outfile.is_open())
    {
        cout << "could not open " << filename << " for writing!";
        throw 1;
    }

    // write output file header
    outfile << "step,numOnInterface,fracOnInterface\n";
}



// -------------------------------------------------------------------------
// Destructor:
// -------------------------------------------------------------------------

ParticleAttachment::~ParticleAttachment()
{
    // close output file
    outfile.close();
}



// -------------------------------------------------------------------------
// Set-up the post-processor:
// -------------------------------------------------------------------------

void ParticleAttachment::setupPostProc()
{
    c1.setTagName(c1TagName);
    c1.getSimInfo();
    c2.setTagName(c2TagName);
    c2.getSimInfo();
    xi.setTagName(xiTagName);
    xi.getSimInfo();
    p.setTagName(partTagName);
    p.getSimInfo();

    // make sure c1,c2, and xi have the same number of files
    int c1_nf = c1.vtkFiles.size();
    int c2_nf = c2.vtkFiles.size();
    int xi_nf = xi.vtkFiles.size();
    int p_nf = p.vtkFiles.size();
    if(c1_nf != c2_nf || c1_nf != xi_nf || c1_nf != p_nf)
    {
        cout << "number of files for each order parameter do not match.\n";
        throw 1;
    }

    // allocate arrays
    for(int i=0; i<p.N; i++) particleType.push_back(0);
}



// -------------------------------------------------------------------------
// Execute the post-processor:
// -------------------------------------------------------------------------

void ParticleAttachment::executePostProc()
{
    // run post processor on all vtk files
    int tagNum = 0;
    for (size_t f=0; f<c1.vtkFiles.size(); f++) 
    {
        // read in current file data:
        c1.readVTKFile(c1.vtkFiles.at(f));
        c2.readVTKFile(c2.vtkFiles.at(f));
        xi.readVTKFile(xi.vtkFiles.at(f));
        p.readVTKFile(p.vtkFiles.at(f));
        // calculate the number of particles on the interface
        int onInterface = calcNumOnInterface();
        // append particle type info to particle vtk file
        if(generateTypeInfo)
        {
            // first delete old info 
            deleteParticleTypeData(p.vtkFiles[f]);
            // write type info
            writeParticleTypeData(p.vtkFiles[f]);
        }
        // calculate the fraction of particles on the interface
        double fracOn = (double)onInterface/(double)p.N;
        // write output:
        tagNum = c1.outputInterval*f;
        outfile << tagNum << "," << onInterface << "," << fracOn << endl;
    }
}



// -------------------------------------------------------------------------
// calculate the number of particles on the interface
// -------------------------------------------------------------------------

int ParticleAttachment::calcNumOnInterface()
{
    // --------------------------------------------
    // Determine particle type (i.e. 0 = in c1,
    // 1 = on interface, 2 = in c2) and return the
    // number of particles on the interface.
    // --------------------------------------------

    int onInter = 0;
    for(int i=0; i<p.N; i++)
    {
        bool touchingC1 = false;
        bool touchingC2 = false;
        // determine particle parameters in terms of grid
        int cx = p.r[i*3+0]/c1.dx; // x-grid pos
        int cy = p.r[i*3+1]/c1.dy; // y-grid pos
        int cz = p.r[i*3+2]/c1.dz; // z-grid pos
        int drad = ceil(p.rad[i]/c1.dx); // grid nodes per rad length
        int dInner = (drad-1)*(drad-1);
        int dOuter = (drad+1)*(drad+1);

        // determine grid box around particle
        int bWidth = drad+1;
        int nsx = cx - bWidth;
        if(nsx<0) nsx = nsx + c1.nx;
        int nsy = cy - bWidth;
        if(nsy<0) nsy = nsy + c1.ny;
        int nsz = cz - bWidth;
        if(nsz<0) nsz = nsz + c1.nz;

        // check in box to see if particle is touching c1 or c2
        int boxWidth = 2*bWidth + 1;
        for (int iz=0; iz<boxWidth;iz++)
            for (int iy=0; iy<boxWidth;iy++)
                for (int ix=0; ix<boxWidth;ix++)
                {
                    int xn = nsx+ix;
                    if(xn > c1.nx-1) xn = xn - c1.nx;
                    int yn = nsy+iy;
                    if(yn > c1.ny-1) yn = yn - c1.ny;
                    int zn = nsz+iz;
                    if(zn > c1.nz-1) zn = zn - c1.nz;
                    int ind = zn*c1.ny*c1.nx+yn*c1.nx+xn;

                    // only check grid spaces around the surface
                    int dx = abs(xn - cx);
                    if(dx > bWidth) dx -= c1.nx;
                    int dy = abs(yn - cy);
                    if(dy > bWidth) dy -= c1.ny;
                    int dz = abs(zn - cz);
                    if(dz > bWidth) dz -= c1.nz;
                    int r2 = dx*dx + dy*dy + dz*dz;
                    if(r2 >= dInner && r2 <= dOuter)
                    {
                        if(c1.a[ind] > c1thresh && xi.a[ind] > cpthresh) 
                            touchingC1 = true;
                        if(c2.a[ind] > c2thresh && xi.a[ind] > cpthresh) 
                            touchingC2 = true;
                    }
                }
        // set particle type
        if(touchingC1 && touchingC2)
        {
            particleType[i] = 1;
            onInter++;
        }
        else if(touchingC1 && !touchingC2)
            particleType[i] = 0;
        else if(!touchingC1 && touchingC2)
            particleType[i] = 2;
        else
            particleType[i] = 4; // an error has occured
    }

    // return number of particles on the interface
    return onInter;
}



// -------------------------------------------------------------------------
// Append the particle type (i.e. on interface, in c1, or in c2) data to
// the particle vtk files
// -------------------------------------------------------------------------

void ParticleAttachment::writeParticleTypeData(std::string filePath)
{
    // -----------------------------------
    // Open vtk file:
    // -----------------------------------

    ofstream outfile;
    outfile.open(filePath.c_str(), ios::app);
    if(!outfile.is_open())
    {
        cout << "could not open " << filePath << " for writing type data!\n";
        throw 1;
    }

    // -----------------------------------
    // write header info:
    // -----------------------------------

    outfile << "SCALARS onInterface float\n";
    outfile << "LOOKUP_TABLE default\n";

    // -----------------------------------
    // write particle type info.
    // -----------------------------------

    for (int i=0; i<p.N; i++) 
    {
        outfile << particleType[i] << endl;
    }
}



// -------------------------------------------------------------------------
// Delete existing particle type data
// -------------------------------------------------------------------------

void ParticleAttachment::deleteParticleTypeData(std::string filePath)
{
    // ---------------------------------------------------
    // make copy of file then rewrite data minus type data
    // ---------------------------------------------------

    std::string cmd = "cp " + filePath + " temp";
    std::system(cmd.c_str());
    ofstream outfile;
    ifstream infile;
    outfile.open(filePath.c_str(), ios::out);
    infile.open("temp", ios::in);
    if(!outfile.is_open())
    {
        cout << "could not open " << filePath << " for writing data!\n";
        throw 1;
    }
    if(!infile.is_open())
    {
        cout << "could not open " << filePath << " for reading data!\n";
        throw 1;
    }

    // -----------------------------------
    // Read and write VTK file header:
    // -----------------------------------

    std::string line;
    getline(infile,line);  // # vtk DataFile Version 3.1
    outfile << line << endl;
    getline(infile,line);  // VTK file containing particle data
    outfile << line << endl;
    getline(infile,line);  // ASCII
    outfile << line << endl;
    getline(infile,line);  // 
    outfile << line << endl;
    getline(infile,line);  // DATASET POLYDATA
    outfile << line << endl;
    getline(infile,line);  // 
    outfile << line << endl;
    getline(infile,line);  // POINTS    N float
    outfile << line << endl;

    // -----------------------------------
    // Read data particle position data:
    // -----------------------------------

    double rx,ry,rz;
    for (int i=0; i<p.N; i++) 
    {
        infile >> rx >> ry >> rz;
        outfile << rx << "    " << ry << "    " << rz << endl;
    }

    // -----------------------------------
    // Read particle radius data:
    // -----------------------------------

    getline(infile,line); // 
    getline(infile,line); // POINT_DATA  N
    outfile << line << endl;
    getline(infile,line); // SCALARS radius float
    outfile << line << endl;
    getline(infile,line); // LOOKUP_TABLE default
    outfile << line << endl;

    double rad;
    for (int i=0; i<p.N; i++) 
    {
        infile >> rad;
        outfile << rad << endl;
    }

    // -----------------------------------
    // Close files:
    // -----------------------------------

    infile.close();
    outfile.close();

    // -----------------------------------
    // Delete temp file
    // -----------------------------------
    //
    cmd = "rm temp";
    std::system(cmd.c_str());
}
