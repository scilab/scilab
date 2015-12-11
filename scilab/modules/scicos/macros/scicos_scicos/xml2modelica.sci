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
function  ok=xml2modelica(xmlfile,Flati)
    // Generate the Modelica model file Flati with
    // the xml file given by the initialization interface.
    // Interface to the external tool xml2modelica.exe
    // if <name> is the basename of filemo this function produces
    // - the flat Modelica model file in outpath+name+'fi.mo'

    //Scilab interface  with external tool xml2modelica

    XML2MODELICA_FILENAME = "XML2Modelica";
    if getos() == "Windows" then
        XML2MODELICA_FILENAME = XML2MODELICA_FILENAME + ".exe";
    end

    tmpdir = pathconvert(TMPDIR, %t, %t);  //for error log and  shell scripts
    xmlfile = pathconvert(xmlfile, %f, %t);
    Flati = pathconvert(Flati, %f, %t);

    exe = """" + pathconvert(XML2MODELICA_FILENAME, %f, %t) + """ ";
    in = """" + xmlfile + """";
    out = "-o """ + Flati + """";
    option = "-init";  // generates a flat modelica file for the initialization
    instr = strcat([exe in option out], " ");

    if getos() == "Windows" then,
        mputl(instr, tmpdir + "igenx.bat");
        instr = tmpdir + "igenx.bat";
    end

    [rep,stat,err]=unix_g(instr);
    if stat == 0 then
        mprintf("%s\n", " xml->Modelica : " + Flati);
        ok = %t;
    else
        messagebox(err, _("XML to Modelica"), "error", "modal");
        ok = %f;
        return
    end
endfunction

