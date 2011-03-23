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

function ok = set_cmap(cmap)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

// appends new colors to the colormap

  if cmap==[] then
    ok = %t ; //** 
    return  ; //** EXIT point 
  end

  gh_curwin = gcf(); 

  //** d = xget('colormap');  
  d = gh_curwin.color_map; //** recover the current color map 
  
  //** Not easy to understand code :( here 
  //** Probably avoid to add already defined colors
  for k=1:size(cmap,1)
    [mc,kk] = min(abs(d-ones(size(d,1),1)*cmap(k,:))*[1;1;1]); 
    if mc>.0001 then
      d=[d;cmap(k,:)]; 
    end
  end

  //** 
  ierr = execstr('set(gh_curwin,'"color_map'",d)','errcatch')
  if ierr<>0 then
    ok = %f ;
  else
    ok = %t ; 
  end
endfunction
