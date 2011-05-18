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
  M=varargin($)
  it=inttype(M)
  if it<>inttype(varargin($-1)) then
    //different integer types (should be hard coded)
    varargin($-1)=iconvert(varargin($-1),it)
    if size(varargin)<=4 then
      M(varargin(1:$-2))=varargin($-1)
      return
    end
  end
  M=generic_i_hm(iconvert(0,it),varargin(:))

endfunction
