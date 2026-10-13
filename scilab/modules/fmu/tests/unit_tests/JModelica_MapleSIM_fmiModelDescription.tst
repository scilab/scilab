//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Vladislav Trubkin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

// Test the main attributes of XML files from differents exporters of FMUs.

// <-- NO CHECK REF -->
// <-- LINUX ONLY -->

path = fmigetPath();
[version, opts] = getversion();
if opts(2) == "x64" then
    // JModelica
    fmu = importFMU(fullfile(path, "tests", "unit_tests", "JModelica_CoupledClutches.fmu"));
    // attributes
    assert_checkequal(fmu.descriptionFMU.fmiVersion, "1.0");
    assert_checkequal(fmu.descriptionFMU.modelName, "CoupledClutches");
    assert_checkequal(fmu.descriptionFMU.modelIdentifier, "CoupledClutches");
    assert_checkequal(fmu.descriptionFMU.guid, "da8d25ac7d21d45362251292e7b75916");
    assert_checkequal(fmu.descriptionFMU.description, []);
    assert_checkequal(fmu.descriptionFMU.author, []);
    assert_checkequal(fmu.descriptionFMU.version, []);
    assert_checkequal(fmu.descriptionFMU.generationTool, []);
    assert_checkequal(fmu.descriptionFMU.generationDateAndTime, "2013-05-17T15:11:09");
    assert_checkequal(fmu.descriptionFMU.variableNamingConvention, "structured");
    assert_checkequal(fmu.descriptionFMU.numberOfContinuousStates, 8);
    assert_checkequal(fmu.descriptionFMU.numberOfEventIndicators, 36);
    //Unit definitions and type definitions
    assert_checkequal(fmu.descriptionFMU.UnitDefinitions, 1);
    assert_checkequal(fmu.descriptionFMU.TypeDefinitions, 1);
    //default experiment
    assert_checkequal(fmu.descriptionFMU.DefaultExperiment.startTime, []);
    assert_checkequal(fmu.descriptionFMU.DefaultExperiment.stopTime, 1.5);
    assert_checkequal(fmu.descriptionFMU.DefaultExperiment.tolerance, []);
    //vendor annotations tool
    assert_checkequal(fmu.descriptionFMU.VendorAnnotations, 1);
    //Model variables
    assert_checkequal(fmu.descriptionFMU.ModelVariables, 218);
    // MapleSIM
    fmu = importFMU(fullfile(path, "tests", "unit_tests", "MapleSIM_CoupledClutches_Linux64bits.fmu"));
    // attributes
    assert_checkequal(fmu.descriptionFMU.fmiVersion, "1.0");
    assert_checkequal(fmu.descriptionFMU.modelName, "CC");
    assert_checkequal(fmu.descriptionFMU.modelIdentifier, "CC");
    assert_checkequal(fmu.descriptionFMU.guid, "{9c71e8450-a5bd-f8c0-d8eb-162acfca8ee}");
    assert_checkequal(fmu.descriptionFMU.description, []);
    assert_checkequal(fmu.descriptionFMU.author, []);
    assert_checkequal(fmu.descriptionFMU.version, []);
    assert_checkequal(fmu.descriptionFMU.generationTool, []);
    assert_checkequal(fmu.descriptionFMU.generationDateAndTime, []);
    assert_checkequal(fmu.descriptionFMU.variableNamingConvention, []);
    assert_checkequal(fmu.descriptionFMU.numberOfContinuousStates, 8);
    assert_checkequal(fmu.descriptionFMU.numberOfEventIndicators, 0);
    //Unit definitions and type definitions
    assert_checkequal(fmu.descriptionFMU.UnitDefinitions, 0);
    assert_checkequal(fmu.descriptionFMU.TypeDefinitions, 0);
    //default experiment
    assert_checkequal(fmu.descriptionFMU.DefaultExperiment.startTime, []);
    assert_checkequal(fmu.descriptionFMU.DefaultExperiment.stopTime, []);
    assert_checkequal(fmu.descriptionFMU.DefaultExperiment.tolerance, []);
    //vendor annotations tool
    assert_checkequal(fmu.descriptionFMU.VendorAnnotations, 0);
    //Model variables
    assert_checkequal(fmu.descriptionFMU.ModelVariables, 147);
else
    // MapleSIM
    fmu = importFMU(fullfile(path, "tests", "unit_tests", "MapleSIM_CoupledClutches_Linux32bits.fmu"));
    // attributes
    assert_checkequal(fmu.descriptionFMU.fmiVersion, "1.0");
    assert_checkequal(fmu.descriptionFMU.modelName, "CC");
    assert_checkequal(fmu.descriptionFMU.modelIdentifier, "CC");
    assert_checkequal(fmu.descriptionFMU.guid, "{9a8f01d8f-213f-5345-6836-245eae25039}");
    assert_checkequal(fmu.descriptionFMU.description, []);
    assert_checkequal(fmu.descriptionFMU.author, []);
    assert_checkequal(fmu.descriptionFMU.version, []);
    assert_checkequal(fmu.descriptionFMU.generationTool, []);
    assert_checkequal(fmu.descriptionFMU.generationDateAndTime, []);
    assert_checkequal(fmu.descriptionFMU.variableNamingConvention, []);
    assert_checkequal(fmu.descriptionFMU.numberOfContinuousStates, 8);
    assert_checkequal(fmu.descriptionFMU.numberOfEventIndicators, 0);
    //Unit definitions and type definitions
    assert_checkequal(fmu.descriptionFMU.UnitDefinitions, 0);
    assert_checkequal(fmu.descriptionFMU.TypeDefinitions, 0);
    //default experiment
    assert_checkequal(fmu.descriptionFMU.DefaultExperiment.startTime, []);
    assert_checkequal(fmu.descriptionFMU.DefaultExperiment.stopTime, []);
    assert_checkequal(fmu.descriptionFMU.DefaultExperiment.tolerance, []);
    //vendor annotations tool
    assert_checkequal(fmu.descriptionFMU.VendorAnnotations, 0);
    //Model variables
    assert_checkequal(fmu.descriptionFMU.ModelVariables, 147);
end
