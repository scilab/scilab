// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function y = convert_to_float(code, binlen, maxbound, minbound)
Max_Bin = 2^binlen-1;
y       = [];
for i=1:length(maxbound)
  y(i) = (maxbound(i) - minbound(i)) * (bin2dec(part(code,(i-1)*binlen+1:i*binlen)) / Max_Bin) + minbound(i);
end
endfunction
