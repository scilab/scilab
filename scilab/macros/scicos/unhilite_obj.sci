function unhilite_obj(k,win)
// Copyright INRIA
//
//** 21 Set 2006:
//** WARNING: I changed mechanism AND input values:
//**          from:
//**                unhilite_obj(o,win)
//**          to
//**                unhilite_obj(k,win)
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
   else
     return; //** exit point: the 'win' specified is not in the SCICOS
   end       //** valid window
 else //** use the default active window
   //** get the handle of the cur. figure
   gh_curwin = gcf();
   //** get the 'win_id' of the cur. figure
   win = gh_curwin.figure_id;
 end

 //** Retrieve graphical handles of objs to hilite
 o_size = size(gh_curwin.children.children);

 //**----------------------------------------------------------------
 //**
 drawlater();

 //** k becomes a single column vector
 k=k(:);
 for i=1:size(k,1) //** loop on number of objects
   //** semi empirical equation :)
   //gh_k = o_size(1) - k(i) + 1;
   gh_k=get_gri(k(i),o_size(1))
   //**
   gh_blk = gh_curwin.children.children(gh_k);
   //** set color = -1 as default for foregroung mark property
   gh_blk.children(1).mark_foreground = -1;
   //** smaller thickness (-3)
   gh_blk.children(1).thickness=gh_blk.children(1).thickness - 3;
   //** loop to know if it is an already selected objs
   kk=find(Select(:,1)==k(i));
   if kk==[] then
     //**deactivate the selection markers
     gh_blk.children(1).mark_mode = "off";
   else
     if find(Select(kk,2)==win)==[] then
       //**deactivate the selection markers
       gh_blk.children(1).mark_mode = "off";
     end
   end
 end

 //** update the display
 draw(gh_curwin.children);
 show_pixmap();
 //**----------------------------------------------------------------

 //**----------------------------------------------------------------
 if rhs>1 then //** if the function have been called with two arguments
   //** restore the active window
   scf(gh_winback);
 end
 //**----------------------------------------------------------------

endfunction
