// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


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



    Max=stacksize();Max=int((Max(1)-Max(2))/3)
    if Max<M*N+100 then stacksize(Max+M*N*1.5),end
    execstr("Matplot(double(z),"+strcat(opts,",")+")")

endfunction
