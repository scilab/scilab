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
// Compilation needed by the graphical interface for intitialization

function   [ok]=compile_init_modelica(xmlmodel,paremb,jaco)

    if exists("%scicos_solver")==0 then
        %scicos_solver = 0 ;
    end

    if ~exists("%scicos_debug_gr") then
        %scicos_debug_gr = %f; //** debug mode : default is "%f"
    end


    MODELICAC_FILENAME = "modelicac";
    if getos() == "Windows" then
        MODELICAC_FILENAME = MODELICAC_FILENAME + ".exe";
    end

    // called by Initilization IHM
    lines(0);
    global icpr;

    //set paths for generated files
    outpath = pathconvert(TMPDIR,%t,%t);

    // created files for initialization
    xmlfile = outpath + xmlmodel + "_init.xml"; // file created by translator
    namei = xmlmodel + "i";
    Flati = outpath + xmlmodel + "i.mo";
    FlatCi = outpath + xmlmodel + "i.c";    // computational C function associated to the flat model for initialization
    incidencei = outpath + xmlmodel + "i_incidence_matrix.xml";
    Flat_functions = outpath + xmlmodel + "_functions" + ".mo"; // file created by translator

    //--------------------------------------------------------------------
    ok = xml2modelica(xmlfile, Flati)
    if ~ok then return,end

    if jaco == "0" then
        JAC = "";
    else
        JAC = " -jac "; // to generate the analytical jacobian
    end

    tmpdir = pathconvert(TMPDIR, %t, %t);  //for error log and  shell scripts

    exe = """" + pathconvert(getmodelicacpath() + MODELICAC_FILENAME, %f, %t) + """";
    Flati = " """ + Flati + """";

    // generates the computational C function associated to the flat model
    instr = exe + Flati + "" + JAC + " -with-init-in """ + strsubst(xmlfile,"\","/") + ..
    """ -with-init-out """ + strsubst(xmlfile,"\","/") + """ -o """ + ..
    FlatCi + """";


    if getos() == "Windows" then
        mputl(instr, tmpdir + "igenm.bat");
        instr = tmpdir + "igenm.bat";
    end

    [rep,stat,err]=unix_g(instr);
    if stat == 0 then
        mprintf("%s"," Init C code   : "+FlatCi);
        mprintf("\n\r");

        ok = Link_modelica_C(FlatCi);
        [nipar, nrpar, nopar, nz, nx, nx_der, nx_ns, nin, nout, nm, ng, dep_u] = reading_incidence(incidencei);

        if (~ok) then
            return;
        end

    else
        messagebox(err, _("Modelica compiler (flat2C)"), "error", "modal");
        ok = %f;
        return
    end

    //build model data structure of the block equivalent to the implicit
    bllst = bllst;
    nblock = length(bllst);
    mdl = bllst(nblock);
    mdl.sim = list(namei, 10004);
    mdl.state = zeros(nx * 2, 1);
    mdl.dstate = zeros(nz, 1);
    mdl.nzcross = ng;
    mdl.nmode = nm;
    mdl.in = ones(nin, 1);
    mdl.out = ones(nout, 1);
    mdl.dep_ut = [dep_u, %t];
    bllst(nblock) = mdl;
    if size(connectmat, 2) == 6 then
        connectmat = connectmat(:,[1 2 4 5]);
    end
    scs_m = null();

    icpr = list();
    if exists(%scicos_solver) == 0 | (exists(%scicos_solver) <> 0 & %scicos_solver < 100) then
        %scicos_solver = 100;
    end
    icpr = c_pass2(bllst, connectmat, clkconnect, cor, corinv);

    if icpr == list() then
        ok = %f;
        return,
    end

    // suppressing display blocks
    Ignore=["bouncexy", "cscope", "cmscope", "canimxy", "canimxy3d", "cevscpe", "cfscope", "cscopexy", ..
    "cscopexy3d", "cscopxy", "cscopxy3d", "cmatview", "cmat3d", "affich", "affich2", "BPLATFORM"];

    for i = 1:length(icpr.sim.funs)
        if type(icpr.sim.funs(i)) <> 13 then
            if find(icpr.sim.funs(i)(1) == Ignore) <> [] then
                icpr.sim.funs(i)(1) = "trash";
            end
        end
    end
endfunction
//-----------------------------------------------------------------------------

