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

function hilite_path(path,mess,with_intermediates)
//hilite a block included in a superblock hierarchy and displays a
//message.   If required all the intermediate superblocs are displayed
//first entry of the path must be a bloc of currently opened diagram  

  global   Scicos_commands

  Scicos_commands=['%diagram_path_objective='+sci2exp(path(1:$-1))+';%scicos_navig=1';
		   'Cmenu=[];show_window();%scicos_navig=[];']

  if argn(2)<3 then with_intermediates=%f,end
  if argn(2)<2 then mess=' ',end
  scs_m;
  mxwin=max(winsid()),opened_windows=[]

  //** save the current figure handle
  gh_wins = gcf();

  hilite_obj(path(1))

  if with_intermediates then
    scs_m=scs_m.objs(path(1)).model.rpar;
    for k=2:size(path,'*')
      scs_show(scs_m,mxwin+k);opened_windows=[mxwin+k opened_windows]
      hilite_obj(path(k))
      scs_m=scs_m.objs(path(k)).model.rpar;
    end
  else
    if size(path,'*')==1 then
      hilite_obj(path)
    else
      for k=1:size(path,'*')-1;scs_m=scs_m.objs(path(k)).model.rpar;end
      scs_show(scs_m,mxwin+1);opened_windows=[mxwin+1 opened_windows]
      hilite_obj(path($))
    end
  end
  messagebox(mess,'modal')

  for k=1:size(opened_windows,'*') //** close opened_windows
    //** select the opened_windows(k) and get the handle
    gh_del = scf(opened_windows(k));
    //** delete the window
    delete(gh_del)
  end
  //scs_m=null()

  //** restore the active window
  scf(gh_wins);

  //** unhilite entity path(1)
  unhilite_obj(path(1))
endfunction
