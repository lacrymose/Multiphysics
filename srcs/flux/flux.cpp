#include <iostream>
#include "flux.hpp"


// see .hpp file for description
void fluxTransport(Field& field, PartialField& partialField, const SolverParams& solverParams, bool boundary)
{
    // the physical flux is F = a*Q", where "a" is a 2D vector
    if(boundary)
    {
        // flux for the x coordinate
        partialField.FluxAtBC[0][0] = solverParams.fluxCoeffs[0]*partialField.uAtBC[0];
        // flux for the y coordinate
        partialField.FluxAtBC[1][0] = solverParams.fluxCoeffs[1]*partialField.uAtBC[0];
    }
    else
    {
        // flux for the x coordinate
        field.flux[0][0] = solverParams.fluxCoeffs[0]*field.u[0];
        // flux for the y coordinate
        field.flux[1][0] = solverParams.fluxCoeffs[1]*field.u[0];
    }
}


// see .hpp file for description
void fluxShallow(Field& field, PartialField& partialField, const SolverParams& solverParams, bool boundary)
{
    // gravity parameter
    double g = solverParams.fluxCoeffs[0];

    // the physical flux is given by
    //  Fx = [H*u,   H*u^2 + g*H^2/2,    H*u*v       ]
    //  Fy = [H*v,   H*u*v,              H*v^2 + g*H ]

    if(boundary)
    {
        // flux for the (x, y) coordinates for H
        partialField.FluxAtBC[0][0] = partialField.uAtBC[1];
        partialField.FluxAtBC[1][0] = partialField.uAtBC[2];

        // flux for the (x, y) coordinates for H*u
        partialField.FluxAtBC[0][1] = partialField.uAtBC[1]*partialField.uAtBC[1]/partialField.uAtBC[0]
                                + g/2*partialField.uAtBC[0]*partialField.uAtBC[0];
        partialField.FluxAtBC[0][2] = partialField.uAtBC[1]*partialField.uAtBC[2]/partialField.uAtBC[0];

        // flux for the (x, y) coordinates for H*v
        partialField.FluxAtBC[1][1] = partialField.uAtBC[1]*partialField.uAtBC[2]/partialField.uAtBC[0];
        partialField.FluxAtBC[1][2] = partialField.uAtBC[2]*partialField.uAtBC[2]/partialField.uAtBC[0]
                                + g/2*partialField.uAtBC[0]*partialField.uAtBC[0];
    }
    else
    {
        if (field.u[0].minCoeff() <= 0)
        {
            std::cerr << "WARNING: Negative height or division by 0 !" << std::endl;
        }

        // flux for the (x, y) coordinates for H
        field.flux[0][0] = field.u[1];
        field.flux[1][0] = field.u[2];

        // flux for the (x, y) coordinates for H*u
        field.flux[0][1] = field.u[1].array().square()/field.u[0].array()
                            + g/2*field.u[0].array().square();
        field.flux[0][2] = field.u[1].array()*field.u[2].array()/field.u[0].array();

        // flux for the (x, y) coordinates for H*v
        field.flux[1][1] = field.u[1].array()*field.u[2].array()/field.u[0].array();
        field.flux[1][2] = field.u[2].array().square()/field.u[0].array()
                            + g/2*field.u[0].array().square();
    }
}



// see .hpp file for description
void fluxShallowLin(Field& field, PartialField& partialField, const SolverParams& solverParams, bool boundary)
{

    // the physical flux is given by
    //  Fx = [h0*u,     g*h0*H, 0       ]
    //  Fy = [h0*v,     0,      g*h_0*h ]
    
    if(boundary)
    {
        // flux for the (x, y) coordinates for H
        partialField.FluxAtBC[0][0] = partialField.uAtBC[1];
        partialField.FluxAtBC[1][0] = partialField.uAtBC[2];

        // flux for the (x, y) coordinates for H*u
        partialField.FluxAtBC[0][1] = solverParams.fluxCoeffs[0]
                                *partialField.uAtBC[0]*solverParams.fluxCoeffs[1];
        partialField.FluxAtBC[0][2] = 0.0;

        // flux for the (x, y) coordinates for H*v
        partialField.FluxAtBC[1][1] = 0.0;
        partialField.FluxAtBC[1][2] = solverParams.fluxCoeffs[0]
                                *partialField.uAtBC[0]*solverParams.fluxCoeffs[1];
    }
    else
    {
        if (field.u[0].minCoeff() <= 0)
        {
            std::cerr << "WARNING: Negative height or division by 0 !" << std::endl;
        }

        // flux for the (x, y) coordinates for H
        field.flux[0][0] = field.u[1];
        field.flux[1][0] = field.u[2];

        // flux for the (x, y) coordinates for u
        field.flux[0][1] = solverParams.fluxCoeffs[0]*solverParams.fluxCoeffs[1]
                                *field.u[0];
        // field.flux[0][2] = Eigen::VectorXd::Zero(field.u[0].size());

        // flux for the (x, y) coordinates for v
        // field.flux[1][1] = Eigen::VectorXd::Zero(field.u[0].size());
        field.flux[1][2] = solverParams.fluxCoeffs[0]*solverParams.fluxCoeffs[1]
                                *field.u[0];
    }
}

