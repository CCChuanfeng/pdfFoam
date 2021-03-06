/*---------------------------------------------------------------------------*\
                pdfFoam: General Purpose PDF Solution Algorithm
                   for Reactive Flow Simulations in OpenFOAM

 Copyright (C) 2012 Michael Wild, Heng Xiao, Patrick Jenny,
                    Institute of Fluid Dynamics, ETH Zurich
-------------------------------------------------------------------------------
License
    This file is part of pdfFoam.

    pdfFoam is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) version 3 of the same License.

    pdfFoam is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with pdfFoam.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::mcSamplingInletRandom

Description
    Generate wall-normal particle-velocity for an inlet boundary

    Generate random values for the velocity inlet using a sampling method
    (simulated ghost cell).


SourceFiles
    mcSamplingInletRandom.C

Author
    Michael Wild

\*---------------------------------------------------------------------------*/

#ifndef mcSamplingInletRandom_H
#define mcSamplingInletRandom_H

#include "mcInletRandom.H"

#include "FIFOStack.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class Random;

/*---------------------------------------------------------------------------*\
                    Class mcSamplingInletRandom Declaration
\*---------------------------------------------------------------------------*/

class mcSamplingInletRandom
   : public mcInletRandom
{
    // Private data

        FIFOStack<scalar> buf_;

    // Private member functions

        // Disallow default bitwise copy construct and assignment
        mcSamplingInletRandom(const mcSamplingInletRandom&);
        void operator=(const mcSamplingInletRandom&);

public:

    //- Runtime type information
        TypeName("mcSamplingInletRandom");

    // Constructors

        //- Construct a random number generator given a Random object and
        // the mean velocity and the RMS of the fluctuations
        mcSamplingInletRandom
        (
            Random& rnd,
            scalar UMean,
            scalar uRms,
            const dictionary& dict
        );

    // Public member functions

        //- Generate one random number
        virtual scalar value();

        //- Update the coefficients for changed UMean and uRms
        virtual void updateCoeffs(scalar UMean, scalar uRms);
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
