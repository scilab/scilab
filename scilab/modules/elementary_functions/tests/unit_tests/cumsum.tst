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
  if cumsum([],typ(:))<>[] then pause,end
  if cumsum([],'*',typ(:))<>[] then pause,end
  if cumsum([],1,typ(:))<>[] then pause,end
  if cumsum([],2,typ(:))<>[] then pause,end
  if cumsum([],3,typ(:))<>[] then pause,end
end

//=======================================================================
//float matrices
d=[1 10;254 9];
T=list(list(),list('native'),list('double'));
for typ=T
  if or(cumsum(d,typ(:))<>[1,265;255,274]) then pause,end
  if or(cumsum(d,'*',typ(:))<>[1,265;255,274]) then pause,end
  if or(cumsum(d,1,typ(:))<>[1,10;255,19]) then pause,end
  if or(cumsum(d,2,typ(:))<>[1,11;254,263]) then pause,end
  if or(cumsum(d,3,typ(:))<>d) then pause,end
end

//hypermatrices of floats
d=[1 10;254 9];d(1,1,2)=1;
T=list(list(),list('native'),list('double'));
for typ=T
  if or(cumsum(d,typ(:))<>hypermat([2,2,2],[1;255;265;274;275;275;275;275])) then pause,end
  if or(cumsum(d,'*',typ(:))<>hypermat([2,2,2],[1;255;265;274;275;275;275;275])) then pause,end
  if or(cumsum(d,1,typ(:))<>hypermat([2,2,2],[1;255;10;19;1;1;0;0])) then pause,end
  if or(cumsum(d,2,typ(:))<>hypermat([2,2,2],[1;254;11;263;1;0;1;0])) then pause,end
  if or(cumsum(d,3,typ(:))<>hypermat([2,2,2],[1;254;10;9;2;254;10;9])) then pause,end
  if or(cumsum(d,5,typ(:))<>d) then pause,end
end

//=======================================================================
//Integers
i=uint8([1 10;254 9]);
for typ=list(list(),list('native'));
  if or(cumsum(i,typ(:))<>uint8([1,9;255,18])) then pause,end
  if or(cumsum(i,'*',typ(:))<>uint8([1,9;255,18])) then pause,end
  if or(cumsum(i,1,typ(:))<>uint8([1,10;255,19])) then pause,end
  if or(cumsum(i,2,typ(:))<>uint8([1,11;254,7])) then pause,end
  if or(cumsum(i,3,typ(:))<>i) then pause,end
end

if or(cumsum(i,'double')<>[1,265;255,274]) then pause,end
if or(cumsum(i,'*','double')<>[1,265;255,274]) then pause,end
if or(cumsum(i,1,'double')<>[1,10;255,19]) then pause,end
if or(cumsum(i,2,'double')<>[1,11;254,263]) then pause,end
if or(cumsum(i,3,'double')<>double(i)) then pause,end

//with hypermatrices
i=uint8([1 10;254 9]);i(1,1,2)=uint8(1);
for typ=list(list(),list('native'));
  if or(cumsum(i,typ(:))<>hypermat([2,2,2],uint8([1;255;9;18;19;19;19;19]))) then pause,end
  if or(cumsum(i,'*',typ(:))<>hypermat([2,2,2],uint8([1;255;9;18;19;19;19;19]))) then pause,end
  if or(cumsum(i,1,typ(:))<>hypermat([2,2,2],uint8([1;255;10;19;1;1;0;0]))) then pause,end
  if or(cumsum(i,2,typ(:))<>hypermat([2,2,2],uint8([1;254;11;7;1;0;1;0]))) then pause,end
  if or(cumsum(i,3,typ(:))<>hypermat([2,2,2],uint8([1;254;10;9;2;254;10;9]))) then pause,end
  if or(cumsum(i,5,typ(:))<>i) then pause,end
end


