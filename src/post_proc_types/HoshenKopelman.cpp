
# include "HoshenKopelman.hpp"
# include <algorithm>
# include <cmath>
# include <ctime>
using namespace std;



// -------------------------------------------------------------------------
// Constructor:
// -------------------------------------------------------------------------

HoshenKopelman::HoshenKopelman() : c()
{
    GetPot InParams("inputPeso.dat");
    tagName = InParams("HoshenKopelman/tagName","c1");
    hkTagName = InParams("HoshenKopelman/hkTagName","hk");
    range = InParams("HoshenKopelman/range","greater");
    cutoff = InParams("HoshenKopelman/cutoff",0.1);

    // open file for writing output
    std::stringstream filenamecombine;
    filenamecombine << "postoutput/" << tagName << "_" << "HoshenKopelman.dat";
    string filename = filenamecombine.str();
    outfile.open(filename.c_str(), ios::out | ios::trunc);
    if(!outfile.is_open())
    {
        cout << "could not open " << filename << " for writing!";
        throw 1;
    }

    // write output file header
    outfile << "step,numOfClusters,avgVol,phaseVol\n";
}



// -------------------------------------------------------------------------
// Destructor:
// -------------------------------------------------------------------------

HoshenKopelman::~HoshenKopelman()
{
    // close output file
    outfile.close();
}



// -------------------------------------------------------------------------
// Set-up the post-processor:
// -------------------------------------------------------------------------

void HoshenKopelman::setupPostProc()
{
    c.setTagName(tagName);
    c.getSimInfo();
}



// -------------------------------------------------------------------------
// Execute the post-processor:
// -------------------------------------------------------------------------

void HoshenKopelman::executePostProc()
{
    // run post processor on all vtk files
    for (size_t f=0; f<c.vtkFiles.size(); f++) 
    {
        // read in current file data:
        c.readVTKFile(c.vtkFiles.at(f));
        executeHK();
        int step = f*c.outputInterval;
        outfile << step << "," << numClust << ",";
        outfile << avgClusterVol << "," << phaseVol << endl;
        writeHK_VTK(step);
    }

}



int HoshenKopelman::new_cluster(int &max_label, std::vector<int> &labels)
{
    max_label++;
    labels[max_label] = max_label;
    return max_label;
}

//Find will return the proper value for the equivalence class described by the 
//input argument
int HoshenKopelman::find(int x, std::vector<int> &labels)
{
    int y = x;
    while (labels[y] != y)
        y = labels[y];

    while (labels[x] != x)
    {
        int z = labels[x];
        labels[x] = y;
        x = z;
    }


    return y;
}

//make_union recognizes labeling conflicts and unifies clusters into the same
//equivalence class
void  HoshenKopelman::make_union(int x, int y, std::vector<int> &labels)
{

    labels[find(y, labels)] = find(x, labels);
    labels[y] = labels[x];
}



// -------------------------------------------------------------------------
// Brian's code:
// -------------------------------------------------------------------------

