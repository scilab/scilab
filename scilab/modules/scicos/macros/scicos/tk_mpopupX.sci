function Cmenu=tk_mpopupX(ll)
  Cmenu=[]
  if length(ll)==0 then return;end
  [txt,MM]=create_popup(ll)
  ierr=execstr('TCL_EvalStr(txt)','continue')
  done=0
  while ~done,end
endfunction

function [txt,MM]=create_popup(ll)
  MM=[];
  global j
  j=0
  'set numx [winfo pointerx .]'
'set numy [winfo pointery .]'
'wm geometry $w +$numx+$numy'
'wm title $w '"Set Block properties'"'
  
  
txt='catch {destroy .zz};toplevel .zz;set numx [winfo pointerx .];set"+...
    " numy [winfo pointery .];wm geometry .zz +$numx+$numy;wm title .zz '" '";frame .zz.scicoslhb;menubutton .zz.scicoslhb.xx -text '"Possible operations'" -underline 0 -menu .zz.scicoslhb.xx.edit -relief raised;'
[txte,MM]=createmenu(ll,'','.zz.scicoslhb.xx.edit',MM)
txt=txt+txte
txt=txt+' pack .zz.scicoslhb.xx -side left -in .zz.scicoslhb;button"+...
    " .zz.scicoslhb.xy -text Cancel -command {destroy .zz};pack"+...
    " .zz.scicoslhb.xy -side right -in .zz.scicoslhb;"+...
"pack .zz.scicoslhb;bind .zz <Destroy> {ScilabEval '"done=1'"}'
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
	   l+''''";destroy .zz};'
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

//getf tk_mpopup.sci
// ll=list('choix',list('sdf','dd',list('d',list('df','deewq'))),'voo'); 
//  Cmenu=tk_mpopup(ll)





