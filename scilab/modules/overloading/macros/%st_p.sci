// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function %st_p(s)

  //matlab-like struct display

  f     = getfield(1,s)
  ll    = lines()
  t     = []
  multi = prod(double(s.dims));

  // No fields
  if size(f, "*")==2 then
    t = strcat(string(double(s.dims)), "x") + " struct array with no fields.";
    write(%io(2),t,"(a)");
    return
  end

  // 0x0 struct with fields
  if multi==0 then
    t="0x0 struct array with fields:"
    for k=3:size(f,"*")
      t=[t;"   "+f(k)]
    end
    write(%io(2),t,"(a)")
    return
  end

  // axb struct where a<>0 & b<>0

  if multi>1 then
    t=strcat(string(double(s.dims)),"x") + " struct array with fields:";
  end

  for k=3:size(f,"*")
    var=f(k)
    value=s(var)
    tp=typeof(value)
    if tp=="st" then
      str="[" + strcat(string(size(value)),"x")+" struct" + "]";
    elseif type(value)> 10 then
      str=tp
    else
      sz=size(value)
      // If number of elements in value is greater than ll(1) (current page width)
      // then we do not call sci2exp
      // because the returned value will be ignored at line 64: size(str,"*")==1
      // Note that ll(1)/2 elements could be the max because of colon, semi-colon and brackets added between elements by sci2exp
      if sz(1)==1 & type(value) <> 9 & prod(sz) < ll(1) then // This line can avoid some stacksize error when field contains a big matrix
        str=sci2exp(value,ll(1))
      else
        str="["+strcat(string(size(value)),"x")+" "+tp+"]"
      end
    end
    txt="   "+var
    if multi<=1 then
      if size(str,"*")==1 then
        txt=txt+": "+str
      else
        tp=typeof(value)
        txt=txt+": "+"["+strcat(string(size(value)),"x")+" "+tp+"]"
      end
    end
    t=[t;txt]
  end

  write(%io(2),t,"(a)")

endfunction
