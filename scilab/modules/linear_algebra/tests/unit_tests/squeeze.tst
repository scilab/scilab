// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge.Steer@inria.fr
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

C={1,%s+1,[3,4],[%f %t],int32(8),-24};
S=[];S.a=1;S(6).a=33;

data=list(1:6,string(1:6),(1:6)+%s,(1:6)>2, int32(1:6),C,S);
//check with hypermatrices and hyper cells
for v=data
  a=matrix(v,[1 2 3]);
  if or(squeeze(a)<>matrix(v,[2 3])) then pause,end

  a=matrix(v,[1 2 1 3]);
  if or(squeeze(a)<>matrix(v,[2 3])) then pause,end
end
//check with matrices and 2D cells the function should return the input
for v=data
  a=matrix(v,[2 3]);
  if or(squeeze(a)<>matrix(v,[2 3])) then pause,end
end

//scalar case
a=33;
if or(size(squeeze(a))<>[1 1]) then pause,end
c={1:3};
if or(size(squeeze(c))<>[1 1]) then pause,end
//empty matrix
if or(size(squeeze([]))<>[0 0]) then pause,end
if or(size(squeeze(cell()))<>[0 0]) then pause,end

