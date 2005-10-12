function   funcallname=lst_funcall(fil,fnamvect)
//  Copyright INRIA
//
//  LST_FUNCALL function (used by "translatepaths" function) Creates a list of vectors. The first component of each vector is the name of a M-file (found in the Paths to translate), followed by the called functions by this file  
//  Ouput 
//  -funcallname : a list of vectors
//  Input
//  -fil : vector which contains all M-files names (path+name)found in the Paths
//  -fnamvect : vector which contains all M-files names (just the name) found in the Paths

k=strindex(fil,".")
if k<>[]
  ke=k($)-1
  base_name=part(fil,1:ke)
else
  ke=length(fil)
  base_name=fil
end

ksep=strindex(fil,sep)
fnam=part(base_name,ksep($)+1:ke) 
txt=mgetl(fil);

[helppart,txt,batch]=m2sci_syntax(txt)
// save txt vector, helpart and batch after the syntax modification
save(base_name+".tree",txt,helppart,batch)

//Replace TAB by SPACE
txt=strsubst(txt,code2str(-40),"")

if txt~=[] then
  quote="''";
  dquote="""";
  kc=strindex(txt(1),"function");
  kc=kc(1);

  // Define Scilab function
  fprot=funcprot();
  funcprot(0);
  
  // Blanks in file name are replaced by _ for batch
  // kc+9 because 'function '
  ksc=min(strindex(txt(1),";")) // searching for a comment on first line after function prototype
  if isempty(ksc) then 
    ksc=length(txt(1))+1;
    firstline=[]
  else
    firstline=part(txt(1),ksc+1:length(txt(1)));
  end
  
  func_proto=part(txt(1),kc+9:ksc-1)
  keq=min(strindex(func_proto,"="))
  kpar=min(strindex(func_proto,"("))
  
  if isempty(keq) then
    keq=0
  end
  if isempty(kpar) then
    kpar=length(func_proto)+1
  end
  
  mname=strsubst(stripblanks(part(func_proto,keq+1:kpar-1))," ","_")
  func_proto=part(func_proto,1:keq)+..
  strsubst(stripblanks(part(func_proto,keq+1:kpar-1))," ","_")+..
  part(func_proto,kpar:length(func_proto))
  
  deff(func_proto,[firstline;txt(2:$)],"n")
  w=who("get");
  //mname=w(1);
  
  // Compilation
  execstr("comp("+mname+",1)")
  funcprot(fprot)
  
  // Get Scilab pseudo code of the function
  macr=evstr(mname)
  mtlbtree=macr2tree(macr);
  
  if ~batch then
    mtlbtree.name=mname;
  else
    mtlbtree.name="";
  end
  
  funcallname=[]
  ninstr=1
  // variablevect is a vector which contains all variables (excluded functions)
  variablevect=[]  
  for i=1:size(mtlbtree.inputs)
    variablevect=[variablevect;mtlbtree.inputs(i).name]
  end
  
  // search the declared variables in mtlbtree  
  while ninstr<=size(mtlbtree.statements)-3  
    [variablevect]=variablesearch(mtlbtree.statements(ninstr),variablevect) 
    ninstr=ninstr+1
  end
  
  ninstr=1
  // search the called functions in the mtlbtree  
  // funcallname contains the name of the M-file, followed by the called functions
  while ninstr<=size(mtlbtree.statements)-3  
    [funcallname,variablevect]=funcallsearch(mtlbtree.statements(ninstr),funcallname,fnamvect,variablevect) 
    ninstr=ninstr+1
  end
end

// add the M-file name in funcallname vector (at the first index)
funcallname=[mtlbtree.name;funcallname]
endfunction