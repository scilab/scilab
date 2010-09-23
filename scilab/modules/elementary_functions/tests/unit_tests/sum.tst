// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->
//empty matrices
T=list(list(),list('native'),list('double'));
for typ=T
  if sum([],typ(:))<>0 then pause,end
  if sum([],'*',typ(:))<>0 then pause,end
  if sum([],1,typ(:))<>[] then pause,end
  if sum([],2,typ(:))<>[] then pause,end
  if sum([],3,typ(:))<>[] then pause,end
end
//=======================================================================
//float matrices
d=[1 10;254 9];
T=list(list(),list('native'),list('double'));
for typ=T
  if sum(d,typ(:))<>274 then pause,end
  if sum(d,'*',typ(:))<>274 then pause,end
  if or(sum(d,1,typ(:))<>[255 19]) then pause,end
  if or(sum(d,2,typ(:))<>[11;263]) then pause,end
  if or(sum(d,3,typ(:))<>d) then pause,end
end

//hypermatrices of floats
d(1,1,2)=1;
for typ=T
  if sum(d,typ(:))<>275 then pause,end
  if sum(d,'*',typ(:))<>275 then pause,end
  if or(sum(d,1,typ(:))<>hypermat([1,2,2],[255;19;1;0])) then pause,end
  if or(sum(d,2,typ(:))<>hypermat([2,1,2],[11;263;1;0])) then pause,end
  if or(sum(d,3,typ(:))<>[2,10;254,9]) then pause,end
  if or(sum(d,5,typ(:))<>d) then pause,end
end

//=======================================================================
//matrices of short integers
// Caution : uint8([11;263]) is equal to uint8([11;7])
i=uint8([1 10;254 9]);
for typ=list(list(),list('native'));
  if sum(i,typ(:))<>uint8(274) then pause,end
  if sum(i,'*',typ(:))<>uint8(274) then pause,end
  if or(sum(i,1,typ(:))<>uint8([255 19])) then pause,end
  if or(sum(i,2,typ(:))<>uint8([11;263])) then pause,end
  if or(sum(i,3,typ(:))<>i) then pause,end
end

if sum(i,'double')<>274 then pause,end
if sum(i,'*','double')<>274 then pause,end
if or(sum(i,1,'double')<>[255 19]) then pause,end
if or(sum(i,2,'double')<>[11;263]) then pause,end
if or(sum(i,3,'double')<>double(i)) then pause,end

//with hypermatrices
i(1,1,2)=uint8(1);
for typ=list(list(),list('native'));
  if sum(i,typ(:))<>uint8(275) then pause,end
  if sum(i,'*',typ(:))<>uint8(275) then pause,end
  if or(sum(i,1,typ(:))<>hypermat([1,2,2],uint8([255;19;1;0]))) then pause,end
  if or(sum(i,2,typ(:))<> hypermat([2,1,2],uint8([11;263;1;0]))) then pause,end
  if or(sum(i,3,typ(:))<> uint8([2,10;254,9])) then pause,end
  if or(sum(i,5,typ(:))<> i) then pause,end
end

if sum(i,'double')<>275 then pause,end
if sum(i,'*','double')<>275 then pause,end
if or(sum(i,1,'double')<>hypermat([1,2,2],[255;19;1;0])) then pause,end
if or(sum(i,2,'double')<> hypermat([2,1,2],[11;263;1;0])) then pause,end
if or(sum(i,3,'double')<>[2,10;254,9]) then pause,end

//=======================================================================
//Matrices of polynomials
s=%s;
p=[s s+1;s^2 0];
T=list(list(),list('native'),list('double'));
for typ=T
  if sum(p,typ(:))<>1+2*s+s^2 then pause,end
  if sum(p,'*',typ(:))<>1+2*s+s^2 then pause,end
  if or(sum(p,1,typ(:))<>[s+s^2,1+s]) then pause,end
  if or(sum(p,2,typ(:))<>[1+2*s;s^2]) then pause,end
  if or(sum(p,3,typ(:))<>p) then pause,end
end

//with hypermatrices
p(1,1,2)=-1;
for typ=T
  if sum(p,typ(:))<>2*s+s^2 then pause,end
  if sum(p,'*',typ(:))<>2*s+s^2 then pause,end
  if or(sum(p,1,typ(:))<>hypermat([1,2,2],[s+s^2;1+s;-1;0*s])) then pause,end
  if or(sum(p,2,typ(:))<>hypermat([2,1,2],[1+2*s;s^2;-1;0*s])) then pause,end
  if or(sum(p,3,typ(:))<>[-1+s,1+s;s^2,0]) then pause,end
  if or(sum(p,5,typ(:))<>p) then pause,end
