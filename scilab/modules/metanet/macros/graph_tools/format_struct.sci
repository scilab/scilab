
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function w=format_struct(s,fields,fielddisplay)
//utilitary function used for data structure display overloading
// s           : tlist or mlist data structure whose fields are row vectors or
//               matrices with identical number of columns
// fields      : selected fields, if ommitted all fields are used
// fielddisplay: field name alias to use in the display. If ommitted
//               fielddisplay is set to  field
// w           : a column vector of right padded strings
  
  if argn(2)==1 then fields=getfield(1,s);fields=fields(2:$);end
  if argn(2)<3 then fielddisplay=fields,end
  w=''
  for k=1:size(fields,'*') //loop on  fields
    f=fields(k)
    v=string(s(f)')
    if size(v,2)==1 then
      c=v
    else // catenate field values
      c=''
      for j=1:size(v,2)
	l=max(length(v(:,j)))
	c=c+part(v(:,j),1:l+1)
      end
    end
    l=max(length(fielddisplay(k)),max(length(c)))
    c=[part(fielddisplay(k),1:l+1);
       part('-',ones(1,l+1))
       part(c,1:l+1)]
    w=w+'|'+c;
  end 
endfunction