if or(cumsum(i,'double')<>hypermat([2,2,2],[1;255;265;274;275;275;275;275])) then pause,end
if or(cumsum(i,'*','double')<>hypermat([2,2,2],[1;255;265;274;275;275;275;275])) then pause,end
if or(cumsum(i,1,'double')<>hypermat([2,2,2],[1;255;10;19;1;1;0;0])) then pause,end
if or(cumsum(i,2,'double')<>hypermat([2,2,2],[1;254;11;263;1;0;1;0])) then pause,end
if or(cumsum(i,3,'double')<>hypermat([2,2,2],[1;254;10;9;2;254;10;9])) then pause,end
if or(cumsum(i,5,'double')<>double(i)) then pause,end

//=======================================================================
//Matrices of Polynomials
s=%s;p=[s s+1;s^2 0];
T=list(list(),list('native'),list('double'));
for typ=T
  if or(cumsum(p,typ(:))<>[s,1+2*s+s^2;s+s^2,1+2*s+s^2]) then pause,end
  if or(cumsum(p,'*',typ(:))<>[s,1+2*s+s^2;s+s^2,1+2*s+s^2]) then pause,end
  if or(cumsum(p,1,typ(:))<>[s,1+s;s+s^2,1+s]) then pause,end
  if or(cumsum(p,2,typ(:))<>[s,1+2*s;s^2,s^2]) then pause,end
  if or(cumsum(p,3,typ(:))<>p) then pause,end
end

//with hypermatrices
s=%s;p=[s s+1;s^2 0];p(1,1,2)=-1;
T=list(list(),list('native'),list('double'));
for typ=T
  if or(cumsum(p,typ(:))<>hypermat([2,2,2],[s;s+s^2;1+2*s+s^2;1+2*s+s^2;2*s+s^2;2*s+s^2;2*s+s^2;2*s+s^2])) then pause,end
  if or(cumsum(p,'*',typ(:))<>hypermat([2,2,2],[s;s+s^2;1+2*s+s^2;1+2*s+s^2;2*s+s^2;2*s+s^2;2*s+s^2;2*s+s^2])) then pause,end
  if or(cumsum(p,1,typ(:))<>hypermat([2,2,2],[s;s+s^2;1+s;1+s;-1;-1;0*s;0*s])) then pause,end
  if or(cumsum(p,2,typ(:))<>hypermat([2,2,2],[s;s^2;1+2*s;s^2;-1;0*s;-1;0*s])) then pause,end
  if or(cumsum(p,3,typ(:))<>hypermat([2,2,2],[s;s^2;1+s;0*s;-1+s;s^2;1+s;0*s])) then pause,end
  if or(cumsum(p,5,typ(:))<>p) then pause,end
end

//=======================================================================
///Matrices of boolean

b=[%t %t;%f %t];
for typ=list(list(),list('double'));
  if or(cumsum(b,typ(:))<>[1,2;1,3]) then pause,end
  if or(cumsum(b,'*',typ(:))<>[1,2;1,3]) then pause,end
  if or(cumsum(b,1,typ(:))<>[1,1;1,2]) then pause,end
  if or(cumsum(b,2,typ(:))<>[1,2;0,1]) then pause,end
  if or(cumsum(b,3,typ(:))<>double(b)) then pause,end
end
if or(cumsum(b,'native')<>[%t %t;%t %t]) then pause,end
if or(cumsum(b,'*','native')<>[%t %t;%t %t]) then pause,end
if or(cumsum(b,1,'native')<>[%t %t;%t %t]) then pause,end
if or(cumsum(b,2,'native')<>[%t %t;%f %t]) then pause,end
if or(cumsum(b,3,'native')<>b) then pause,end

