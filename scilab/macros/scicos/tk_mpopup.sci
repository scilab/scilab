function Cmenu=tk_mpopup(ll)
// Copyright INRIA
  
  Cmenu = []
  if length(ll)==0 then
    return
  end

  txt = create_popup(ll)
  ierr = execstr('TCL_EvalStr(txt)','continue')
endfunction
//**-------------------------------------------------------------------------------------------------------------------



function txt=create_popup(ll)
//  global j
  j=0
txt='catch {destroy .scicoslhb};toplevel .scicoslhb;wm state .scicoslhb withdrawn;'
txte=createmenu(ll,'','.scicoslhb.edit',j)
txt=txt+txte

txt=txt+'bind .scicoslhb.edit <Unmap> {ScilabEval '"Done=''1'''"};'

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
	   l+''''"};'
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





