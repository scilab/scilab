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

function fname=choosefile(path,comm,errmess)
// choosefile - select a file through a filter
//%Syntax
//  fname=choosefile(path)
//  fname=choosefile()
//%Parameters
// path   : character string for selection rule.
// fname  :  character string : selected file name or empty matrix if none
//!


[lhs,rhs]=argn(0)
select rhs
case 0 then
  path='./*'
  comm='Choose a file'
  errmess=%t
case 1 then
  comm='Choose a file'
  errmess=%t
case 2 then
  errmess=%t
end
lst=lstfiles(path)
if lst==[]&errmess then message('No such file exists'),end
if prod(size(lst))>0 then
  n=choose(lst,comm,'Cancel')
  if n<>0 then
    fname=lst(n)
  else
    fname=[]
  end
else
  fname=[]
end
endfunction
