
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [pop_out] = codage_binary(pop_in,direction,param)
// pop_in is a list().
// pop_out must be a list().
if ~isdef('param','local') then
  param = [];
end

// We deal with some parameters to take into account the boundary of the domain and the neighborhood size
if is_param(param,'binary_length') then
  BinLen = get_param(param,'binary_length');
else
  BinLen = 8;
end
if is_param(param,'minbound') then
  MinBounds = get_param(param,'minbound');
else
  MinBounds = -2*ones(size(pop_in(1),1),size(pop_in(1),2));
end
if is_param(param,'maxbound') then
  MaxBounds = get_param(param,'maxbound');
else
  MaxBounds = 2*ones(size(pop_in(1),1),size(pop_in(1),2));
end

pop_out   = list();
Dimension = length(pop_in(1));
Max_Bin   = 2^BinLen-1;
// A template of '0' so as to be sure that the binary code will be 8 bits long
template  = strsubst(dec2bin(Max_Bin,BinLen),'1','0');

if (direction=='code') then
  for i=1:length(pop_in)
    str_tmp = '';
    for j=1:Dimension
      tmp = dec2bin(Max_Bin * (pop_in(i)(j) - MinBounds(j)) / (MaxBounds(j) - MinBounds(j)),BinLen);
      tmp = strcat([part(template,1:BinLen-length(tmp)) tmp]);
      str_tmp = strcat([str_tmp, tmp]);
    end
    pop_out(i) = str_tmp;
  end
elseif (direction=='decode') then
  for i=1:length(pop_in)
    pop_out(i) = zeros(size(MaxBounds,1),size(MaxBounds,2));
    for j=1:length(MaxBounds)
      pop_out(i)(j) = (MaxBounds(j) - MinBounds(j)) * (bin2dec(part(pop_in(i),(j-1)*BinLen+1:j*BinLen)) / Max_Bin) + MinBounds(j);
    end
  end
else
  error('codage_binary: wrong direction');
end
endfunction
