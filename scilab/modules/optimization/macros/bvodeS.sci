// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Rainer von Seggern
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function z=bvodeS(x,m,n,a,b,fsub,gsub,zeta,...
    ystart,dfsub,dgsub,fixpnt,ndimf,ndimi,ltol,tol,ntol,nonlin,...
    collpnt,subint,iprint,ireg,ifail)
    //
    // PURPOSE
    //   Simplified call of bvode. (Version 08.05.2005)
    //   List of arguments: see help bvode in Scilab.
    //   The arguments from ystart to ifail are optional.
    //   They may be called by name in the form argument=name
    //   in any order.
    //

    ms=sum(m);ipar=zeros(1,11);

    if n>20 then
        error(msprintf(_("%s: More than 20 equations are not allowed"),"bvodeS"));
    end

    [lhs,rhs]=argn()
    if rhs<8 | rhs>23 then
        error(msprintf(gettext("%s: Wrong number of input argument(s).\n"), ...
        "bvodeS"));
    end
    if min(m)<1 | max(m)>4 then
        error(msprintf(gettext("%s: The elements of the argument #%d must be in [%d %d].\n"),"bvodeS",2,1,4));
    end

    if exists("guess","local") then ystart=guess,end
    if ~exists("ystart","local") then
        %ys=%ystart1; iguess=0;
    else
        if and(type(ystart)<>[13 15 11]) then
            error(msprintf(gettext("%s: Wrong type for input argument #%s: external expected.\n"),"bvodeS","guess"));
        end
        %ys=%ystart;iguess=1;
    end

    if ~exists("dfsub","local") then
        dfsubA=%dfsub;
    else
        if and(type(ystart)<>[13 15 11]) then
            error(msprintf(gettext("%s: Wrong type for input argument #%s: external expected.\n"),"bvodeS","dfsub"));
        end
        dfsubA=%dfsub1;
    end
    if ~exists("dgsub","local") then
        dgsubA=%dgsub;
    else
        if and(type(ystart)<>[13 15 11]) then
            error(msprintf(gettext("%s: Wrong type for input argument #%s: external expected.\n"),"bvodeS","dgsub"));
        end
        dgsubA=%dgsub1;
    end
    if ~exists("ltol","local")    then ltol=1:ms;           end
    if ~exists("tol","local")     then tol=1e-4*ones(1,ms); end
    if ~exists("ntol","local")    then ntol=length(tol);    end
    if ~exists("nonlin","local")  then nonlin=1;            end
    if ~exists("collpnt","local") then collpnt=0;           end
    if ~exists("subint","local")  then subint=0;            end
    if ~exists("iprint","local")  then iprint=-1;           end
    if ~exists("ireg","local")    then ireg = 0;            end

    if ~exists("ndimf","local") then
        ndimf=1000*(4+ms+(5+n*collpnt)*(ms+n*collpnt)+4*ms^2);
    end

    if ~exists("ndimi","local") then
        ndimi=1000*(3+ms+n*collpnt);
    end

    if ~exists("fixpnt","local") then
        fixpnt=[];
    else
        ipar(11)=length(fixpnt);
    end

    if ~exists("ifail","local") then
        ifail=0;
    else
        ifail=1;
    end

    ipar(1)=nonlin;
    ipar(2)=collpnt;
    ipar(3)=subint;
    ipar(4)=ntol;
    ipar(5)=ndimf;
    ipar(6)=ndimi;
    ipar(7)=iprint;
    ipar(9)=iguess;
    ipar(10)=ireg;

    if ifail==1 then
        mprintf("%s\n",["ltol   = "+sci2exp(ltol,0)
        "tol    = "+sci2exp(tol,0)
        "fixpnt = "+sci2exp(fixpnt,0)
        "ipar   = "+sci2exp(ipar,0)])
    end

    z=bvode(x,n,m,a,b,zeta,ipar,ltol,tol,fixpnt,%fsub,dfsubA,%gsub,dgsubA,%ys);

endfunction

function y=%R(f_,x,z)
    if type(f_)==15 then
        R_=f_(1); y=R_(x,z,f_(2:$));
    elseif type(f_)==13 then
        y=f_(x,z)
    end
endfunction

function y=%fsub(x,z)
    y=%R(fsub,x,z); sy=size(y);
    if sy(1)==1, y=y', end
endfunction

function RS=%fsubT(z,x)
    RS=%fsub(x,z)
endfunction

function J=%dfsub(x,z)
    sz=size(z);if sz(1)==1, z=z'; end
    J=numderivative(list(%fsubT,x),z)
endfunction

function J=%dfsub1(x,z)
    J=%R(dfsub,x,z)
endfunction

function g=%gsub(i,z)
    g=%R(gsub,i,z)
endfunction

function g=%gsubT(z,i)
    g=%gsub(i,z)
endfunction

function dg=%dgsub(i,z)
    sz=size(z);if sz(1)==1, z=z'; end
    dg=numderivative(list(%gsubT,i),z)
endfunction

function dg=%dgsub1(i,z)
    dg=%R(dgsub,i,z)
endfunction

function [z,LhS]=%RR(f_,x)
    if type(f_)==15 then
        RR_=f_(1); [z,LhS]=RR_(x,f_(2:$));
    elseif type(f_)==13 then
        [z,LhS]=f_(x)
    end
endfunction

function [z,LhS]=%ystart(x)
    [z,LhS]=%RR(ystart,x)
endfunction

function [z,LS]=%ystart1(x),
    z=0; LS=0;
endfunction
