
# ifndef AVEFIELD_H
# define AVEFIELD_H

# include "../base/PesoBase.hpp"
# include "../utils/vtkField.hpp"


class AveField : public PesoBase {

private:

	vtkField c;
	std::string tagName;
    int tagInterval;
    int numFiles;
	int nx,ny,nz;

public:

	AveField();
	~AveField();
	void setupPostProc();
	void executePostProc();
	void outputData(int,double);

};

# endif  // AVEFIELD_H
