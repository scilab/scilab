//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Yann COLLETTE
//
// This file is distributed under the same license as the Scilab package.
//

// icse.init.bas : initialization and tests for icse
//**************************************************
//

// creation of y0 (initial state)
if exists("y0")==0 then
    y0 = ones(1,ny);
end
// creation of b,fy and fu and transformation to line vectors
if exists("b")==0 then
    b = ones(1,ny);
end
if exists("fy")==0 then
    fy1 = ones(1,ny*ny);
else
    fy1 = matrix(fy,1,ny*ny);
end
if exists("fu")==0 then
    fu1 = ones(1,ny*(nuc+nuv));
else
    fu1 = matrix(fu,1,ny*(nuc+nuv));
end

format("e");
iu(5) = 0;
[xx,nitu] = size(itu);
[yy,ndtu] = size(dtu);

if xx+yy>2 then
    error("itu and / or dtu is not a line vector");
end
u = [];
if nuc>0 then u = uc; end;
if nuv>0 then u = [u,uv]; end;
nu = nuc+nuv*(nti+ntf+1);

if size(u)<>[1,nu] then
    error("dimensions of the control are incompatible");
end
clear xx yy;

//    initialisation du common icsez
[nitv,nrtv,ndtv]=fort("icse0",nu,1,"i",t0,2,"d",tf,3,"d",dti,4,"d",..
dtf,5,"d",ermx,6,"d",iu,7,"i",nuc,8,"i",nuv,9,"i",ilin,10,"i",nti,..
11,"i",ntf,12,"i",ny,13,"i",nea,14,"i",itmx,15,"i",nex,16,"i",nob,..
17,"i",ntob,18,"i",ntobi,19,"i",nitu,20,"i",ndtu,21,"i","sort",..
[1,1],22,"i",[1,1],23,"i",[1,1],24,"i");

