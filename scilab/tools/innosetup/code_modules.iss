//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
        ModulesXmlFileLines[i] := ' @author Allan CORNET'; i := i + 1;
        ModulesXmlFileLines[i] := ' @date DIGITEO 2010'; i := i + 1;
        ModulesXmlFileLines[i] := ' ==================='; i := i + 1;
        ModulesXmlFileLines[i] := ' Do not touch if you do not know what you are doing'; i := i + 1;
        ModulesXmlFileLines[i] := ' Warning "core" module must be always the first module'; i := i + 1;
        ModulesXmlFileLines[i] := ' activate="yes" module will be launched ( or not "no")'; i := i + 1;
        ModulesXmlFileLines[i] := ' -->'; i := i + 1;
        ModulesXmlFileLines[i] := '<!-- =================== -->'; i := i + 1;
        ModulesXmlFileLines[i] := '<modules>'; i := i + 1;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="core" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="api_scilab" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_JVM_MODULE}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="helptools" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="differential_equations" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="double" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="elementary_functions" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="functions" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="fileio" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_GRAPHICS}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="graphics" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="renderer" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="graphic_objects" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="graphic_export" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="boolean" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="cacsd" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="data_structures" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="console" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_JVM_MODULE}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="action_binding" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="completion" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="history_manager" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_JVM_MODULE}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="gui" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="preferences" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="dynamic_link" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="integer" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="io" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="output_stream" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_JVM_MODULE}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="jvm" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="commons" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="linear_algebra" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="localization" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_JVM_MODULE}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="graph" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="ui_data" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="history_browser" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_INTERSCI}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="intersci" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_INTERP}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="interpolation" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_OPTIM}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="optimization" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="overloading" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_POLY}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="polynomials" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_SIGPROC}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="signal_processing" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_SPARSE}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="sparse" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="special_functions" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="string" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_SYMB}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="symbolic" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="time" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="windows_tools" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_STATS}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="statistics" activate="yes"/>'; i := i + 1;
            end;

        ModulesXmlFileLines[i] := '<!-- comments -->'; i := i + 1;

        if IsComponentSelected( ExpandConstant('{#COMPN_ARNOLDI}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="arnoldi" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_COMPATFUNCTIONS}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="compatibility_functions" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') ) and IsComponentSelected( ExpandConstant('{#COMPN_JVM_MODULE}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="javasci" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_M2SCI}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="m2sci" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_MEXLIB}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="mexlib" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_TCLSCI}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="tclsci" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_SOUND}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="sound" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_EXTERNAL_OBJECTS}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="external_objects" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_RANDLIB}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="randlib" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_DEVTOOLS}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="development_tools" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="demo_tools" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_SPREADSHEET}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="spreadsheet" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="call_scilab" activate="yes"/>'; i := i + 1;
            end;

        ModulesXmlFileLines[i] := '<!-- comments -->'; i := i + 1;

        if IsComponentSelected( ExpandConstant('{#COMPN_XCOS}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="xcos" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="scicos" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="scicos_blocks" activate="yes"/>'; i := i + 1;
            end;

        ModulesXmlFileLines[i] := '<!-- comments -->'; i := i + 1;

        if IsComponentSelected( ExpandConstant('{#COMPN_FFTW}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="fftw" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_UMFPACK}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="umfpack" activate="yes"/>'; i := i + 1;
            end;

        ModulesXmlFileLines[i] := '<!-- comments -->'; i := i + 1;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="genetic_algorithms" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="simulated_annealing" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="parameters" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_MATIO}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="matio" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_ATOMS}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="atoms" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_MODULES_MANAGER}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="modules_manager" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_JVM_MODULE}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="types" activate="yes"/>'; i := i + 1;
            end;

        ModulesXmlFileLines[i] := '<module name="hdf5" activate="yes"/>'; i := i + 1;

        if IsComponentSelected( ExpandConstant('{#COMPN_SCINOTES}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="scinotes" activate="yes"/>'; i := i + 1;
            end;
            
#ifndef SCILAB_F2C
        if IsComponentSelected( ExpandConstant('{#COMPN_PARALLEL}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="parallel" activate="yes"/>'; i := i + 1;
            end;
#endif
        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="xml" activate="yes"/>'; i := i + 1;
            end;


        ModulesXmlFileLines[i] := '</modules>'; i := i + 1;

    Result := SaveStringsToFile(ModuleFileName, ModulesXmlFileLines, False);

    end;
//------------------------------------------------------------------------------
