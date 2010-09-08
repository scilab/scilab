// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2009 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [matrix_sorted, origin_indices] = %_gsort(matrix_to_sort, sort_type, sort_direction)

  if ~isdef('matrix_to_sort') then
    matrix_to_sort = [];
    return
  end

  if type(matrix_to_sort) <> 1 then 
    error(999,msprintf(_("%s: Wrong type for input argument #%d: A matrix expected.\n"),'gsort',1));
  end

  if ~isdef('sort_type') then
    sort_type = 'g';
  end

  if ~isdef('sort_direction') then
    sort_direction = 'd';
  end

  if type(sort_type) <> 10 then 
    error(999,msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"),'gsort',2));
  end
  
  if type(sort_direction) <> 10 then 
    error(999,msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"),'gsort',3));
  end
  
  if size(sort_type,'*') <> 1 then
    error(999,msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),'gsort',2));
  end 

  if size(sort_direction,'*') <> 1 then
    error(999,msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),'gsort',3));
  end 

  if ~or(sort_type == ['g' 'r' 'c' 'lc' 'lr']) then
    error(999,msprintf(_("%s: Wrong value for input argument #%d: [''g'' ''r'' ''c'' ''lc'' ''lr''] expected.\n"),'gsort',2));
  end

  if ~or(sort_direction == ['d' 'i']) then
    error(999,msprintf(_("%s: Wrong value for input argument #%d: [''d'' ''i''] expected.\n"),'gsort',3));
  end
  
  magnitude_matrix = abs(matrix_to_sort);
  
  
  matrix_sorted = [];
  origin_indices = [];
  
  [magnitude_sorted, origin_indices] = gsort(magnitude_matrix, sort_type, sort_direction);

  if (sort_type == 'g') then
    dimIn = size(matrix_to_sort);
    matrix_sorted = matrix(matrix_to_sort(origin_indices), dimIn(1), dimIn(2));
  end

  if (sort_type == 'r') then
    nr = size(matrix_to_sort, 'r');
    nc = size(matrix_to_sort, 'c');
    matrix_sorted = [];
    for i = 1:nc ; 
      matrix_sorted = [matrix_sorted, matrix(matrix_to_sort(origin_indices(:,i),i),nr,1)];
    end 
  end

  if (sort_type == 'c') then
    nr = size(matrix_to_sort, 'r');
    nc = size(matrix_to_sort, 'c');
    matrix_sorted = [];
    for i=1:nr ; 
      matrix_sorted = [matrix_sorted; matrix(matrix_to_sort(i,origin_indices(i,:)),1,nc)];
    end 
  end
  
  if (sort_type == 'lr') then
    matrix_sorted = matrix_to_sort(origin_indices, :);
  end
  
  if (sort_type == 'lc') then
    matrix_sorted = matrix_to_sort(:, origin_indices);
  end

endfunction