void HoshenKopelman::executeHK()
{
    clock_t begin = clock(); //start timer

    double n = c.nx*c.ny*c.nz;
    double kSpace = c.nx*c.ny;
    double jSpace = c.nx;

    double phase = 0;

    double max_clusters = n / 2;
    vector<int> labels(max_clusters, 0); //vector to store equivalence classes
    vector< vector<int> > labels2(labels.size(), vector<int>(2,0)); //vector of vectors for re-work of labels
    vector<int> neighbors(6,0);
    vector<int> neighbors2(6,0);
    vector<double> allSizes(max_clusters,0);

    int noise = 0;
    int max_neighbor = 0;
    int min_neighbor = 0;
    int neighbor_count = 0;
    int max_label = 0;
    int k = 0;          //current height
    int j = 0;          //row number
    int i = 0;          //column number
    int up = 0;         //initialize neighbor variable
    int down = 0;       //initialize neighbor variable  
    int left = 0;       //initialize neighbor variable
    int right = 0;      //initialize neighbor variable
    int top = 0;        //initialize neighbor variable
    int bottom = 0;     //initialize neighbor variable

    //////////////////////////////////////////////////////////////////////		
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////

    double point;
    for (int a = 0; a < n; a++)
    {
        point = c.a[a]; 
        if (range=="less") {
            if (point < cutoff) { 
                c.a[a] = 1;
                phase++;
            }
            else
                c.a[a] = 0;
        }
        else if ( range=="greater") {
            if (point > cutoff) { 
                c.a[a] = 1;
                phase++;
            }
            else
                c.a[a] = 0;
        }


        // Determination of cluster
        if (c.a[a] > 0) 
        {
            i = a - k*kSpace - j*jSpace;
            if (j == 0)
                down = 0;
            else
                down = labels[c.a[a - c.nx]];
            if (i == 0)
                left = 0;
            else
                left = labels[c.a[a - 1]];
            if (j == c.ny - 1)
                up = labels[c.a[a - j*c.nx]];
            else
                up = 0;
            if (i == c.nx - 1)
                right = labels[c.a[a - c.nx + 1]];
            else 
                right = 0;
            if (k == 0)
                bottom = 0;
            else
                bottom = labels[c.a[a - kSpace]];
            if (k == c.nz - 1)
                top = labels[c.a[a - k*kSpace]];
            else
                top = 0;
            // Populate neighbors vector and count relevant neighbors
            neighbors = {up, down, left, right, top, bottom};
            neighbor_count = 0;
            for (size_t m = 0; m < neighbors.size(); m++){
                if (neighbors[m] > 0)
                    neighbor_count++;
            }

            max_neighbor = *max_element(neighbors.begin(),neighbors.end());

            sort(neighbors.begin(), neighbors.end());
            bool min_check = false;
            for (size_t m = 0; m < neighbors.size(); m++){
                if (min_check == false && neighbors[m] != 0){
                    min_neighbor = neighbors[m];
                    min_check = true;
                }
            }


            // Create new cluster if no neighbors are relevant
            if (neighbor_count == 0){
                c.a[a] = new_cluster(max_label, labels);
                allSizes[max_label+1] += 1;
            }
            // Assign value of relevant neighbor if only one exists
            else if (neighbor_count == 1){
                c.a[a] = max_neighbor;
                allSizes[max_neighbor] += 1;
            }
            // Determine smallest neighbor cluster value and merge relevant neighbors into that cluster
            else {
                for (size_t m = 0; m < neighbors.size(); m++){
                    neighbors2[m] = neighbors[m];
                    if (neighbors2[m] == 0){
                        neighbors2[m] = 1000000;
                    }
                }
                int min_m = 0;
                int min_n2 = *min_element(neighbors2.begin(), neighbors2.end());
                for (size_t m = 0; m < neighbors2.size(); m++){
                    if (neighbors2[m] == 1000000){
                        neighbors2[m] = 0;
                    }
                    if (neighbors2[m] == min_n2){
                        min_m = m;
                    }
                }
                for (size_t m = 0; m < neighbors2.size(); m++){
                    c.a[a] = neighbors[min_m];
                    allSizes[neighbors[min_m]] += 1;
                    if (neighbors[m] != 0){
                        if (neighbors[min_m] <= neighbors[m] && neighbors[min_m] <= min_neighbor)
                        {
                            make_union(neighbors[min_m], neighbors[m], labels);
                            allSizes[neighbors[min_m]] += allSizes[neighbors[m]];
                            allSizes[neighbors[m]] = 0;
                        }
                    }
                }	
            }	
        }
        if (a > 0)
        {
            if ((a+1)%c.nx*c.ny == 0)
                k++;
            if ((a+1)%c.ny == 0)
                j++;
        }
    }

    /////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////

    for (int m = 0; m < max_clusters; m++)
    {
        if (labels[m] != 0)
        {
            labels[m] = labels[labels[m]];
            /* allSizes[m]; */
        }
    }



    //Re-work labels to be consecutive

    for (size_t i = 0; i < labels.size(); i++){

        labels2[i][1] = labels[i];
        labels2[i][0] = i;
    }

    sort(labels2.begin(), labels2.end(), [](const std::vector< int >& a, const std::vector< int >& b){ return a[1] < b[1]; });

    int old_label = 0;
    int new_label = 0;
    bool check = false;

    for (size_t i = 2; i < labels.size(); i++){

        if(check == true && labels2[i][1] != old_label){
            check = false;
        }

        if(check == true){
            labels2[i][1] = new_label;
        }

        if (labels2[i][1] > labels2[i-1][1] && check == false){
            old_label = labels2[i][1];
            new_label = labels2[i-1][1] + 1;
            labels2[i][1] = new_label;
            check = true;
        }

    }

    sort(labels2.begin(), labels2.end(), [](const std::vector< int >& a, const std::vector< int >& b){ return a[0] < b[0]; });

    for (size_t i = 0; i < labels.size(); i++){
        labels[i] = labels2[i][1];
    }


    int new_max = *max_element(labels.begin(), labels.end());
    vector<double> sizes(new_max +1,0);

    // Assign newly ordered labels to arr
    for (int a = 0; a < n; a++)
    {
        if (c.a[a] > 0)
        {
            c.a[a] = labels[c.a[a]];
            sizes[c.a[a]] += 1;
        }	
    }

    /////////////////////////////////////////////////////
    /////////////////////////////////////////////////////
    // Attempted addition to code 9/14/17 ///////////////
    /////////////////////////////////////////////////////

    vector< vector<double> > sizeAndLabels(new_max + 1, vector<double>(3,0)); //vector of vectors for rework of labels
    for (size_t i = 0; i < sizes.size(); i++){
        sizeAndLabels[i][1] = sizes[i];
        sizeAndLabels[i][0] = i;
    }

    sort(sizeAndLabels.begin()+1, sizeAndLabels.end(), [](const std::vector< double >& a, const std::vector< double >& b){ return a[1] < b[1]; });

    vector< int > sizeChecks(new_max+1,0);
    for (int a = 0; a < n; a++)
    {
        if (c.a[a] > 0)
        {
            for (size_t n = 1; n < sizes.size(); n++)
            {
                if (sizeAndLabels[n][0] == c.a[a])
                {
                    c.a[a] = n;
                    n = sizes.size();
                }
            }
        }	
    }



    /* double max_size = *max_element(sizes.begin(), sizes.end()); */
    new_max -= noise;
    /* cout << new_max << " " << max_size << endl; */

    sizes[0] = 0;

    double clusterVolSum = 0.0;
    for (size_t k = 1; k < sizes.size(); k++)
    {
        sizes[k] = sizes[k]/c.nxyz;
        if (sizes[k] != 0)
        {
            clusterVolSum += sizes[k];
        }
    }
    phaseVol = clusterVolSum;
    avgClusterVol = clusterVolSum/new_max;
    numClust = new_max;


    //////////////////////////////////////////////////////////////////////		
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////	
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << endl << elapsed_secs << endl;
}



