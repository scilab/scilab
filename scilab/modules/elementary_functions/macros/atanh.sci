// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [t] = atanh(z)
  //
  //  PURPOSE
  //     element wise hyperbolic arctangent
  //
  //  METHOD
  //     based on the formula  atanh(z) = i atan(-i z)
  //

  rhs = argn(2);

  if rhs <> 1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"atanh",1));
  end

  if type(z)<>1 then
   error(msprintf(gettext("%s: Wrong type for input argument #%d: Real or complex matrix expected.\n"),"atanh",1));
  end

  if isreal(z) then 
    if max(abs(z)) > 1 then  // il faut faire le test ds ce sens pour
                             // les Nan (sinon on obtient NaN + i NaN ! )
      // result is complex
      t = imult(atan(-imult(z)));
    else
      // result is real
      t= -imag(atan(-imult(z)));
    end
  else
    t = imult(atan(-imult(z)));
  end

endfunction