end

//=======================================================================
//Matrices of rationals
s=%s;
r=1.0 ./[s s+1;s^2 1];
T=list(list(),list('native'),list('double'));
for typ=T
  if sum(r,typ(:))<>(1+2*s+3*s^2+s^3)./(s^2+s^3) then pause,end
  if sum(r,'*',typ(:))<>(1+2*s+3*s^2+s^3)./(s^2+s^3) then pause,end
  if or(sum(r,1,typ(:))<>[1+s,2+s]./[s^2,1+s]) then pause,end
  if or(sum(r,2,typ(:))<>[1+2*s;1+s^2]./[s+s^2;s^2]) then pause,end
  if or(sum(r,3,typ(:))<>r) then pause,end
end

//=======================================================================
///Matrices of boolean

b=[%t %t %f %t];
for typ=list(list(),list('double'));
  if sum(b,typ(:))<>3 then pause,end
  if sum(b,'*',typ(:))<>3 then pause,end
  if or(sum(b,1,typ(:))<>[1 1 0 1]) then pause,end
  if or(sum(b,2,typ(:))<>3) then pause,end
  if or(sum(b,3,typ(:))<>double(b)) then pause,end
end

if sum(b,'native')<>%t then pause,end
if sum(b,'*','native')<>%t then pause,end
if or(sum(b,1,'native')<>b) then pause,end
if or(sum(b,2,'native')<>%t) then pause,end
if or(sum(b,3,'native')<>b) then pause,end
//with hypermatrices 
b=[%t %t %f %t];b(1,1,2)=%f;
for typ=list(list(),list('double'));
  if sum(b,typ(:))<>3 then pause,end
  if sum(b,'*',typ(:))<>3 then pause,end
  if or(sum(b,1,typ(:))<>hypermat([1,4,2],[1;1;0;1;0;0;0;0])) then pause,end
  if or(sum(b,2,typ(:))<>hypermat([1,1,2],[3;0])) then pause,end
  if or(sum(b,3,typ(:))<>[1 1 0 1]) then pause,end
  if or(sum(b,5,typ(:))<>double(b)) then pause,end
end

if sum(b,'native')<>%t then pause,end
if sum(b,'*','native')<>%t then pause,end
if or(sum(b,1,'native')<>hypermat([1,4,2],[%t;%t;%f;%t;%f;%f;%f;%f])) then pause,end
if or(sum(b,2,'native')<>hypermat([1,1,2],[%t;%f])) then pause,end
if or(sum(b,3,'native')<>[%t %t %f %t]) then pause,end
if or(sum(b,5,'native')<>b) then pause,end

//=======================================================================
//sparse matrices of floats
s=sparse([1 10 0;0 254 9]);
T=list(list(),list('native'),list('double'));
for typ=T
  if sum(s,typ(:))<>274 then pause,end
  if sum(s,'*',typ(:))<>274 then pause,end
  if or(sum(s,1,typ(:))<>sparse([1,264,9])) then pause,end
  if or(sum(s,2,typ(:))<>sparse([11;263])) then pause,end
  if or(sum(s,3,typ(:))<>s) then pause,end
end

//=======================================================================
//sparse  matrices of boolean
bs=sparse([1 10 0;0 254 9])<>0;
for typ=list(list(),list('double'));
  if sum(bs,typ(:))<>4 then pause,end
  if sum(bs,'*',typ(:))<>4 then pause,end
  if or(sum(bs,1,typ(:))<>sparse([1,2,1])) then pause,end
  if or(sum(bs,2,typ(:))<>sparse([2;2])) then pause,end
  if or(sum(bs,3,typ(:))<>bool2s(bs)) then pause,end
end

if sum(bs,'native')<>%t then pause,end
if sum(bs,'*','native')<>%t then pause,end
if or(sum(bs,1,'native')<>sparse([%t,%t,%t])) then pause,end
if or(sum(bs,2,'native')<>sparse([%t;%t])) then pause,end
if or(sum(bs,3,'native')<>bs) then pause,end

// TODO : test the "m" option


