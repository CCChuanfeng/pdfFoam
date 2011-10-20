/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2010 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::mcOmegaModel

Description
    Abstract base class for Monte Carlo turbulent frequency models

SourceFiles
    mcOmegaModel.C

Author
    Michael Wild

\*---------------------------------------------------------------------------*/

#ifndef mcOmegaModel_H
#define mcOmegaModel_H

#include "autoPtr.H"
#include "dictionary.H"
#include "interpolationCellPoint.H"
#include "runTimeSelectionTables.H"
#include "typeInfo.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class mcParticle;
class mcParticleCloud;

/*---------------------------------------------------------------------------*\
                       Class mcOmegaModel Declaration
\*---------------------------------------------------------------------------*/

class mcOmegaModel :
    public dictionary
{

    // Private Member Functions

        // Disallow default bitwise copy construct and assignment
        mcOmegaModel(const mcOmegaModel&);
        void operator=(const mcOmegaModel&);

public:

    //- Runtime type information
    TypeName("mcOmegaModel");


    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            mcOmegaModel,
            mcOmegaModel,
            (
                const dictionary& mcOmegaModelDict
            ),
            (mcOmegaModelDict)
        );

    // Constructors

        //- Construct from components
        mcOmegaModel
        (
            const dictionary& mcOmegaModelDict
        );


    // Selectors

        //- Return a reference to the selected Omega model
        static autoPtr<mcOmegaModel> New
        (
            const dictionary& mcOmegaModelDict
        );

    //- Destructor
        virtual ~mcOmegaModel()
        {}

    // Member Functions

        //- Apply the Omega model to the particle cloud
        virtual void correct(mcParticleCloud& cloud) = 0;

        //- Apply the Omega model to a single particle
        virtual void correct
        (
            mcParticleCloud& cloud,
            mcParticle& p,
            bool prepare=true
        ) = 0;

};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //