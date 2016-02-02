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

function fplot3d(xr,yr,f,theta,alpha,leg,flag,ebox)
    // fplot3d(xr,yr,f,teta,alpha,leg,[flag,ebox])
    // Trace la surface d\'efinie par un external f ( ex macro [z]=f(x,y))
    // on calcule d'abord f sur la grille definie par xr.yr
    // xr et yr sont des vecteurs implicites donnant les
    // abscisses et les ordonn\'ees des points de la grille
    // -teta, alpha : sont les angles en coordonn\'ees spheriques du
    //      point d'observation
    // -flag et ebox (voir plot3d)
    // Exemple : taper fplot3d() pour voir un exemple
    //!

    [lhs,rhs]=argn(0)
    if rhs <= 0 then   // demo
        deff("[z]=Surf(x,y)","z=sin(x)*cos(y)");
        t=-%pi:0.3:%pi;
        fplot3d(t,t,Surf,35,45,"X@Y@Z");
        return
    end

    if rhs<3 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "fplot3d", 3));
    end;

    opts=[]
    if exists("theta","local")==1 then opts=[opts,"theta=theta"],end
    if exists("alpha","local")==1 then opts=[opts,"alpha=alpha"],end
    if exists("leg"  ,"local")==1 then opts=[opts,"leg=leg"]    ,end
    if exists("flag" ,"local")==1 then opts=[opts,"flag=flag"]  ,end
    if exists("ebox" ,"local")==1 then opts=[opts,"ebox=ebox"]  ,end
    if type(f)==11 then comp(f),end;

    execstr("plot3d(xr,yr,feval(xr,yr,f),"+strcat(opts,",")+")")
endfunction
