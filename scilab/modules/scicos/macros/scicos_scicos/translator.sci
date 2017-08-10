//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//
function [ok]=translator(filemo,Mblocks,_Modelica_libs,Flat)
    //Generate the flat model of the Modelica model given in the filemo file
    //and the Modelica libraries. Interface to the external tool
    //translator.
    // if <name> is the basename of filemo this function produces
    // - the flat Modelica model file in outpath+name+'f.mo'
    // - the flat xml representation file in  outpath+name+'f_init.xml'
    // - the flat xml  file in  outpath+name+'f_relations.xml'

    // TO DO : rename filename too generic
    TRANSLATOR_FILENAME = "modelicat";
    if getos() == "Windows" then
        TRANSLATOR_FILENAME = TRANSLATOR_FILENAME + ".exe";
    end

    mlibs = pathconvert(_Modelica_libs,%f,%t);
    filemo = pathconvert(filemo,%f,%t);
    Flat = pathconvert(Flat,%f,%t);

    name = basename(filemo);
    namef = name + "f";

    [modelica_libs,modelica_directory] = getModelicaPath();

    molibs = [];

    for k = 1:size(Mblocks,"r")
        funam = stripblanks(Mblocks(k));
        [dirF, nameF, extF] = fileparts(funam);
        if (extF == ".mo") then
            molibs = [molibs; """" + funam + """"];
        else
            molibs = [molibs; """" + modelica_directory + funam + ".mo" + """"]
        end
    end

    // directories for translator libraries
    for k = 1:size(mlibs,"*")
        modelica_directories = mlibs(k);
        if modelica_directories<> [] then
            molibs = [molibs; """" + modelica_directories + """"];
        end
    end

    translator_libs = strcat(" -lib "+ molibs);

    // build the sequence of -lib arguments for translator
    if getos() == "Windows" then, Limit=1000;else, Limit=3500;end
    if (length(translator_libs)>Limit) then
        // OS limitation may restrict the length of shell command line
        // arguments. If there are too many .mo file we catenate them into a
        // single MYMOPACKAGE.mo file
        msg = _("There are too many Modelica files.\nIt would be better to define several \nModelica programs in a single file.")
        messagebox(msprintf(msg),"warning","modal")
        mymopac = pathconvert(outpath+"MYMOPACKAGE.mo",%f,%t);
        txt = [];
        for k = 1:size(molibs,"*")
            [pathx,fnamex,extensionx] = fileparts(molibs(k));
            if (fnamex <> "MYMOPACKAGE") then
                txt = [txt; mgetl(evstr(molibs(k)))];
            end
        end
        mputl(txt, mymopac);
        translator_libs= " -lib """+mymopac+"""";
    end
    translator_libs = translator_libs + "  -lib """ + filemo + """"

    //Build the shell instruction for calling the translator

    exe = getmodelicacpath() + TRANSLATOR_FILENAME
    exe = """" + pathconvert(getmodelicacpath() + TRANSLATOR_FILENAME,%f,%t) + """ ";

    out =" -o """+Flat+"""" //flat modelica
    Errfile = outpath + "S_translator.err""";

    // with the with-init option
    instr = exe + translator_libs + out + " -with-init -command """ + name + " " + namef + ";"" >""" + Errfile

    if getos() == "Windows" then
        mputl(instr,outpath+"/gent.bat")
        instr = outpath + "/gent.bat";
    end

    if execstr("unix_s(instr)","errcatch") <> 0 then
        messagebox([_("-------Modelica translator error message:-----");
        mgetl(outpath + "S_translator.err")], "error", "modal");
        ok = %f,
    else
        ok = %t
    end
endfunction

