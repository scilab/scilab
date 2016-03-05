// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [WINID] = GEDeditvar(varargin)
    // Simple Variable Editor
    // This file is part of sciGUI toolbox
    // Copyright (C) 2004 Jaime Urzua Grez
    // mailto:jaime_urzua@yahoo.com
    // rev. 0.2 2004/06/24
    //
    // This program is free software; you can redistribute it and/or modify
    //it under the terms of the GNU General Public License as published by
    //the Free Software Foundation; either version 2 of the License, or
    //(at your option) any later version.

    sciGUI_init()

    [%_nams]=who("get");
    %_loc_type=type(varargin(1))
    if (%_loc_type~=10) then error(42), end
    %_in_list=find(%_nams==varargin(1));
    if (%_in_list==[]) then error(42), end
    execstr("%_loc_var="+varargin(1));
    %_loc_type=type(%_loc_var);
    %_allo=find([1;4;10]==%_loc_type);
    if (%_allo==[]) then error(42), end

    //disp("Please wait...");
    %_loc_nam=varargin(1);
    %_loc_ni=size(%_loc_var,1);
    %_loc_nj=size(%_loc_var,2);
    %_loc_type=string(%_loc_type);

    TCL_EvalStr("set EdVarLoc [GEDsciGUIEditVar -1]");

    %_winId=TCL_GetVar("EdVarLoc");

    TCL_SetVar("sciGUITable(win,"+%_winId+",data,name)",%_loc_nam);
    TCL_SetVar("sciGUITable(win,"+%_winId+",data,type)",string(%_loc_type));
    TCL_SetVar("sciGUITable(win,"+%_winId+",data,ni)",string(%_loc_ni));
    TCL_SetVar("sciGUITable(win,"+%_winId+",data,nj)",string(%_loc_nj));

    //
    // This is a slow manner to move the data ...
    // Anyone have a faster method than TCL_SetVar
    // I would like set some tcl variable like varname(pos_i,pos_j)
    //
    Nb_data=(%_loc_nj)*(%_loc_ni)
    // disp("Nb_data=")
    // disp(Nb_data);
    // winWB=waitbar('Loading data...');
    // tmp = 0;
    // waitbar(tmp,winWB);


    for %_j=1:%_loc_nj,
        for %_i=1:%_loc_ni,
            %_value=string(%_loc_var(%_i,%_j));
            %_varname="sciGUITable(win,"+%_winId+",data,"+string(%_i)+","+string(%_j)+")";
            TCL_SetVar(%_varname,%_value);
        end

        //tmp = ((%_j)*(%_loc_ni)) / Nb_data;
        //tmp = tmp *100; tmp = int(tmp); tmp = tmp /100;
        //disp("tmp dans for=")
        //disp(tmp)
        //waitbar(tmp,winWB);
    end


    // disp("%_winId=");
    // disp(%_winId);
    // disp("type(%_winId)=");
    // disp(type(%_winId));
    // disp("winWB=");
    // disp(winWB);
    // disp("type(winWB)=");
    // disp(type(winWB));
    // disp("AVANT DrawGrid");


    TCL_EvalStr("GEDsciGUIEditVarDrawGrid "+%_winId)

    WINID = %_winId;

endfunction
