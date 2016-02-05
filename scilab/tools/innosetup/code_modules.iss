//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//------------------------------------------------------------------------------
// Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
//
//------------------------------------------------------------------------------
function CreateModulesFile: Boolean;
Var ModuleFileName : String;
i : Integer;
d : Integer;
ModulesXmlFileLines: TArrayOfString;
begin

    i := 0;

    setArrayLength(ModulesXmlFileLines, 256);
    for d := 0 to GetArrayLength(ModulesXmlFileLines)-1 do
      begin
        ModulesXmlFileLines[d] := '';
      end;

    ModuleFileName := ExpandConstant('{app}') +'\etc\modules.xml';

    ModulesXmlFileLines[i] := '<?xml version="1.0" encoding="UTF-8" standalone="no"?>'; i := i + 1;
    ModulesXmlFileLines[i] := '<!DOCTYPE modules SYSTEM "../modules/core/xml/modules.dtd">'; i := i + 1;
    ModulesXmlFileLines[i] := '<!-- =================== -->'; i := i + 1;
    ModulesXmlFileLines[i] := '<!--'; i := i + 1;
    ModulesXmlFileLines[i] := ' Modules list for Scilab'; i := i + 1;
    ModulesXmlFileLines[i] := ' @author Scilab Enterprises'; i := i + 1;
    ModulesXmlFileLines[i] := ' @date 2013'; i := i + 1;
    ModulesXmlFileLines[i] := ' ==================='; i := i + 1;
    ModulesXmlFileLines[i] := ' Do not touch if you do not know what you are doing'; i := i + 1;
    ModulesXmlFileLines[i] := ' Warning "core" module must be always the first module'; i := i + 1;
    ModulesXmlFileLines[i] := ' activate="yes" module will be launched ( or not "no")'; i := i + 1;
    ModulesXmlFileLines[i] := ' -->'; i := i + 1;
    ModulesXmlFileLines[i] := '<!-- =================== -->'; i := i + 1;
    ModulesXmlFileLines[i] := '<modules>'; i := i + 1;

    ModulesXmlFileLines[i] := '<module name="core" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="api_scilab" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="ast" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="differential_equations" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="elementary_functions" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="functions" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="fileio" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="boolean" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="cacsd" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="data_structures" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="console" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="completion" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="history_manager" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="dynamic_link" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="integer" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="io" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="output_stream" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="linear_algebra" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="localization" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="interpolation" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="optimization" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="overloading" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="polynomials" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="signal_processing" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="sparse" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="special_functions" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="string" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="time" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="windows_tools" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="statistics" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="arnoldi" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="compatibility_functions" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="m2sci" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="mexlib" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="sound" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="external_objects" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="randlib" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="demo_tools" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="spreadsheet" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="call_scilab" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="umfpack" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="genetic_algorithms" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="simulated_annealing" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="parameters" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="matio" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="atoms" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="xml" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="tclsci" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="hdf5" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="types" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="fftw" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="slint" activate="yes"/>'; i := i + 1;
    ModulesXmlFileLines[i] := '<module name="coverage" activate="yes"/>'; i := i + 1;

#ifndef SCILAB_F2C
    //ModulesXmlFileLines[i] := '<module name="parallel" activate="yes"/>'; i := i + 1;
#endif

    ModulesXmlFileLines[i] := '<!-- JMV dependencies -->'; i := i + 1;
    if (IsComponentSelected( ExpandConstant('{#COMPN_JVM_MODULE}') )) then
    begin
        ModulesXmlFileLines[i] := '<module name="helptools" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="action_binding" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="preferences" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="jvm" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="commons" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="history_browser" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="graph" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="javasci" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="types" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="external_objects_java" activate="yes"/>'; i := i + 1;
    end;

    ModulesXmlFileLines[i] := '<!-- Scinotes dependencies -->'; i := i + 1;
    if IsComponentSelected( ExpandConstant('{#COMPN_SCINOTES}') ) then
    begin
        ModulesXmlFileLines[i] := '<module name="scinotes" activate="yes"/>'; i := i + 1;
    end;

    ModulesXmlFileLines[i] := '<!-- Graphics dependencies -->'; i := i + 1;
    if IsComponentSelected( ExpandConstant('{#COMPN_GRAPHICS}') ) then
    begin
        ModulesXmlFileLines[i] := '<module name="gui" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="graphics" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="renderer" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="graphic_objects" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="graphic_export" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="ui_data" activate="yes"/>'; i := i + 1;
    end;

    ModulesXmlFileLines[i] := '<!-- Xcos dependencies -->'; i := i + 1;
    if IsComponentSelected( ExpandConstant('{#COMPN_XCOS}') ) then
    begin
        ModulesXmlFileLines[i] := '<module name="xcos" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="scicos" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="scicos_blocks" activate="yes"/>'; i := i + 1;
    end;

    ModulesXmlFileLines[i] := '<!-- Dev tools dependencies -->'; i := i + 1;
    if IsComponentSelected( ExpandConstant('{#COMPN_DEVTOOLS}') ) then
    begin
        ModulesXmlFileLines[i] := '<module name="development_tools" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="modules_manager" activate="yes"/>'; i := i + 1;
    end;

    if IsComponentSelected( ExpandConstant('{#COMPN_MPI}') ) then
    begin
        ModulesXmlFileLines[i] := '<module name="mpi" activate="yes"/>'; i := i + 1;
    end;

    ModulesXmlFileLines[i] := '</modules>'; i := i + 1;

    Result := SaveStringsToFile(ModuleFileName, ModulesXmlFileLines, False);

end;
//------------------------------------------------------------------------------