//with hypermatrices
b=[%t %t;%f %t];b(1,1,2)=%f;
for typ=list(list(),list('double'));
  if or(cumsum(b,typ(:))<>hypermat([2,2,2],[1;1;2;3;3;3;3;3])) then pause,end
  if or(cumsum(b,'*',typ(:))<>hypermat([2,2,2],[1;1;2;3;3;3;3;3])) then pause,end
  if or(cumsum(b,1,typ(:))<>hypermat([2,2,2],[1;1;1;2;0;0;0;0])) then pause,end
  if or(cumsum(b,2,typ(:))<>hypermat([2,2,2],[1;0;2;1;0;0;0;0])) then pause,end
  if or(cumsum(b,3,typ(:))<>hypermat([2,2,2],[1;0;1;1;1;0;1;1])) then pause,end
  if or(cumsum(b,5,typ(:))<>double(b)) then pause,end
end

if or(cumsum(b,'native')<>hypermat([2,2,2],[%t;%t;%t;%t;%t;%t;%t;%t])) then pause,end
if or(cumsum(b,'*','native')<>hypermat([2,2,2],[%t;%t;%t;%t;%t;%t;%t;%t])) then pause,end
if or(cumsum(b,1,'native')<>hypermat([2,2,2],[%t;%t;%t;%t;%f;%f;%f;%f])) then pause,end
if or(cumsum(b,2,'native')<>hypermat([2,2,2],[%t;%f;%t;%t;%f;%f;%f;%f])) then pause,end
if or(cumsum(b,3,'native')<>hypermat([2,2,2],[%t;%f;%t;%t;%t;%f;%t;%t])) then pause,end
if or(cumsum(b,5,'native')<>b) then pause,end

//=======================================================================
//sparse matrices of floats
s=sparse([0 10 0;0 254 9]);
T=list(list(),list('native'),list('double'));
for typ=T
  if or(cumsum(s,typ(:))<>sparse([1,2;1,3;2,2;2,3],[10;264;264;273],[2,3])) then pause,end
  if or(cumsum(s,'*',typ(:))<>sparse([1,2;1,3;2,2;2,3],[10;264;264;273],[2,3])) then pause,end
  if or(cumsum(s,1,typ(:))<>sparse([1,2;2,2;2,3],[10;264;9],[2,3])) then pause,end
  if or(cumsum(s,2,typ(:))<>sparse([1,2;1,3;2,2;2,3],[10;10;254;263],[2,3])) then pause,end
  if or(cumsum(s,3,typ(:))<>s) then pause,end
end

//=======================================================================
//sparse  matrices of boolean
bs=sparse([0 10 0;0 254 9])<>0;
for typ=list(list(),list('double'));
  if or(cumsum(bs,typ(:))<>sparse([1,2;1,3;2,2;2,3],[1;2;2;3],[2,3])) then pause,end
  if or(cumsum(bs,'*',typ(:))<>sparse([1,2;1,3;2,2;2,3],[1;2;2;3],[2,3])) then pause,end
  if or(cumsum(bs,1,typ(:))<>sparse([1,2;2,2;2,3],[1;2;1],[2,3])) then pause,end
  if or(cumsum(bs,2,typ(:))<>sparse([1,2;1,3;2,2;2,3],[1;1;1;2],[2,3])) then pause,end
  if or(cumsum(bs,3,typ(:))<>bool2s(bs)) then pause,end
end

if or(cumsum(bs,'native')<> sparse([1,2;1,3;2,2;2,3],[%t;%t;%t;%t],[2,3])) then pause,end
if or(cumsum(bs,'*','native')<> sparse([1,2;1,3;2,2;2,3],[%t;%t;%t;%t],[2,3])) then pause,end
if or(cumsum(bs,1,'native')<>sparse([1,2;2,2;2,3],[%t;%t;%t],[2,3])) then pause,end
if or(cumsum(bs,2,'native')<>sparse([1,2;1,3;2,2;2,3],[%t;%t;%t;%t],[2,3])) then pause,end
if or(cumsum(bs,3,'native')<>bs) then pause,end

// TODO : test the "m" option
