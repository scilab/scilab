// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function %i_Matplot(z,strf,rect,nax,frameflag,axesflag)

    [N,M]=size(z)
    opts=[];
    isframeflag=%f
    isaxesflag=%f
    narg=0
    if exists("strf","local")==1 then
        opts=[opts,"frameflag="+part(strf,2),"axesflag="+part(strf,3)],
        isframeflag=%t
        isaxesflag=%t
        narg=narg+1
    end
    if exists("rect","local")==1 then
        opts=[opts,"rect=rect"],
    end
    if exists("nax" ,"local")==1 then
        opts=[opts,"nax=nax"]
    end
    if exists("frameflag" ,"local")==1 then
        if isframeflag then
            error(msprintf(gettext("%s: ''%s'' already defined by the ''%s'' argument."),"%%i_Matplot","frameflag","strf"))
        end
        opts=[opts,"frameflag=frameflag"]  ,
    end
    if exists("axesflag" ,"local")==1 then
        if isaxesflag then
            error(msprintf(gettext("%s: ''%s'' already defined by the ''%s'' argument."),"%%i_Matplot","axesflag","strf"))
        end
        opts=[opts,"axesflag=axesflag"]  ,
    end
    if size(opts,2)+1-narg <argn(2) then
        error(msprintf(gettext("%s: Wrong value for input argument: ''%s'', ''%s'', ''%s'', ''%s'' or ''%s'' expected.\n"),"%%i_Matplot","strf","rect","nax","frameflag","axesflag"));
    end

    execstr("Matplot(double(z),"+strcat(opts,",")+")")

endfunction
