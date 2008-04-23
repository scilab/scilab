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

function Cmenu=tk_mpopupX(ll)
// To make it work like in windows case
  
  Cmenu = []
  Done = []
  if length(ll)==0 then
    return
  end

  txt = create_popup(ll)
  ierr = execstr('TCL_EvalStr(txt)','continue')
  while (Cmenu==[]&Done==[])
     xpause(10000)
  end

endfunction
//**-------------------------------------------------------------------------------------------------------------------


function txt=create_popup(ll)
//  global j
  j=0
txt='catch {destroy .scicoslhb};toplevel .scicoslhb;wm state .scicoslhb withdrawn;'
txte=createmenu(ll,'','.scicoslhb.edit',j)
txt=txt+txte

txt=txt+'bind .scicoslhb.edit <Unmap> {ScilabEval '"Done=''1'''" sync};'

txt=txt+' proc showpopup {} {set numx [winfo pointerx .];set numy [winfo pointery .];set z {expr {$numy+2}};set numz [eval $z];tk_popup .scicoslhb.edit $numx $numz;.scicoslhb.edit activate 0};showpopup'
endfunction


function [txt,j]=createmenu(ll,txt,path,j)
//  global j
  i=1
  txt1=[];
  txt2=[];
  txt3=[]
  for l=ll
    if type(l)==10 then
      i=i+1
      txt1=txt1+path+' add command -label '"'+l+''"  -command scicosl'+string(j)+';'
      txt2=txt2+'proc scicosl'+string(j)+' {} {ScilabEval '"Cmenu='''+ ...
	   l+''''" sync};'
      j=j+1
    else
      i=i+1
      if length(l)<2 then error('A menu is empty'),end
      cpath=path+'.edit'+string(i);
      txt1=txt1+path+' add cascade -label '"'+l(1)+''"  -menu '+cpath+';'
      l(1)=null()
      [txt31,j]=createmenu(l,txt,cpath,j);
      txt3=txt3+txt31
    end
  end
  txt=txt+'menu '+ path+' -tearoff 0;'+txt1+txt2+txt3

endfunction


// ll=list('choix',list('sdf','dd',list('d',list('df','deewq'))),'voo'); 
//  Cmenu=tk_mpopup(ll)





