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

        ModulesXmlFileLines[i] := '<module name="core" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="types" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="double" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="elementary_functions" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="boolean" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="integer" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="io" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="functions" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="output_stream" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="matio" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="fileio" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="development_tools" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="gui" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="time" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="overloading" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="string" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="scinotes" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="data_structures" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="localization" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="windows_tools" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="jvm" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="helptools" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="hdf5" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="dynamic_link" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="action_binding" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="mexlib" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="history_manager" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="console" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="modules_manager" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="signal_processing" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="linear_algebra" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="statistics" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="javasci" activate="yes"/>'; i := i + 1;
		ModulesXmlFileLines[i] := '<module name="operations" activate="yes"/>'; i := i + 1;

        ModulesXmlFileLines[i] := '</modules>'; i := i + 1;

    Result := SaveStringsToFile(ModuleFileName, ModulesXmlFileLines, False);

    end;
//------------------------------------------------------------------------------
