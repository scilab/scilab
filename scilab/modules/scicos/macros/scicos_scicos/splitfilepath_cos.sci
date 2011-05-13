//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function [path,name,ext]=splitfilepath_cos(fname)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

l=length(fname)
//getting the extension part
n=l
while n>0
  cn=part(fname,n)
  if cn=='.'|cn=='/'|cn=='\' then break,end
  n=n-1
end
if n==0 then
  ext=emptystr()
  name=fname
  path='./'
  return
elseif cn=='/'|cn=='\' then
  ext=emptystr()
  n=l
else
  ext=part(fname,n+1:l)
  n=n-1
end
//getting the name part
l=n
n=l
while n>0
  cn=part(fname,n)
  if cn=='/'|cn=='\' then break,end
  n=n-1
end
if n==0 then
  name=part(fname,1:l)
  path='./'
else
  name=part(fname,n+1:l)
  path=part(fname,1:n)
end
endfunction
