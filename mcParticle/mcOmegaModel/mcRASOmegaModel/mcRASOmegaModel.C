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

\*---------------------------------------------------------------------------*/

#include "mcRASOmegaModel.H"

#include "addToRunTimeSelectionTable.H"
#include "compressible/turbulenceModel/turbulenceModel.H"
#include "compressible/RAS/kOmegaSST/kOmegaSST.H"
#include "mcParticleCloud.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{

    defineTypeNameAndDebug(mcRASOmegaModel, 0);
    addNamedToRunTimeSelectionTable
    (
        mcOmegaModel,
        mcRASOmegaModel,
        mcOmegaModel,
        RAS
    );

} // namespace Foam

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::mcRASOmegaModel::mcRASOmegaModel(const Foam::dictionary& dict)
:
    mcOmegaModel(dict),
    OmegaInterp_()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::mcRASOmegaModel::setupInterpolator
(
    const Foam::mcParticleCloud& cloud
)
{
    interpolationCellPoint<scalar>* interp;
    const compressible::turbulenceModel& turbModel = cloud.turbulenceModel();
    if (isA<compressible::RASModels::kOmegaSST>(turbModel))
    {
        // If we have a kOmegaSST (or derived) object, use omega directly
        const compressible::RASModels::kOmegaSST& kOmegaModel =
            refCast<const compressible::RASModels::kOmegaSST>(turbModel);
        interp = new interpolationCellPoint<scalar>(kOmegaModel.omega());
    }
    else
    {
        // Otherwise compute Omega from epsilon/k
        interp = new interpolationCellPoint<scalar>
        (
            turbModel.epsilon() / turbModel.k()
        );
    }
    OmegaInterp_.reset(interp);
}


void Foam::mcRASOmegaModel::correct(Foam::mcParticleCloud& cloud)
{
    setupInterpolator(cloud);
    forAllIter(mcParticleCloud, cloud, pIter)
    {
        correct(cloud, pIter(), false);
    }
}


void Foam::mcRASOmegaModel::correct
(
    Foam::mcParticleCloud& cloud,
    Foam::mcParticle& p,
    bool prepare
)
{
    if (prepare)
    {
        setupInterpolator(cloud);
    }
    else
    {
        if (!OmegaInterp_.valid())
        {
            FatalErrorIn("mcRASOmegaModel::correct"
                "(mcParticleCloud&,mcParticle&,bool)")
                << "Interpolator not initialized"
                << exit(FatalError);
        }
    }
    const fvMesh& mesh = cloud.mesh();
    cellPointWeight cpw(mesh, p.position(), p.cell(), p.face());
    p.Omega() = OmegaInterp_().interpolate(cpw);
}

// ************************************************************************* //