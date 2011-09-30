// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1998-2011 - INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function M=%i_i_i(varargin)
//- insertion of an integer matrix in an matrix of integers 
//            for more than 2 indices
// -insertion an integer matrix in an integer matrix which has a
//            different integer type
  rhs=argn(2)
  M=varargin(rhs)
  it=inttype(M)
  M=mlist(['hm','dims','entries'],int32(size(M)),M(:))
  varargin(rhs)=M;
  
  if it<>inttype(varargin(rhs-1)) then
    //different integer types (should be hard coded)
    varargin(rhs-1)=iconvert(varargin(rhs-1),it)
  end

  M=generic_i_hm(iconvert(0,it),varargin(:))
endfunction
