function mfile2sci(fil,res_path,Imode,Recmode)
// preforms translation of a single m-file
// Copyright INRIA

// default arguments
[lhs,rhs]=argn(0)
if rhs<4 then Recmode=%f,end
if rhs<3 then Imode=%f,end
if rhs<2 then res_path='./',end
if MSDOS then
  fil=strsubst(fil,'\','/')
  res_path=strsubst(res_path,'\','/')
end
if part(res_path,length(res_path))<>'/' then res_path=res_path+'/',end

// get context
if exists('m2scilib')==0 then load('SCI/macros/m2sci/lib'),end
global('m2sci_infos')
[l,mac]=where()
Reclevel=size(find(mac=='mfile2sci'),'*')
if Reclevel==1 then
  nametbl=[]
else
  m2sci_infos_save=m2sci_infos
end
m2sci_infos=[%f %f]
if exists('logfile')==0 then
  logfile=%io(2) // logical unit of the logfile
end

// output "begin of translation" message

mss='------------'+part(' ',ones(1,3*Reclevel))+'begin of translation of '+fil+' -----------'
write(logfile,mss)
if logfile<>%io(2) then 
  write(%io(2),mss)
end

res=[]

// handle file path
k=strindex(fil,'.')
if k<>[]
  ke=k($)-1
  basename=part(fil,1:ke)
else
  ke=length(fil)
  basename=fil
end
k=strindex(fil,'/')
if k==[] then
  file_path='./'
else
  file_path=part(fil,1:k($))
end
if exists('Paths')==0 then 
  Paths=file_path,
  if MSDOS then 
    Paths=strsubst(Paths,'/','\')
    mfiles=unix_g('dir /b '+Paths+'*.m')
    sep='\'
  else
    mfiles=unix_g('ls '+Paths+'*.m')
    sep='/'
  end
end

fnam=part(basename,k($)+1:ke) // name of the file witout extension

// read in the file as text
txt=readmfile(fil)
txt=strsubst(txt,code2str(-40),' ')
if txt==[] then 
  write(logfile,'Empty file! nothing done'),
  return,
end

// make minor changes on syntax
[helppart,txt,batch]=m2sci_syntax(txt)

// write .cat file and update whatis
if helppart<>[] then
  catfil=res_path+fnam+'.cat'
  whsfil=res_path+'whatis'
  u=file('open',catfil,'unknown')
  write(u,helppart,'(a)')
  file('close',u)
  if exists('whsfil_unit')==1 then
    write(whsfil_unit,stripblanks(helppart(1))+' @'+fnam,'(a)')
  end
end

if txt==[] then return,end
killed=[];
quote='''';
dquote="""";
kc=strindex(txt(1),'function');kc=kc(1);

// define scilab function
deff(part(txt(1),kc+8:length(txt(1))),txt(2:$),'n')
w=who('get');mname=w(1);nametbl=[nametbl;mname]
if  fnam<>mname then
  mss=['Warning: file '+fil+' defines function '+mname+' instead of '+fnam;
       '         '+mname+'.sci, '+mname+'.cat and sci_'+mname+'.sci will be generated']

  if logfile<>%io(2) then  write(%io(2),mss,'(a)');end
  if logfile>0 then  write(logfile,mss,'(a)'),end
end
//prot=funcprot();funcprot(0);
execstr('comp('+mname+',1)')
// get its pseudo code
code=macr2lst(evstr(mname))
//funcprot(prot)

// perform the translation

[res,trad]=m2sci(code,w(1),Imode,Recmode)

//strip last return and blank lines
n=size(res,1)
while res(n)==part(' ',1:length(res(n))) then n=n-1,end
res=res(1:n-1)
ext='.sci'

// write sci-file
scifil=res_path+fnam+ext
u=file('open',scifil,'unknown')
write(u,res,'(a)')
file('close',u)

// write sci_* translation file
if trad<>[] then
  sci_fil=res_path+'sci_'+mname+'.sci'
  u=file('open',sci_fil,'unknown')
  write(u,trad,'(a)')
  file('close',u)
end

// output summary information
infos=[]
if m2sci_infos(1)&~m2sci_infos(2) then
  infos='Translation may be improved (see the //! comments)'
elseif m2sci_infos(1)&m2sci_infos(2) then
  infos='Translation may be wrong (see the //!! comments) or improved see the (//! comments)'
elseif ~m2sci_infos(1)&m2sci_infos(2) then
  infos='Translation may be wrong (see the //!! comments)'  
end
mss='------------'+part(' ',ones(1,3*Reclevel))+'end of translation of '+fil+' -----------'

write(logfile,[infos;mss])
if logfile<>%io(2) then 
  write(%io(2),[infos;mss])
end
if Reclevel>1 then
  m2sci_infos=m2sci_infos_save
end
nametbl($)=[]
nametbl=resume(nametbl)
endfunction
