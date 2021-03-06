    Info<< "Reading field U\n" << endl;
    volVectorField U
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading thermophysical properties\n" << endl;
    mcThermo thermo(mesh);
    thermo.correct();
    volScalarField& rho = thermo.rho();
    volScalarField& p = thermo.p();

    #include "compressibleCreatePhi.H"

    label pRefCell = 0;
    scalar pRefValue = 0.0;
    setRefCell(p, mesh.solutionDict().subDict("SIMPLE"), pRefCell, pRefValue);

    Info<< "Creating turbulence model\n" << endl;
    autoPtr<compressible::RASModel> turbulence
    (
        compressible::RASModel::New
        (
            rho,
            U,
            phi,
            thermo
        )
    );

    thermo.createCloud();

    dimensionedScalar initialMass = fvc::domainIntegrate(rho);
