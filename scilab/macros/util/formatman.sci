function formatman(path,to)
[lhs,rhs]=argn(0)
if rhs<1 then path='./',end
if rhs<2 then to='ascii',end

select to
case 'ascii'
  ext='.cat'
  whatis='whatis'
  convert=man2ascii
  getwhatis=asciiwhatis
case 'tex' 
  ext='.tex'
  whatis='whatis.tex'
  convert=man2tex
  getwhatis=texwhatis
case 'html'
  ext='.html'
  whatis='index.html'
  convert=man2html
  getwhatis=htmlwhatis
else
   error('Only ascii, tex or html output format supported')
end

path=stripblanks(path)
if MSDOS then
  if part(path,length(path))<>'\' then
     path=path+'\';
  end
else
  if part(path,length(path))<>'/' then
     path=path+'/';
  end
end

path1=path
if part(path,1:4)=='SCI/' then path=SCI+part(path,4:length(path)),end
if part(path,1:2)=='~/' then path=getenv('HOME')+part(path,2:length(path)),end

if MSDOS then
  if part(path,1:4)=='SCI\' then path=SCI+part(path,4:length(path)),end
  path=strsubst(path,'/','\')
  lst=unix_g('dir /B /OD '""+path+'* ""')
  lst=lst($:-1:1)
else
  lst=unix_g('ls  -t1 '+path+'*.*') 
end
// lookfor .man files
man=[];
for k=1:size(lst,'*')
  ke=strindex(lst(k),'.man')
  if ke<>[] then 
    if ke($)==length(lst(k))-3 then
      man=[man;k];
    end
  end
end
modified=%f
for k1=1:size(man,'*')  // loop on .man files
  k=man(k1);
  if MSDOS then
    fl=path+lst(k)
    fnam=lst(k)
  else
    fl=lst(k)
    ks=strindex(fl,'/')
    if ks==[] then fnam=fl;else fnam=part(fl,ks($)+1:length(fl));end
  end
  cat=find(strsubst(lst(k),'.man',ext)==lst)
  if cat==[]| cat>k then
    modified=%t
    mputl(convert(getman(fl)),strsubst(fl,'.man',ext))
  end
end

if modified then //create whatis file
  WH=[]
  for k1=1:size(man,'*')  // loop on .man files
    k=man(k1);
    if MSDOS then
      fl=path+lst(k)
      fnam=lst(k)
    else
      fl=lst(k)
      ks=strindex(fl,'/')
      if ks==[] then fnam=fl;else fnam=part(fl,ks($)+1:length(fl));end
    end
    WH=[WH;getwhatis(fl,strsubst(fnam,'.man',''))]
  end
  mputl(gsort(WH,'g','i'),path+whatis)
end


function txt=man2ascii(man,ind)
if argn(2)<2 then ind=1,end
ll=75
txt=[];k=0
n=size(man)
item=[]
while k<n
  k=k+1;mk=man(k)
  select mk(1)
  case 'TH' then
    n1=ll-length(mk(2))*2-length(mk(4))-length(mk(5))
    txt=[txt;
	mk(2)+part(' ',ones(1,n1/3))+mk(4)+part(' ',ones(1,n1/3))+..
               mk(5)+part(' ',ones(1,n1-2*int(n1/3)))+mk(2)]
  case 'SH' then
    txt=[txt;convstr(mk(2),'u')]   
    ind=ind(1)
  case 'begin_indent' then
    ind($+1)=mk(2)
  case 'end_indent' then
    if ind<>[] then ind($)=[],end
  case 'item' then
    item=mk(2)
  case 'latex' then
  case 'latex_ignore' then
    txt=[txt;man2ascii(mk(2),ind)]
  case 'table' then
    txt=[txt;maketable(mk(2),ind)]
  case 'fill' then
    txt=[txt;justify(item,mk(2),ind,mk(3))]
    item=[]
  case 'verbatim' then
    txt=[txt;part(' ',ones(1,sum(ind)))+mk(2)]
  case 'font' then
  else
  end
end

function txt=man2tex(man)
txt=[];k=0
n=size(man)
while k<n
  k=k+1;mk=man(k)
  select mk(1)
  case 'TH' then
    txt=[txt;
	'\phead{'+texsubstitute(mk(2))+'}{1}{'+texsubstitute(mk(3))+..
	    '}{'+texsubstitute(mk(4))+'}{'+texsubstitute(mk(5))+'}']
  case 'SH' then
    t=convstr(mk(2),'u')
    if t=='NAME' then
      k=k+1
      t=man(k)(2)
      p=strindex(t,' -');nm=texsubstitute(stripblanks(part(t,1:p-1)))
      txt=[txt;
	  '\Sdoc{'+nm+'}{'+part(t,p:length(t))+'}\index{'+nm+'}\label{'+nm+'}'
	  '\begin{flushleft}'
          '\end{flushleft}']
    elseif t=='SEE ALSO' then
      txt=[txt;
	  '\Seealso{SEE ALSO}']
      k=k+1
      t=strcat(man(k)(2),' ')
      p=[0 strindex(t,',') length(t)+1]
      nm=[]
      for i=2:size(p,2)
	nm=[nm,part(t,p(i-1)+1:p(i)-1)]
      end
      nm=stripblanks(nm)
      txt=[txt;
	  strcat('{\verb?'+nm+'?}'+' \pageref{'+nm+'}',',')]
    else
      txt=[txt;'\Shead{'+texsubstitute(t)+'}']
    end
  case 'begin_indent' then
    txt=[txt;'\begin{scitem}']
  case 'end_indent' then
    txt=[txt;'\end{scitem}% end Env']
  case 'item' then
    txt=[txt;'\item[{\tt '+mk(2)(1)+'}]']
  case 'latex' then
    txt=[txt;mk(2)]
  case 'latex_ignore' then

  case 'table' then
    txt=[txt;table2tex(mk(2))]
  case 'fill' then
    mk(2)=texsubstitute(mk(2))
    txt=[txt;mk(2)]
  case 'verbatim' then
    txt=[txt;
	'\begin{verbatim}'
	mk(2)
	'\end{verbatim}']
  case 'font' then
  else
  end
end

function txt=man2html(man)
txt=[];k=0
n=size(man)
bl='&nbsp'
bl10=strcat(bl(ones(1,10)))+' '
while k<n
  k=k+1;mk=man(k)
  select mk(1)
  case 'TH' then
    txt=[txt;
	'<body>'
	mk(2)+bl10+mk(3)+bl10+mk(4)+bl10+mk(5)]
  case 'SH' then
    mk(2)=htmlsubstitute(mk(2))
    t=convstr(mk(2),'u')
    txt=[txt;
	  '<h1>'
	  t+'</h1>']
    if t=='SEE ALSO' then
      k=k+1
      if size(man(k))>=2
	t=strcat(man(k)(2),' ')
	p=[0 strindex(t,',') length(t)+1]
	nm=[]
	for i=2:size(p,2)
	  nm=[nm,part(t,p(i-1)+1:p(i)-1)]
	end
	nm=stripblanks(nm)
	txt=[txt;
	    '<br>'+strcat('<a href=""'+gethtmlref(nm)+'"">'+nm+'</a>',', ')]
      end
    end

  case 'begin_indent' then
    txt=[txt;'<ul>']
  case 'end_indent' then
    txt=[txt;'</ul>']
  case 'item' then
    txt=[txt;
	  '';
	  '<li>'
	  +'<b>'+mk(2)(1)+'</b></li>']
  case 'latex' then
  case 'latex_ignore' then
    txt=[txt;man2html(mk(2))]
  case 'table' then
    txt=[txt;table2html(mk(2))]
  case 'fill' then
    mk(2)=htmlsubstitute(mk(2))
    txt=[txt;
	strcat(mk(2)(1:$),' ')+'<br>']
  case 'verbatim' then
    mk(2)=htmlsubstitute(mk(2))
    txt=[txt;
	'<br><TT>'+strsubst(mk(2),' ','&nbsp;')+'</TT>'
	'<br>']
  case 'font' then
  else
  end
end

function man=getman(path)
//given a man file, this function analyse it and returns a data structure 
write(%io(2),'Processing '+path)
ind_def=4
man=list()
txt=mgetl(path);
if txt==[] then return,end
txt=strsubst(txt,code2str(-40)," ")
txt($+1)='.LP '
//
debugflag=%f
com=find(part(txt,1)=='.');//les index des commandes
k=0
begin=%t;tp=0
fill=%t;table=%f;output=%f;k1=1
while k<size(com,'*')
  k=k+1
  tk=txt(com(k))
  OP=part(tk,1:4);//disp(OP)
  select OP
  case '.TH ' then
    th=stripblanks(part(tk,5:length(tk)))
    p=strindex(th,' ')
    nm=strsubst(part(th,1:p(1)-1),'""','')
    th=part(th,p(1)+1:length(th));p=strindex(th,'""')
    if size(p,2)>=2 then dt=part(th,p(1)+1:p(2)-1),else dt='',end
    if size(p,2)>=4 then aut=part(th,p(3)+1:p(4)-1),else aut='',end
    if size(p,2)>=6 then typ=part(th,p(5)+1:p(6)-1),else typ='',end
    man($+1)=list('TH',nm,dt,aut,typ)
    k1=com(k)+1
  case '.nf '
    if com(k)-1>=k1 then
      k2=com(k)-1
      man($+1)=list('fill',replacefonts(txt(k1:k2)),tp),
    end
    fill=%f
    k1=com(k)+1
  case '.RS '
    output=%t
    begin=%t
  case '.FI '
    man(pos)=list('latex_ignore',list(man(pos:$)))
    for kp=pos+1:size(man),man(kp)=null(),end
    output=%t
  case '.ft '
    man($+1)=list('font')
    output=%t
  case '.fi' then
    if com(k)-1>=k1 then
      k2=com(k)-1
      man($+1)=list('verbatim',strsubst(txt(k1:k2),'\\','\')),
    end
    output=%f
  else
    output=%t
  end
  if output then
    k2=com(k)-1
    if table then //.TS ... .TE
      man($+1)=list('table',gettable(txt(k1:k2)))
      table=%f
    elseif fill then
      if k2>=k1 then 
	man($+1)=list('fill',replacefonts(txt(k1:k2)),tp),
      end
    else //.nf  .fi
      if k2>=k1 then 
	man($+1)=list('verbatim',strsubst(txt(k1:k2),'\\','\')),
      end
      fill=%t
    end
    output=%f
    k1=k2+2
    select OP
    case '.SH ' then
      if tp>0 then man($+1)=list('end_indent'),tp=tp-1;end
      man($+1)=list('SH',stripblanks(part(tk,5:length(tk)))) 
      begin=%t
      fill=%t
      k1=com(k)+1
    case '.LA ' then 
      if man($)(1)=='latex' then
	man($)(2)($+1)=part(tk,5:length(tk))
      else
	man($+1)=list('latex',part(tk,5:length(tk)))
      end
      k1=com(k)+1
    case '.RE ' then
      man($+1)=list('end_indent')
      tp=tp-1
    case '.IG '
      pos=size(man)+1
    case '.TP '
      if begin then
	if stripblanks(part(tk,5:length(tk)))<>'' then
	  man($+1)=list('begin_indent',evstr(part(tk,5:length(tk))))
	else
	  man($+1)=list('begin_indent',ind_def)
	end
	begin=%f
	k1=com(k)+1
	tp=tp+1
      end
      man($+1)=list('item',stripblanks(txt(k1)))
      k1=k1+1
    case '.IP ' then
      if begin then
	man($+1)=list('begin_indent',ind_def)
	begin=%f
	tp=tp+1
      end
      txt(com(k))=part(txt(com(k)),4:length(txt(com(k))))
      k1=com(k)
    end
    table=OP=='.TS '
  end
end
if tp>0 then man($+1)=list('end_indent'),tp=tp-1,end

function tab=gettable(txt)
//given troff instruction defining a table this function returns a 
//matrix of string
txt=replacefonts(txt)
t1=txt(1)
k=strindex(t1,')')
if part(t1,1:4)<>'tab('|k==[] then error('invalid table definition'),end
del=part(t1,5:k-1) // the column delimiter
t2=txt(2) //alignment def
tab=[]
for k=1:size(txt,1)-2
  tk=txt(k+2)
  c=[0 strindex(tk,del) length(tk)+1]
  for i=2:size(c,'*')
    tab(k,i-1)=part(tk,c(i-1)+1:c(i)-1)
  end
end
function wh=asciiwhatis(path,fnam)
txt=mgetl(path)
d=find(part(txt,1:8)=='.SH NAME')
if d==[] then wh=[],return,end
d=d(1)+1
f=find(part(txt(d:$),1:4)=='.SH ')
if f==[] then f=$+1,end
wh=txt(d:d+f(1)-2)
k=find(part(wh,1:3)=='.nf'|part(wh,1:3)=='.fi')
if k<>[] then wh(k)=[];end
wh=stripblanks(wh)
k=find(wh=='');if k<>[] then wh(k)=[];end
wh=wh+'  @'+fnam

function wh=texwhatis(path,fnam)
wh='\input '+fnam


function wh=htmlwhatis(path,fnam)
txt=mgetl(path)
d=find(part(txt,1:8)=='.SH NAME')
if d==[] then wh=[],return,end
d=d(1)+1
f=find(part(txt(d:$),1:4)=='.SH ')
if f==[] then f=$+1,end
wh=txt(d:d+f(1)-2)
k=find(part(wh,1:3)=='.nf'|part(wh,1:3)=='.fi')
if k<>[] then wh(k)=[];end
fnam=fnam+'.html'
for k=1:size(wh,1)
  whk=stripblanks(wh(k))
  p=strindex(wh(k),' - ');
  if p==[] then
    p=strindex(wh(k),' ')
  end
  p=p(1)

  whk=part(whk,1:p-1)+'</a>'+part(whk,p:length(whk))
  wh(k)='<br><a href=""'+fnam+'"">'+whk+'<br>'
end

function txt=replacefonts(txt)
fonts='\f'+['V','R','B','P','I','(CR']
for f=fonts,txt=strsubst(txt,f,''),end
//suppress inline comments
[l,k]=grep(txt,'\""')
for i=l
  txt(i)=part(txt(i),1:k-1)
end
txt=strsubst(txt,'\\','\')

function t=maketable(tab,ind)
t=part(' ',ones(1,sum(ind)))+emptystr(size(tab,1),1)+'|'
for k=1:size(tab,2)
  t=t+part(tab(:,k),1:max(length(tab(:,k))))+'|'
end

function t=justify(item,txt,ind,tp)
curind=sum(ind)
tp=max(0,tp)
t=[]
if txt==[] then return,end
txt=replacefonts(txt)
if item<>[]&ind<>[] then
  item=replacefonts(item)
  if length(item)>ind($) then
    t=[t;part(' ',ones(1,sum(ind(1:$-1))))+item]
    txt(1)=part(' ',ones(1,sum(ind(1:$-1))))+txt(1)
  else 
    txt=[part(' ',ones(1,sum(ind(1:$-1))))+part(item,1:ind($));txt]
  end
else
  txt(1)=part(' ',ones(1,curind))+txt(1)
end
txt=strcat(txt,' ')+' '

ind=strindex(txt,' ')

k0=1
shift=0
while %t
  k=find(ind<k0+ll-shift);
  if k==[] then break,end
  k=k($)
  t=[t;part(txt,k0:ind(k)-1)]
  k0=ind(k)+1
  ind(1:k)=[]
  shift=curind
end
t=[t;part(txt,k0:length(txt))]
if tp then k1=2,else k1=1,end
t(k1:$)=part(' ',ones(1,curind+1))+t(k1:$)

function t=texsubstitute(t)
    t=strsubst(t,'_','\_')
    t=strsubst(t,'%','\%')
    t=strsubst(t,'&','\&')
    t=strsubst(t,'<','$<$')
    t=strsubst(t,'>','$>$')
function t=htmlsubstitute(t)
    t=strsubst(t,'<','&lt ')

function t=gethtmlref(nm)
nm=stripblanks(nm)
if MSDOS then del='\',else del='/',end
t=emptystr(nm)
ok=t==nm;
nh=size(%helps,1)
k=0
while ~and(ok)&k<nh
  k=k+1
  for l=1:size(nm,'*')
    if ~ok(l) then
      if find(part(mgetl(%helps(k,1)+del+'whatis'),1:length(nm(l)))==nm(l))<>[] then
	t(l)=%helps(k,1)+del+nm(l)
	ok(l)=%t
      end
    end
  end
end
undef=find(t=='')
t(undef)=nm(undef)
t=t+'.html'

function t=table2html(tab)
t='<table BORDER NOSAVE >'
//t='<table NOSAVE >'
for l=1:size(tab,1)
  t=[t;'<tr>']
  for k=1:size(tab,2),t=[t;'<td>'+tab(l,k)+'</td>'],end
  t=[t;'</tr>']
end
t=[t;'</table>']
