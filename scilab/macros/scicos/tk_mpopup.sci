function Cmenu=tk_mpopup(ll)
  Cmenu=[]
  if length(ll)==0 then return;end

  [txt,MM]=create_popup(ll)
  ierr=execstr('TK_EvalStr(txt)','continue')
endfunction

function [txt,MM]=create_popup(ll)
  MM=[];
  global j
  j=0
txt='catch {destroy .scicoslhb};toplevel .scicoslhb;wm state .scicoslhb withdrawn;'
[txte,MM]=createmenu(ll,'','.scicoslhb.edit',MM)
txt=txt+txte
txt=txt+' proc showpopup {} {set numx [winfo pointerx .];set numy [winfo pointery .];set z {expr {$numy+2}};set numz [eval $z];tk_popup .scicoslhb.edit $numx $numz;.scicoslhb.edit activate 0};showpopup'
endfunction


function [txt,MM]=createmenu(ll,txt,path,MM)
  global j
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
      if length(l)<2 then error('A menu is empty'),end
      cpath=path+'.edit'+string(i);
      txt1=txt1+path+' add cascade -label '"'+l(1)+''"  -menu '+cpath+';'
      l(1)=null(1)
      [txt3,MM]=createmenu(l,txt,cpath,MM);
    end
  end
  MM=[MM,path]
  txt=txt+'menu '+ path+' -tearoff 0;'+txt1+txt2+txt3
endfunction


// ll=list('choix',list('sdf','dd',list('d',list('df','deewq'))),'voo'); 
//  Cmenu=tk_mpopup(ll)





