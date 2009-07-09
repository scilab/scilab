// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2009 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [matrix_sorted, origin_indices] = %_sort(matrix_to_sort, sort_type)

  if ~isdef('matrix_to_sort') then
    matrix_to_sort = [];
    return
  end

  if ~isdef('sort_type') then
    sort_type = 'g';
  end
  
  if ~or(sort_type == ['g' 'r' 'c']) then
    error(999,msprintf(_("%s: Wrong value for input argument #%d: [''g'' ''r'' ''c''] expected.\n"),'gsort',2));
  end

  // sort default direction is 'd'
  sort_direction = 'd';

  [matrix_sorted, origin_indices] = %_gsort(matrix_to_sort, sort_type, sort_direction);

endfunction 
 