#include <cmath>
#include <iostream>
#include <gmsh.h>
#include <Eigen/Sparse>
#include <Eigen/Dense>
#include "computeNorm.hpp"
#include "computeError.hpp"


bool computeError(const Mesh& mesh, const SolverParams& solverParams, const std::string& meshName,
                     const std::string& resultsName, double& errorL2, double& errorLinf)
{
	gmsh::initialize();
	gmsh::option::setNumber("General.Terminal", 1);
	gmsh::open(resultsName);
	std::vector<int> tags;
	gmsh::view::getTags(tags);

	std::string dataType;
	int numComponents;
	std::vector<int> elementTags;
	std::vector<unsigned int> elementNumNodes = mesh.nodeData.elementNumNodes;
    std::vector<std::vector<double>> data(elementNumNodes.size());

	double t = 0.4;
	int step = t/solverParams.timeStep;

    //get the results computed by the solver
    gmsh::view::getModelData(tags[0], step, dataType, elementTags,
                data, t, numComponents);

    int totalSize = data.size()*data[0].size();

    //put the field in a vector
    Eigen::VectorXd u(totalSize);
    int k=0;
    for (int i = 0; i < data.size(); ++i)
    {
    	for (int j = 0; j < data[i].size(); ++j)
    	{
    		u[k] = data[i][j];
    		++k;
    	}
    }


gmsh::finalize();

gmsh::initialize();
gmsh::option::setNumber("General.Terminal", 1);
gmsh::open(meshName);
computeNorm(mesh, solverParams, t, u, errorL2, errorLinf);
gmsh::finalize();

std::cout << "Error value:" << errorL2  << "\t" << errorLinf << std::endl;
        

    return true;
}