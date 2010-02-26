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

function scs_m = do_ident(scs_m)

//** Alan 21/12/06 : use of objects swap in gh_curwin.children.children()
  K=find(Select(:,2)==%win)
  if K==[] then //** if no object are selected,
    xc = %pt(1);     //** take the last "cosclic" mouse position  and
    yc = %pt(2)  ;
    k = getobj(scs_m,[xc;yc]) ; //** look for an object
    if k==[] then return,end  ; //** if no object --> EXIT
  else
    k = Select(K,1)'; %pt=[]  ; //** ... otherwise take the object
  end

  //** Filter out the "mutiple object selected" case
  if size(k,'*')>1
    messagebox(_("Only one block can be selected in current window for this operation."),"modal")
    Cmenu=[]; %pt=[]; return
  end

  numero_objet = k     ; //** object ID
  scs_m_save   = scs_m ; //** save the diagram (for "Cancel" operation)

  objet = scs_m.objs(numero_objet) ; //** isolate the object data structure
  type_objet = typeof(objet)       ;

  //** Acquire the current clicked window 
  gh_curwin = scf(%win) ;
  gh_axes = gca(); 
  o_size = size(gh_axes.children) ; //** o_size(1) is the number of compound object
  gr_k = get_gri(k,o_size(1));
  
  //** select the possible cases
  if type_objet == "Block" then
    //** -------- BLOCK --------------
    identification = objet.graphics.id
    if identification == [] then
      identification = emptystr() ;
    end
    //** Use a dialog box to acquire/modify the id string
    texte_1 = _("Set Block identification") ;
    texte_2 = "ID"                       ;
    [ok, identification] = scicos_getvalue(texte_1, texte_2, list('str', 1), identification) ;

    if ok then
      objet.graphics.id = stripblanks(identification); //** update the identification structure
      gr_k = get_gri(numero_objet, o_size(1));
      drawlater();
        update_gr(gr_k, objet);
        drawnow(); 
      scs_m.objs(numero_objet) = objet ; //** update the object data structure
    end
    //**----------------------------
  elseif type_objet == "Link" then
    //** ----- LINK -----------
    identification = objet.id ;
    if identification == [] then
      identification = emptystr() ;
    end
    //** Use a dialog box to acquire/modify the id string
    texte_1 = _("Set link Identification") ;
    texte_2 = "ID"                      ;
    [ok, identification] = scicos_getvalue(texte_1, texte_2, list('str', 1),identification) ;
    //
    if ok then
      identification = stripblanks(identification)  ;
      c_links = connected_links(scs_m, numero_objet) ;
      //- set identification to all connected links
      drawlater();
      for numero = c_links ;
        scs_m.objs(numero).id = identification ;
        gr_k = get_gri(numero, o_size(1)) ; 
        update_gr(gr_k, scs_m.objs(numero)) ;
      end

      drawnow(); 
      
    end
  else
  //** It is NOT a Block AND it is NOT a Link: for any other object type
    messagebox(_("It is impossible to set ID for this type of object"),'error','modal')
  end
  //

  if ok then [scs_m_save,enable_undo,edited] = resume(scs_m_save,%t,%t),end

endfunction
