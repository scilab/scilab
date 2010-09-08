// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function M=%i_i_i(varargin)
//insertion of an integer matrix in an matrix of integers for more than 2 indices

  M=varargin($)
  it=inttype(M)
  M=mlist(['hm','dims','entries'],int32(size(M)),M(:))
  varargin($)=M;
  M=generic_i_hm(iconvert(0,it),varargin(:))

endfunction
