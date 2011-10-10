// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function   y = permute(x,dims)

  // This function returns an array y which results of the x permutation  
  // Input :
  // -x a (multi-dimensionnnal) array of cells, or doubles or strings, ... 
  // -dims a vector which contains the permutation order
  // Output :
  // -y the result of the x permutation  

  // Verify input arguments number
  if argn(2)<>2 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"permute",2));
  end

  // Verify if the size of dims corresponds to dimension of x

  if ndims(dims)<>2 then
    error(msprintf(gettext("%s: Wrong size for argument #%d: Vector expected.\n"),"permute",2));

  elseif or(gsort(dims,"c","i")<>(1:prod(size(dims)))) then
    error(msprintf(gettext("%s: Wrong size for input argument #%d.\n"),"permute",2));

  elseif prod(size(dims))<ndims(x) then
    error(msprintf(gettext("%s: Wrong size for input argument #%d: At least the size of input argument #%d expected.\n"),"permute",2,1));
  end

  // Case x is empty
  if isempty(x) then
    y=x
    return
  end

  // xsize vector contains the size of x
  xsize=size(x)
  // ysize vector contains the new size of x after the permutation 
  ind1=find(dims<=ndims(x))
  ind2=find(dims>ndims(x))
  ysize(ind1)=xsize(dims(ind1))
  ysize(ind2)=1
  dims=dims(ind1)

  // delete the last dimensions of ysize which are equal to 1, ex : [2,3,1,4,1,1,1] -> [2,3,1,4]  
  i=prod(size(ysize))
  while i>2 & ysize(i)==1 & i>max(ind1)
    ysize(i)=[]
    i=i-1
  end

  // index vector contains all indices of x 
  index=[]
  for k=1:size(xsize,"*")
    for j=1:size(x,"*")/prod(xsize(1:k))
      for l=1:xsize(k)
        index=[index,ones(1:prod(xsize(1:k-1)))*l]
      end
    end
  end
  index=matrix(index,size(x,"*"),size(xsize,"*"))

  // prodxsize is a vector, its ith component contains the prod of the first to the (ith-1) entries of xsize, its first component is always equal to one
  prodxsize(1)=1
  for i=2:size(xsize,"*")
    prodxsize($+1)=prod(xsize(1:i-1))
  end
  prodysize(1)=1
  for i=2:size(ysize,"*")
    prodysize($+1)=prod(ysize(1:i-1))
  end

  // newindex contains the indices of x dimensions permutation
  for j=1:size(index,1)
    indexj=index(j,:) 
    newindexj=ones(1:prod(size(ysize)))
    newindexj(ind1)=indexj(dims)
    indexj(2:$)=indexj(2:$)-1
    newindexj(2:$)=newindexj(2:$)-1
    if typeof(x)=="ce" then //case x is a cell array
      y(newindexj*prodysize).entries=x(indexj*prodxsize).entries
    else
      y(newindexj*prodysize)=x(indexj*prodxsize)
    end
  end

  y=matrix(y,ysize)

endfunction
