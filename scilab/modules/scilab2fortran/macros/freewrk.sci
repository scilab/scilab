// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function nwrk=freewrk(nwrk,name)
//cette macro libere la place occupee par la variable dont le nom est
//donne dans name
//!
//  write(6,'-----------------'+name);
if part(name,1:7)=='work(iw' then
     ext=part(name,8:length(name)-1)
     if isnum(ext) then
       nb=evstr(ext)+1
       nw2=nwrk(2);
       nw2(2,nb)='0'
//     write(6,'libere :'+nw2(1,nb))
       nwrk(2)=nw2
     end
  elseif part(name,1:9)=='iwork(iiw' then
     ext=part(name,10:length(name)-1)
     if isnum(ext) then
       nb=evstr(ext)+1
       nw5=nwrk(5)
       nw5(2,nb)='0'
       nwrk(5)=nw5
     end
  end
endfunction
