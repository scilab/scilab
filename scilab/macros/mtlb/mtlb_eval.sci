function r=mtlb_eval(%s1,%s2)
// Copyright INRIA
// Emulation function for Matlab eval()
// V.C.

[lhs,rhs]=argn()
//if exists('m2scilib')==0 then load('SCI/macros/m2sci/lib'),end

%prot=funcprot();
funcprot(0)

// Init all variables useful to launch mfile2sci()
deff("r=isanmfile(s)","r=%f");
res_path=[];
logfile=-1;
batch=%t;
fnam="%fun";
dquote="''";
lcount=1;
level=[0,0];
sciexp=0;
sciparam();

// Scilab variable types
Double=1;
Boolean=4; // Boolean type can also be 6
Sparse=5;
Int=8;
String=10;
Cell=17;
Struct=16;
Void=0;
Unknown=-1;
Complex="Complex"
Real="Real"


varslist=m2sci_init()

// Define a function which contains expression/instruction to eval and convert it
deff("%fun()",%s1)
mtlblst=macr2lst(%fun)
mtlbtree=lst2tree(mtlblst)
[scitree,varslist]=mtlbtree2sci(mtlbtree,varslist)
%txt=tree2code(scitree)
%txt(find(part(%txt,1:2)=="//"))=[]
%txt(1)=[];
%txt($)=[];

%r=0
%nold=size(who("get"),"*")
%r=execstr(%txt,"errcatch")

if %r<>0&rhs==2 then
  deff("%fun()",%s2)
  mtlblst=macr2lst(%fun)
  mtlbtree=lst2tree(mtlblst)
  [scitree,varslist]=mtlbtree2sci(mtlbtree,varslist)
  %txt=tree2code(scitree)
  %txt(find(part(%txt,1:2)=="//"))=[]
  %txt(1)=[];
  %txt($)=[];
  disp(%txt)
  
  %nold=size(who("get"),"*")
  %r=execstr(%txt,"errcatch")
else
  %nold=%nold+1
end
nams=who("get");
if size(nams,"*")<=%nold then
  r=0
  return
end
nams=nams(1:$-%nold)
%r=funcprot(%prot);

if nams=="ans" then
  r=ans
else
  r=[];
  execstr("["+strcat(nams,",")+"]"+"=resume"+"("+strcat(nams,",")+")")
end

endfunction
function [varslist]=m2sci_init()
varslist=list()
varslist($+1)=M2scivar("%i","i",Infer(list(1,1),Type(1,"Complex")))
varslist($+1)=M2scivar("%i","j",Infer(list(1,1),Type(1,"Complex")))
varslist($+1)=M2scivar("%nan","NaN",Infer(list(1,1),Type(1,"Real")))
varslist($+1)=M2scivar("%nan","nan",Infer(list(1,1),Type(1,"Real")))
varslist($+1)=M2scivar("%inf","Inf",Infer(list(1,1),Type(1,"Real")))
varslist($+1)=M2scivar("%inf","inf",Infer(list(1,1),Type(1,"Real")))
varslist($+1)=M2scivar("$","end",Infer(list(1,1),Type(1,"Real")))
varslist($+1)=M2scivar("%pi","pi",Infer(list(1,1),Type(1,"Real")))
varslist($+1)=M2scivar("%eps","eps",Infer(list(1,1),Type(1,"Real")))
endfunction
