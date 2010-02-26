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

function hilite_obj(k,win)
//
//** WARNING: I changed mechanism AND input values:
//**          from:
//**                hilite_obj(o,win)
//**          to
//**                hilite_obj(k,win)
//**
//**          'o' was the object
//**          'k' IS the object INDEX inside 'scs_m'
//**
//** 23 Nov 2006: New mechanism that use ONLY the "mark_mode" ["on","off"]
//**              graphics attribute for BOTH "Block" and "Link"
//**

 //** Get number of Right Hand Side arguments
 rhs = argn(2)
 if rhs>1 then //** if the function is called with two arguments
   if or(winsid()==win) then //** check if "win" is an active window
     //** save the handle of the sel. windows
     gh_winback = gcf();
     //** get the handle of the cur. windows
     gh_curwin = scf(win);
     gh_axes = gca(); 
   else
     return; //** exit point: thw win specified is not in the SCICOS
   end       //** valid window
 else //** use the default active window
   //** get the handle of the cur. figure
   gh_curwin = gcf();
   gh_axes = gca(); 
   //** get the 'win_id' of the cur. figure
   win = gh_curwin.figure_id;
 end

 //** Retrieve graphical handles of objs to hilite
 o_size = size(gh_axes.children);

 //**----------------------------------------------------------------
 //**
 drawlater();

 //** k becomes a single column vector
 k = k(:);
 for i=1:size(k,1) //** loop on number of objects
   gh_k = get_gri(k(i),o_size(1)); 
   //**
   gh_blk = gh_axes.children(gh_k);
   //** set red color for foreground mark property
   gh_blk.children(1).mark_foreground = 5;
   //** bigger thickness (+3)
   //DEBUG YC: les blocs ne sont plus des dessins scilab, mais des images.
   //            ou alors problems d'ordre dans les blocs.
   ////gh_blk.children(1).thickness = gh_blk.children(1).thickness + 3;
   //** activate the selection markers
   gh_blk.children(1).mark_mode = "on";
 end

 //** update the display
 //** draw(gh_curwin.children);
 drawnow()
 //** show_pixmap() ; //** not useful on Scilab 5
 //**----------------------------------------------------------------

 //**----------------------------------------------------------------
 if rhs>1 then //** if the function have been called with two arguments
   //** restore the active window
   scf(gh_winback);
 end
 //**----------------------------------------------------------------

endfunction
