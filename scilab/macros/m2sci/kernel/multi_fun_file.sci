function bval=multi_fun_file(fil,res_path,Recmode,only_double,verbose_mode,prettyprint)
// Copyright INRIA
// Scilab Project - V. Couvert
// This function converts M-Files containing more than one functio
// Inputs are the same as mfile2sci()
// Outputs :
// - bval: boolean value, %t if file contains more than on function, %f else

sciparam();

// File name
k=strindex(fil,".")
if k<>[]
  ke=k($)-1
  base_name=part(fil,1:ke)
else
  ke=length(fil)
  base_name=fil
end
// File path
k=strindex(fil,"/")
if k==[] then
  file_path="./"
else
  file_path=part(fil,1:k($))
end
base_name=part(base_name,k($)+1:ke)

txt=mgetl(fil);

kf=grep(txt,"function")

if isempty(kf) then
  // Batch file
  bval=%f
elseif size(kf,"*")==1 then
  // Only one function defined
  bval=%f
else
  funcdecl=[]
  for kk=kf
    ind=strindex(txt(kk),"function")
    if isacomment(txt(kk))==0 & ~isinstring(txt(kk),ind) & part(stripblanks(txt(kk)),1:8)=="function" then // function prototype
      funcdecl=[funcdecl kk]
    end
  end
  
  if isempty(funcdecl) then
    // "function" only exists in comments and strings
    bval=%f
    return
  elseif size(funcdecl,"*")==1 then
    bval=%f
    return
  end
  
  write(%io(2)," -- File "+fil+" contains more than one function -- ");
      
  bval= %t
  
  // First split file into as many files as function declared
  funcdecl=[funcdecl size(txt,"*")+1] 
  
  tmpfiles=[]
  for k=1:size(funcdecl,"*")-1
    functxt=txt(funcdecl(k):funcdecl(k+1)-1)
    funcname=stripblanks(part(txt(funcdecl(k)),strindex(txt(funcdecl(k)),"function")+8:strindex(txt(funcdecl(k)),"(")-1))
    keq=strindex(funcname,"=")
    if ~isempty(keq) then
      funcname=stripblanks(part(funcname,keq+1:length(funcname)))
    end
    tmpfiles=[tmpfiles;funcname]
    mputl(functxt,pathconvert(TMPDIR)+tmpfiles($)+".m")
  end
  
  write(%io(2)," -- Each function converted separately: "+strcat(tmpfiles," ")+" -- ");
  write(%io(2)," -- Temporary files put in: "+pathconvert(TMPDIR));

  // Conversion of each file
  for k=1:size(tmpfiles,"*")
    mfile2sci(pathconvert(TMPDIR)+tmpfiles(k)+".m",res_path,Recmode,only_double,verbose_mode,prettyprint)
  end
  
  // Catenation of all .sci files to have only one output file
  txt=[]
  for k=1:size(tmpfiles,"*")
    txt=[txt ; mgetl(res_path+tmpfiles(k)+".sci")]
  end
  
  // Delete useless .sci files
  for k=1:size(tmpfiles,"*")
    mdelete(res_path+tmpfiles(k)+".sci")
  end
   
  mputl(txt,res_path+base_name+".sci")

  // Catenation of all .log files to have only one output file
  if exists("logfile")==0 then
    txt=[]
    for k=1:size(tmpfiles,"*")
      txt=[txt ; mgetl(res_path+"m2sci_"+tmpfiles(k)+".log")]
    end
    
    // Delete useless .log files
    for k=1:size(tmpfiles,"*")
      mdelete(res_path+"m2sci_"+tmpfiles(k)+".log")
    end
  
    mputl(txt,res_path+"m2sci_"+base_name+".log")
  end
  
  // Delete useless .m files
  for k=1:size(tmpfiles,"*")
    mdelete(pathconvert(TMPDIR)+tmpfiles(k)+".m")
  end
  
  
end
  
endfunction
