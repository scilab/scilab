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
// <-- NO CHECK REF -->
// <-- WINDOWS ONLY -->


// Test the main attributes of XML files from differents exporters.

path = fmigetPath();
// CATIA
fmu = importFMU(fullfile(path, "tests", "unit_tests", "CATIA_Modelica_Electrical_Digital_Examples_DFFREG.fmu"));
//attributes
assert_checkequal(fmu.descriptionFMU.fmiVersion, "1.0");
assert_checkequal(fmu.descriptionFMU.modelName, "DFFREG");
assert_checkequal(fmu.descriptionFMU.modelIdentifier, "DFFREG");
assert_checkequal(fmu.descriptionFMU.guid, "{8d861735-f7c2-4c16-ae93-caec11fe6871}");
assert_checkequal(fmu.descriptionFMU.description, "Pulse triggered D-Register-Bank, high active reset");
assert_checkequal(fmu.descriptionFMU.author, []);
assert_checkequal(fmu.descriptionFMU.version, "3.2");
assert_checkequal(fmu.descriptionFMU.generationTool, []);
assert_checkequal(fmu.descriptionFMU.generationDateAndTime, "");
assert_checkequal(fmu.descriptionFMU.variableNamingConvention, "structured");
assert_checkequal(fmu.descriptionFMU.numberOfContinuousStates, 0);
assert_checkequal(fmu.descriptionFMU.numberOfEventIndicators, 4);
//Unit definitions and type definitions
assert_checkequal(fmu.descriptionFMU.UnitDefinitions, 0);
assert_checkequal(fmu.descriptionFMU.TypeDefinitions, 3);
//default experiment
assert_checkequal(fmu.descriptionFMU.DefaultExperiment.startTime, 0.0);
assert_checkequal(fmu.descriptionFMU.DefaultExperiment.stopTime, 1.0);
assert_checkequal(fmu.descriptionFMU.DefaultExperiment.tolerance, 1E-005);
//vendor annotations tool
assert_checkequal(fmu.descriptionFMU.VendorAnnotations, 0);
//Model variables
assert_checkequal(fmu.descriptionFMU.ModelVariables, 442);
 
// Dymola
fmu = importFMU(fullfile(path, "tests", "unit_tests", "Dymola_Modelica_Mechanics_Rotational_Examples_CoupledClutches.fmu"));
//attributes
assert_checkequal(fmu.descriptionFMU.fmiVersion, "1.0");
assert_checkequal(fmu.descriptionFMU.modelName, "Modelica.Mechanics.Rotational.Examples.CoupledClutches");
assert_checkequal(fmu.descriptionFMU.modelIdentifier, "Modelica_Mechanics_Rotational_Examples_CoupledClutches");
assert_checkequal(fmu.descriptionFMU.guid, "{1186dc68-d82d-440c-8d1a-5a91ea6b13b8}");
assert_checkequal(fmu.descriptionFMU.description, "Drive train with 3 dynamically coupled clutches");
assert_checkequal(fmu.descriptionFMU.author, []);
assert_checkequal(fmu.descriptionFMU.version, "3.2");
assert_checkequal(fmu.descriptionFMU.generationTool, "Dymola Version 2013 FD01 (64-bit), 2012-10-18");
assert_checkequal(fmu.descriptionFMU.generationDateAndTime, "2012-12-17T14:58:20Z");
assert_checkequal(fmu.descriptionFMU.variableNamingConvention, "structured");
assert_checkequal(fmu.descriptionFMU.numberOfContinuousStates, 8);
assert_checkequal(fmu.descriptionFMU.numberOfEventIndicators, 54);
//Unit definitions and type definitions
assert_checkequal(fmu.descriptionFMU.UnitDefinitions, 1);
assert_checkequal(fmu.descriptionFMU.TypeDefinitions, 12);
//default experiment
assert_checkequal(fmu.descriptionFMU.DefaultExperiment.startTime, 0.0);
assert_checkequal(fmu.descriptionFMU.DefaultExperiment.stopTime, 1.5);
assert_checkequal(fmu.descriptionFMU.DefaultExperiment.tolerance, 0.0001);
//vendor annotations tool
assert_checkequal(fmu.descriptionFMU.VendorAnnotations, 0);
//Model variables
assert_checkequal(fmu.descriptionFMU.ModelVariables, 199);

// MapleSIM
fmu = importFMU(fullfile(path, "tests", "unit_tests", "MapleSIM_Rectifier1_0_Win32_64bits.fmu"));
//attributes
assert_checkequal(fmu.descriptionFMU.fmiVersion, "1.0");
assert_checkequal(fmu.descriptionFMU.modelName, "Rectifier1_0");
assert_checkequal(fmu.descriptionFMU.modelIdentifier, "Rectifier1_0");
assert_checkequal(fmu.descriptionFMU.guid, "{60224b9b5-d85e-bc6c-ac93-05fdbf390b6}");
assert_checkequal(fmu.descriptionFMU.description, []);
assert_checkequal(fmu.descriptionFMU.author, []);
assert_checkequal(fmu.descriptionFMU.version, []);
assert_checkequal(fmu.descriptionFMU.generationTool, []);
assert_checkequal(fmu.descriptionFMU.generationDateAndTime, []);
assert_checkequal(fmu.descriptionFMU.variableNamingConvention, []);
assert_checkequal(fmu.descriptionFMU.numberOfContinuousStates, 4);
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
assert_checkequal(fmu.descriptionFMU.ModelVariables, 61);

// SumualtionX
fmu = importFMU(fullfile(path, "tests", "unit_tests", "SimulationX_CoupledClutches.fmu"));
//attributes
assert_checkequal(fmu.descriptionFMU.fmiVersion, "1.0");
assert_checkequal(fmu.descriptionFMU.modelName, "CoupledClutches");
assert_checkequal(fmu.descriptionFMU.modelIdentifier, "CoupledClutches");
assert_checkequal(fmu.descriptionFMU.guid, "{45C0AE2E-03C9-48BD-9CEB-4DE8A4B76C60}");
assert_checkequal(fmu.descriptionFMU.description, "CoupledClutches");
assert_checkequal(fmu.descriptionFMU.author, "Bastian Binder");
assert_checkequal(fmu.descriptionFMU.version, []);
assert_checkequal(fmu.descriptionFMU.generationTool, "SimulationX 3.5.707.4 (15.04.13) x64");
assert_checkequal(fmu.descriptionFMU.generationDateAndTime, "2013-04-17T15:01:04");
assert_checkequal(fmu.descriptionFMU.variableNamingConvention, "structured");
assert_checkequal(fmu.descriptionFMU.numberOfContinuousStates, 8);
assert_checkequal(fmu.descriptionFMU.numberOfEventIndicators, 28);
//Unit definitions and type definitions
assert_checkequal(fmu.descriptionFMU.UnitDefinitions, 0);
assert_checkequal(fmu.descriptionFMU.TypeDefinitions, 0);
//default experiment
assert_checkequal(fmu.descriptionFMU.DefaultExperiment.startTime, 0);
assert_checkequal(fmu.descriptionFMU.DefaultExperiment.stopTime, 1.5);
assert_checkequal(fmu.descriptionFMU.DefaultExperiment.tolerance, 1e-005);
//vendor annotations tool
assert_checkequal(fmu.descriptionFMU.VendorAnnotations, 0);
//Model variables
assert_checkequal(fmu.descriptionFMU.ModelVariables, 5);
// inputs 
fmu.setValues.inputs = ["clutch3.f_normalized"];
