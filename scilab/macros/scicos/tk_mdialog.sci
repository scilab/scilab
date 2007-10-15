function result=tk_mdialog(titlex,items,init)
// Copyright INRIA
if argn(2)<1 then
  titlex=['this is a demo';'this is a demo']
  items=['item 1';'item 2']
end
if argn(2)<3 then
  init=['init 1';'init 2']
end
titlex=sci2tcl(titlex);
for i=1:size(items,'*')
  items(i)=sci2tcl(items(i))
  init(i)=sci2tcl(init(i))
end
txt=tk_create_txt(titlex,items,init);
result=[];
TCL_EvalStr(txt)
done=TCL_GetVar('done')
if done==string(1) then
 for i=1:size(items,'*')
   execstr('result(i)=TCL_GetVar(''x'+string(i)+''')')
 end
end
TCL_EvalStr('destroy $www')
endfunction


function txt=tk_create_txt(titlex,items,init)
txt=['set www .formxx'
     'catch {destroy $www}'
     'toplevel $www'
     'set numx [winfo pointerx .]'
     'set numy [winfo pointery .]'
     'wm geometry $www +$numx+$numy'
     'wm title $www '"Set Block properties'"'
     'wm iconname $www '"form'"'
     '#positionWindow $www'
     'label $www.msg  -wraplength 4i -justify left -text '"'+titlex+''"'
     'frame $www.buttons'
     'pack $www.buttons -side bottom -fill x -pady 2m'
     'button $www.buttons.dismiss -text Dismiss -command {set done 2}'
     'button $www.buttons.code -text OK -command {set done 1}'
     'pack $www.buttons.dismiss $www.buttons.code -side left -expand 1'];

for i=1:size(items,'*')
  txt=[txt
       'frame $www.f'+string(i)+' -bd 2'
       'entry $www.f'+string(i)+'.entry -relief sunken -width 40'
       'label $www.f'+string(i)+'.label'
       'pack $www.f'+string(i)+'.entry -side right'
       'pack $www.f'+string(i)+'.label -side left'];
end
for i=1:size(items,'*')
 txt=[txt
      '$www.f'+string(i)+'.label config -text '"'+items(i)+''"'];
end
for i=1:size(items,'*')
 txt=[txt
      '$www.f'+string(i)+'.entry insert 0 '"'+init(i)+''"'];
end

tt=''
for i=1:size(items,'*')
  tt=tt+'global x'+string(i)+';set x'+string(i)+' [$www.f'+string(i)+'.entry get];'
  //tt=tt+'ScilabEval '"result('+string(i)+')=x'+string(i)+''";'
end
txt=[txt;
     'proc done1 {www} {'+tt+'}']
tt=''
for i=1:size(items,'*')
  tt=tt+'$www.f'+string(i)+' '
end
txt=[txt;
     'pack $www.msg '+tt+'-side top -fill x'
     'focus $www.f1.entry'
     'set done 0'
     'bind $www <Return> {set done 1}'
     'bind $www <Destroy> {set done 2}'
     'tkwait variable done'
     'if {$done==1} {done1 $www}']
endfunction
