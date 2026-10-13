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

// import FMU test

// Windows
// all ok tests
path = fmigetPath();
function checkUnzip(modelPath)
    assert_checkequal(execstr("importFMU(modelPath)","errcatch",'m'), 0);
endfunction

if getos() == "Windows" then
    modelPath = [...
        "CATIA_Modelica_Electrical_Digital_Examples_DFFREG.fmu"
        "FMUSDK_2.0.4_win64_bouncingBall.fmu"
        "CATIA_Modelica_Mechanics_MultiBody_Examples_Loops_Engine1b.fmu"
        "CATIA_Modelica_Mechanics_Rotational_Examples_CoupledClutches.fmu"
        "SCADE_CruiseControl__CruiseControl_FMU.fmu"
        "Dymola_Modelica_Electrical_Digital_Examples_DFFREG.fmu"
        "Dymola_Modelica_Mechanics_MultiBody_Examples_Loops_Engine1b.fmu"
        "Dymola_Modelica_Mechanics_Rotational_Examples_CoupledClutches.fmu"
        "CATIA_Modelica_Mechanics_Rotational_Examples_CoupledClutches.fmu"
        "MapleSIM_CoupledClutches_Win32_64bits.fmu"
        "MapleSIM_Rectifier1_0_Win32_64bits.fmu"
        "SimulationX_ControlledTemperature.fmu"
        "SimulationX_CoupledClutches.fmu"
        "SimulationX_Engine1b.fmu"
        "SimulationX_Rectifier.fmu"
        "Folder with spaces\SimulationX_ControlledTemperature.fmu"
        "Folder with spaces\SCADE_CruiseControl__CruiseControl_FMU.fmu"
        "Folder with spaces\SCADE (1) CruiseControl.(  CruiseControl_FMU (1)  (2).fmu"
        "Folder with spaces\SCADE.  CruiseControl  CruiseControl_FMU.fmu"];
else
// Linux
    // check version 64/32 bits
    [version, opts] = getversion();
    if opts(2) == "x64"
        modelPath = [...
            "JModelica_CoupledClutches.fmu"
            "JModelica_Modelica_Blocks_Examples_PID_Controller.fmu"
            "SCADE_CruiseControl__CruiseControl_FMU.fmu"
            "MapleSIM_Rectifier1_0_Linux64bits.fmu"
            "MapleSIM_CoupledClutches_Linux64bits.fmu"
            "Folder with spaces/SCADE_CruiseControl__CruiseControl_FMU.fmu"
            "Folder with spaces/SCADE (1) CruiseControl.(  CruiseControl_FMU (1)  (2).fmu"
            "Folder with spaces/SCADE.  CruiseControl  CruiseControl_FMU.fmu"];
    else
        modelPath = [...
            "SCADE_CruiseControl__CruiseControl_FMU.fmu"
            "MapleSIM_Rectifier1_0_Linux32bits.fmu"
            "MapleSIM_CoupledClutches_Linux32bits.fmu"
            "Folder with spaces/SCADE_CruiseControl__CruiseControl_FMU.fmu"
            "Folder with spaces/SCADE (1) CruiseControl.(  CruiseControl_FMU (1)  (2).fmu"
            "Folder with spaces/SCADE.  CruiseControl  CruiseControl_FMU.fmu"];
    end
end

modelPath = fullfile(path, "tests", "unit_tests", modelPath);
for i = modelPath'
    checkUnzip(i);
end

// error tests
// Test the FMUs nonexistent
errmsg = msprintf(_("%s: The file %s doesn''t exist or is not read accessible.\n"), "importFMU", "toto.fmu");
assert_checkerror("importFMU(""toto.fmu"")",errmsg);
errmsg = msprintf(_("%s: The file %s doesn''t exist or is not read accessible.\n"), "importFMU", "toto");
assert_checkerror("importFMU(""toto"")", errmsg);

// Test a not valid extension of a file
errmsg = msprintf(_("%s: Wrong extension '"%s'" of file: %s expected.\n"), "importFMU", ".tst", "fmu");
assert_checkerror("importFMU(fullfile(path, ""tests"", ""unit_tests"", ""CruiseControl__CruiseControl_FMU.tst""))", errmsg);

// Test invalidArchive of FMU
errmsg = msprintf(_("%s: FMU was not unzipped."), "importFMU");
assert_checkerror("importFMU(fullfile(path , ""tests"", ""unit_tests"", ""invalidFMU"", ""invalidArchive.fmu""))",errmsg);

// Test an invalid XML file
errmsg = msprintf(_("%s: The file %s doesn''t exist or is not read accessible.\n"), "importFMU", "modelDescription.xml");
assert_checkerror("importFMU(fullfile(path, ""tests"", ""unit_tests"", ""invalidFMU"", ""InvalidXML.fmu""))", errmsg);

// Test zero binaries
errmsg = msprintf(_("%s: FMU has not available libraries."), "importFMU");
assert_checkerror("importFMU(fullfile(path, ""tests"", ""unit_tests"", ""invalidFMU"", ""InvalidBinarie.fmu""))", errmsg);
