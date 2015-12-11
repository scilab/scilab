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

function  ok = modelicac(Flat, Flat_functions, xmlfileTMP, Jacobian, Cfile, with_gui, init)
    //Scilab interface with external tool modelicac

    MODELICAC_FILENAME = "modelicac";
    if getos() == "Windows" then
        MODELICAC_FILENAME = MODELICAC_FILENAME + ".exe";
    end

    if argn(2) < 7 then init = %f,end
    incidence = "";
    tmpdir = pathconvert(TMPDIR, %t, %t);  //for error log and  shell scripts
    xmlfileTMP = pathconvert(xmlfileTMP, %f, %t);
    Cfile = pathconvert(Cfile, %f, %t);

    if fileinfo(Flat_functions) == [] then
        Flat_functions = " ";
    else
        Flat_functions = """" + Flat_functions + """";
    end

    if ( (with_gui) & (fileinfo(xmlfileTMP) <> []) ) then // if GUI is running
        XMLfiles = " -with-init-in """ + strsubst(xmlfileTMP, "\", "/") + """ -with-init-out """ + strsubst(xmlfileTMP,"\","/") + """";
    else
        XMLfiles = "";
    end

    exe = """" + pathconvert(getmodelicacpath() + MODELICAC_FILENAME, %f, %t) + """";

    Flat = """" + Flat + """";
    out = "-o """ + Cfile + """";

    if Jacobian then
        JAC = " -jac ";
    else
        JAC="";
    end

    instr = strcat([exe, Flat, Flat_functions, XMLfiles, out, JAC], " ");

    if getos() == "Windows" then
        if init then
            mputl(instr,tmpdir+"igenm.bat");
            instr = tmpdir + "igenm.bat";
        else
            mputl(instr, tmpdir + "genm2.bat");
            instr = tmpdir + "genm2.bat";
        end
    end

    [rep,stat,err]=unix_g(instr);
    if stat <> 0 then
        messagebox(err, _("Modelica compiler"), "error", "modal");
        ok=%f;
        return
    end

endfunction

