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
// === LICENSE_END ===

function fcontour2d(xr,yr,f,nz,style,strf,leg,rect,nax,void)

    // deff('[z]=Surf(x,y)','z=x**2+y**2');
    // fcontour(Surf,-1:0.1:1,-1:0.1:1,10);


    [lhs,rhs]=argn(0);

    if rhs == 0 then   // demo
        deff("[z]=Surf(x,y)","z=x**3+y");
        fcontour2d(-1:0.1:1,-1:0.1:1,Surf,10,style=1:10,rect=[-1,-1,1,1]*1.5,strf="011");
        return
    end

    if rhs<3,
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "fcontour2d", 3));
    end

    opts=[]
    if exists("nz","local")==0 then nz=10,rhs=rhs+1,end
    if exists("style","local")==1 then opts=[opts,"style=style"],end
    if exists("strf","local")==1 then opts=[opts,"strf=strf"],end
    if exists("leg","local")==1 then opts=[opts,"leg=leg"],end
    if exists("rect","local")==1 then opts=[opts,"rect=rect"],end
    if exists("nax","local")==1 then opts=[opts,"nax=nax"],end
    if exists("frameflag","local")==1 then opts=[opts,"frameflag=frameflag"],end
    if exists("axesflag","local")==1 then opts=[opts,"axesflag=axesflag"],end

    if type(f)==11 then comp(f),end;
    execstr("contour2d(xr,yr,feval(xr,yr,f),nz,"+strcat(opts,",")+")")
endfunction
