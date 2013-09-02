//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
// Copyright (C) 2010-2010 - DIGITEO - Clément DAVID
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//
//

function xcosConfigureModelica()
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

