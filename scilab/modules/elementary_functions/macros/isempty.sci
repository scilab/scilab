// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1998 - INRIA
// Copyright (C) DIGITEO - 2011-2012 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function answ = isempty(m)

  rhs = argn(2);
  if rhs <> 1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "isempty", 1));
  end

  m_type = type(m);

  if( (type(m) >= 11) & (type(m) <= 13) | (type(m) >= 128) ) then
    error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"), "isempty", 1));
  end

  select m_type
  case 10
    // matrix of character string
    answ = ( max(length(m)) == 0 );

  case 15
    // list
    answ = %t;
    for i=1:size(m),
      clear __element__;
      __element__ = m(i);
      if isdef('__element__') then
        r = isempty(m(i));
      else
        r = %F;
      end
      answ = answ & r;
    end;

  case 16
    // typed list
    answ = %t;
    for i=1:size(m),
      clear __element__;
      __element__ = m(i);
      if isdef('__element__') then
        r = isempty(m(i));
      else
        r = %F;
      end
      answ = answ & r;
    end;

  case 17
    // mlist
    answ = %f;

  else
    answ = size(m,'*')==0;
  end

endfunction
