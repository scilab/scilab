function xpause(i)
  realtimeinit(1d-6),
  realtime(0),
  realtime(i),
endfunction




function rep=x_message(comment,btns)
// Copyright INRIA
  rep=[]
  [lhs,rhs]=argn(0)
  comment=matrix(comment,prod(size(comment)),1)
  if rhs==1 then
  else
    if size(btns,'*')==1
    else
      str=readline()
      rep=find(str==btns)
    end
  end
endfunction

function str=x_dialog(comment,default)
// Copyright INRIA
  if argn(2)==1 then default=' ',end
  str=[]

  while %t do
    s=readline()
    if part(s,1:2)=='o '|part(s,1)=='c ' then break,end
    str=[str;s]
  end
  if str==' ' then str=default,end
  rep=s
  if part(rep,1:2)=='c ' then str=[],end

  ln=max(size(default,1),size(str,1))+2
  lw=max(length([default;str]))+2

  txt=create_txt4(comment,default)

  TCL_EvalStr(txt)

  TCL_EvalStr('$w.mess  insert 0.0 '"'+sci2tcl(default)+''"')
  for t=1:100,  xpause(1000),end
  TCL_EvalStr('$w.mess delete 0.0 end')
  TCL_EvalStr('$w.mess  insert 0.0 '"'+sci2tcl(str)+''"')

  for t=1:min(140*(ln-1),7000),  xpause(2000),end
//disp(timer())
  TCL_EvalStr('destroy $w')
endfunction

function str=x_mdialog(description,labels,valuesini)
// Copyright INRIA
  n=prod(size(labels))
  if argn(2)==2 then valuesini(n)=' ',end

   txt=create_txt1(description,labels,valuesini);

   TCL_EvalStr(txt)
   xpause(500000)
   for k=1:n
     str(k,1)=readline()
     if str(k)==' ' then str(k)=valuesini(k),end
   end
   rep=readline()
   for i=1:3
     txt=create_txt2(valuesini);
     TCL_EvalStr(txt)
     xpause(500000)
     txt=create_txt2(str);
     TCL_EvalStr(txt)
     xpause(500000)
   end
    xpause(1500000)
   TCL_EvalStr('destroy $w')
   if part(rep,1)=='c' then str=[],end
endfunction


function num=x_choose(tochoose,comment,button)
// Copyright INRIA
  num=-1
  while num<0|num>size(tochoose,'*') then
    num=evstr(readline())
  end
  l=list()
  for t=tochoose(:)'
    l($+1)=t,
  end
//  pause
  tk_mpop(l,num,xget('wpos')+[200,200]);

endfunction


function [btn,xc,yc,win,Cmenu]=xclick();
// Copyright INRIA
  global %PT
  xselect()
  str=readline()
//  disp(str)
  rep=evstr('list('+str+')')
  btn=rep(1)
  xc=rep(2)
  yc=rep(3)
  %PT=int([xc,yc])
  mrk=xget('mark')
  xset('mark',0,5)
  plot2d(xc,yc,-1,'000')
  plot2d(xc,yc,-1,'000')
  xset('mark',mrk(1),mrk(2))
  win=0

  if size(rep)>=4 then
    win=rep(4),
    if win>0 then
      ww=xget('window')
//      if ww<>win then
	xset('window',win)
	al=xget('alufunction')
	xset('alufunction',6)
	for k=1:4
	  mrk=xget('mark')
	  xset('mark',0,5)
	  plot2d(xc,yc,-1,'000')
	  xset('mark',mrk(1),mrk(2))
	  xpause(300000)
	end
	xset('alufunction',al)
	xset('window',ww)
//      end
    end
  end
  if size(rep)>=5 then
    Cmenu=rep(5)
  else
    Cmenu=[]
  end
  if rep(1)==-2 then
    //display choices
    mme=part(rep(5),9:strindex(rep(5),'_')-1)
    kk=find(menus(1)==mme)

    l=list()
    tochoose=menus(kk)
    for t=tochoose(:)'
      l($+1)=t,
    end
    num=part(rep(5),max(strindex(rep(5),'('))+1:min(strindex(rep(5),...
						  ')'))-1)
    if kk==6 & evstr(num)>11 then
      return
    else
      tk_mpop(l,evstr(num),xget('wpos')+[(kk-1)*60,60]);
    end
  end
endfunction

function rep=xgetmouse(flag);
// Copyright INRIA
  str=readline()
  rep=evstr('['+str+']')
  // next line for animation
  xpolys(rep(1),rep(2),-1);xpause(10000);xpolys(rep(1),rep(2),-1);
endfunction


function str=readline()
// Copyright INRIA
  global LineCount
  LineCount=LineCount+1
  comm='//'
  blank=' '
  while %t do
    rep=mgetl(%I,1)
    k=strindex(rep,comm)
    if k<>[] then
      k=k(1)
      if part(rep,1:k-1)<>part(blank,1:k-1) then
	str=stripblanks(part(rep,1:k-1))
	com=part(rep,k+1:length(rep))
	if part(str,1:3)=='-->' then
	  execstr(part(str,4:length(str)))
	else
	  break
	end
      end
    else
      n=length(rep)
      str=stripblanks(rep)
      com=emptystr()
      if part(str,1:3)=='-->' then
	execstr(part(str,4:length(str)))
      else
	break
      end
    end
  end
  bl='=';txt=com+':'+str;txt=part(bl,ones(1,60-length(txt)))+txt
endfunction

function p=xgetfile(file_mask,dir,title)
  p=readline()
endfunction


function c=getcolor(title,cini)
// Copyright INRIA
  str=readline()
  c=evstr('['+str+']')
endfunction

function rep=message(comment,btns)
// Copyright INRIA
  if argn(2)==1 then
    rep=x_message(comment)
  else
    rep=x_message(comment,btns)
  end
endfunction

function rep=dialog(labels,valueini)
// Copyright INRIA
  if argn(2)==1 then
    rep=x_dialog(labels)
  else
    rep=x_dialog(labels,valueini)
  end
endfunction

function  txt=create_txt4(comment,default)
  txt=['set w .form'
       'catch {destroy $w}'
       'toplevel $w'
       'wm title $w '"Scicos'"'
       'wm iconname $w '"scicos'"'
       'label $w.msg  -wraplength 4i -justify left -text '"'+sci2tcl(comment)+''"'
       'pack $w.msg -side top'
       'text $w.mess -font  {-*-helvetica-normal-r-*-*-12} -relief sunken -bd 2 -setgrid true -width '+string(lw)+' -height '+string(ln)
       'frame $w.buttons'
       'pack $w.buttons -side bottom -fill x -pady 2m'
       'button $w.buttons.dismiss -text Dismiss'
       'button $w.buttons.code -text OK '
       'pack $w.buttons.dismiss $w.buttons.code -side left -expand 1'
       'pack $w.mess  -expand yes -fill both -side top'
      ]

function txt=create_txt1(title,items,init)
  global %PT
txt=['set w .form'
'catch {destroy $w}'
'toplevel $w'
'set numx '+string(%PT(1))
'set numy '+string(%PT(2))
'wm geometry $w +$numx+$numy'
'wm title $w '"Parameter changes'"'
'wm iconname $w '"form'"'
'#positionWindow $w'
'label $w.msg  -wraplength 4i -justify left -text '"'+sci2tcl(title)+''"'
'frame $w.buttons'
'pack $w.buttons -side bottom -fill x -pady 2m'
'button $w.buttons.dismiss -text Dismiss'
'button $w.buttons.code -text OK '
'pack $w.buttons.dismiss $w.buttons.code -side left -expand 1'];

for i=1:size(items,'*')
  txt=[txt
       'frame $w.f'+string(i)+' -bd 2'
       'entry $w.f'+string(i)+'.entry -relief sunken -width 40'
       'label $w.f'+string(i)+'.label'
       'pack $w.f'+string(i)+'.entry -side right'
       'pack $w.f'+string(i)+'.label -side left'];
end
for i=1:size(items,'*')
txt=[txt
     '$w.f'+string(i)+'.label config -text '"'+items(i)+''"'];
end

 for i=1:size(items,'*')
   txt=[txt
	'$w.f'+string(i)+'.entry insert 0 '"'+sci2tcl(init(i))+''"'
];
   end

tt=''
for i=1:size(items,'*')
  tt=tt+'global x'+string(i)+';set x'+string(i)+' [$w.f'+string(i)+'.entry get];'
end
txt=[txt;
     'proc done1 {w} {'+tt+'}'
]
tt=''
for i=1:size(items,'*')
  tt=tt+'$w.f'+string(i)+' '
end
 txt=[txt;
     'pack $w.msg '+tt+'-side top -fill x'
      'focus $w.f1.entry']
endfunction


function txt=create_txt2(init)
txt=[]

 for i=1:size(init,'*')
   txt=[txt
	'$w.f'+string(i)+'.entry delete 0 37'
	'$w.f'+string(i)+'.entry insert 0 '"'+sci2tcl(init(i))+''"'
];
   end

tt=''
for i=1:size(init,'*')
  tt=tt+'$w.f'+string(i)+' '
end
 txt=[txt;
     'pack $w.msg '+tt+'-side top -fill x'
      'focus $w.f1.entry'
]
endfunction

function tk_mpop(ll,but,pos)
  if MSDOS then return,end
  [lhs,rhs]=argn(0)
  if rhs<3 then pos=-1;end
  if length(ll)==0 then return;end
  for i=1:50,xpause(5000),end
  for bb=1:but-1
  [txt,MM]=create_pop(ll,bb,pos)
  ierr=execstr('TCL_EvalStr(txt)','continue')
  for i=1:50,xpause(5000),end
   txt='catch {destroy .scicoslhb}'
  ierr=execstr('TCL_EvalStr(txt)','continue')
  end
  [txt,MM]=create_pop(ll,but,pos)
  ierr=execstr('TCL_EvalStr(txt)','continue')

  xpause(1000000)
  txt='catch {destroy .scicoslhb}'
  ierr=execstr('TCL_EvalStr(txt)','continue')
endfunction

function [txt,MM]=create_pop(ll,but,pos)
  MM=[];
  global j
  j=0
txt='catch {destroy .scicoslhb};toplevel .scicoslhb;wm state .scicoslhb withdrawn;'
[txte,MM]=createmenu(ll,'','.scicoslhb.edit',MM)
txt=txt+txte
if pos==-1 then
  txt=txt+' proc showpopup {} {set numx [winfo pointerx .];set numy [winfo pointery .];set z {expr {$numy+2}};set numz [eval $z];tk_popup .scicoslhb.edit $numx $numz;.scicoslhb.edit activate '+string(but-1)+'};showpopup'
else

    txt=txt+' proc showpopup {} {set numx '+string(pos(1))+';set numy '+string(pos(2))+';set z {expr {$numy+2}};set numz [eval $z];tk_popup .scicoslhb.edit $numx $numz;.scicoslhb.edit activate '+string(but-1)+'};showpopup'
end
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