// -------------------------------------------------------------------------
// Write the vtkouput for 
// -------------------------------------------------------------------------

void HoshenKopelman::writeHK_VTK(int step)
{

    // -----------------------------------
    // Define the file location and name:
    // -----------------------------------

    ofstream outfile;
    std::stringstream filenamecombine;
    filenamecombine << "vtkoutput/" << hkTagName << "_" << step << ".vtk";
    string filename = filenamecombine.str();
    outfile.open(filename.c_str(), ios::out);

    // -----------------------------------
    //	Write the 'vtk' file header:
    // -----------------------------------

    string d = "   ";
    outfile << "# vtk DataFile Version 3.1" << endl;
    outfile << "VTK file containing grid data" << endl;
    outfile << "ASCII" << endl;
    outfile << " " << endl;
    outfile << "DATASET STRUCTURED_POINTS" << endl;
    outfile << "DIMENSIONS" << d << c.nx << d << c.ny << d << c.nz << endl;
    outfile << "ORIGIN " << d << 0 << d << 0 << d << 0 << endl;
    outfile << "SPACING" << d << c.dx << d << c.dy << d << c.dz << endl;
    outfile << " " << endl;
    outfile << "POINT_DATA " << c.nxyz << endl;
    outfile << "SCALARS " << hkTagName << " float" << endl;
    outfile << "LOOKUP_TABLE default" << endl;

    // -----------------------------------
    //	Write the data:
    // -----------------------------------

    for (size_t a = 0; a < c.a.size(); a++)
        outfile << c.a[a] << endl;
    outfile.close();
}
