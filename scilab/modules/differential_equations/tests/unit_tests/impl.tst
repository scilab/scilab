// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

y=impl([1;0;0],[-0.04;0.04;0],0,0.4,'resid','aplusp');
if size(y) <> [3 1] then pause,end
if norm( y - [0.9851721;0.0000339;0.0147941]) < %eps then pause,end

//   DAE's
Leps=1.e-6;
// scilab macros
function r=resid(t,y,s)
    r(1)=-.04d0*y(1)+1.d4*y(2)*y(3)-s(1);
    r(2)=.04d0*y(1)-1.d4*y(2)*y(3)-3.d7*y(2)*y(2)-s(2);
    r(3)=y(1)+y(2)+y(3)-1.d0;
endfunction

function p=aplusp(t,y,p)
    p(1,1)=p(1,1)+1.d0;
    p(2,2)=p(2,2)+1.d0;
endfunction

function p=dgbydy(t,y,s)
    p(1,1)=-.04d0;
    p(1,2)=1.d4*y(3);
    p(1,3)=1.d4*y(2);
    p(2,1)=.04d0;
    p(2,2)=-1.d4*y(3)-6.d7*y(2);
    p(2,3)=-1.d4*y(2);
    p(3,1)=1.d0;
    p(3,2)=1.d0;
    p(3,3)=1.d0;
endfunction

//         calling scilab
//  
yt=impl([1;0;0],[-.04;.04;0],0,0.4,resid,aplusp,dgbydy);
//  
r1=yt-impl([1;0;0],[-0.04;0.04;0],0,0.4,resid,aplusp);
if abs(r1) > 1.e-10 then pause,end
//   calling fortran
r2=yt-impl([1;0;0],[-0.04;0.04;0],0,0.4,'resid','aplusp');
if abs(r2) > 1.e-10 then pause,end

