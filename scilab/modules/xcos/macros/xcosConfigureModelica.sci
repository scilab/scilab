//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
// Copyright (C) 2010-2010 - DIGITEO - Clément DAVID
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
//

function xcosConfigureModelica(scs_m)
    // Configure the current diagram with the modelica setting UI.
    //
    // Description
    // On modelica diagrams, variable initialization has to be performed by a
    // specific UI. This achieve on this macro which rely on an existing scs_m
    // diagram variable.
    //

    modelica_libs = getModelicaPath()

    name=scs_m.props.title(1);
    if ~validvar(name) then
        messagebox(msprintf(_("%s is not a valid name, please change the title of the diagram."),name),"error","modal");
        return
    end

    name=stripblanks(name)+"_im";
    path=TMPDIR+"/";
    path=pathconvert(stripblanks(path),%t,%t)


    mofile=path+name+".mo";
    xmlfile_init=path+name+"f_init.xml";
    xmlfile_relations=path+name+"f_relations.xml";

    [info,err1]=fileinfo(xmlfile_init);
    [info,err2]=fileinfo(xmlfile_relations);
    [info,err3]=fileinfo(mofile);

    if (err1==0 & err2==0 & err3 == 0) then,
        if (newest(xmlfile_init,mofile)==2) then compile=%t;end;
    else
        compile=%t;
    end

    compile=%t; // Very conservative

    needcompile=4;

    if compile then
        %Modelica_Init=%t
        // in order to generate *_im.mo -> *_im_f.mo -> *_im.xml
        [bllst,connectmat,clkconnect,cor,corinv,ok]=c_pass1(scs_m);
        %Modelica_Init=%f
    end
    [info,err1]=fileinfo(xmlfile_init);
    [info,err2]=fileinfo(xmlfile_relations);

    if err1==0 & err2==0 then
        xcosConfigureXmlFile(xmlfile_init, xmlfile_relations);
    end

    // variables needed by compile_init_modelica
    [modelica_libs,bllst,connectmat,clkconnect,cor,corinv]=resume(modelica_libs,bllst,connectmat,clkconnect,cor,corinv);

endfunction

