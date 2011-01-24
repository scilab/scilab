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

function [Path,path,ok]=whereintree(Tree,xc,yc)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

[m,k]=min((Tree('x')-xc).^2+(Tree('y')-yc).^2)
ok=%t;path=[];Path=list()
if k==1 then return,end

O=Tree('orig')
path=Tree('paths')(k-1);
if size(O,'*')>size(super_path,'*') then
  messagebox(['Navigator tree doesnt correspond to this'
      'diagram'],'modal')
  ok=%f
  return
end
n=size(O,'*')
if or(O<>super_path(1:n)) then
  messagebox(['Navigator tree doesnt correspond to this'
      'diagram'],'modal')
  ok=%f
  return
end

sp=super_path;sp(1:n)=[]
if size(path,'*')<size(sp,'*') then
  messagebox(['Upper level or New branch';
      'close current edition window first'],'modal')
  ok=%f
  return
end
n=size(sp,'*')
if or(sp<>path(1:n)) then
  messagebox(['New branch, close current edition window first'],'modal')
  ok=%f
  return
end
path(1:n)=[]



Path=list();
for pk=path
  Path=lstcat(Path,'objs',pk,'model','rpar');
end
endfunction
