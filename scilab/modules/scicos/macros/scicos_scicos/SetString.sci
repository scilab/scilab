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
function myint8=SetString(str)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

//function to convert scilab string in char
//AL
//Copyright INRIA
 if type(str)==10 then
   myint8=[]
   for i=1:size(str,2)
     if i<>1 then myint8=[myint8,10], end
     for j=1:size(str,1)
       if j<>1 then myint8=[myint8,10], end
       myint8=[myint8,ascii(str(j,i))]
     end
   end
   myint8=int8([myint8,0])
 else
   myint8=[]
 end
endfunction
