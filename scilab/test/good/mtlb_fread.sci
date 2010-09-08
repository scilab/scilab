// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [a, count]=mtlb_fread(fid,sz,prec)
// Emulation function for fread() Matlab function

prectbl=["c"  "char"    "char"          
         "?"  "schar"   "signed char"   
         "s"  "short"   "short"         
         "l"  "int"     "int"           
         "l"  "long"    "long"          
         "f"  "float"   "float"         
         "d"  "double"  "double"        
         "uc" "uchar"   "unsigned char" 
         "us" "ushort"  "unsigned short"
         "ul" "uint"    "unsigned int"  
         "ul" "ulong"   "unsigned long" 
         "c"  "char"    "char*1"        
         "f"  "float32" "real*4"        
         "d"  "float64" "real*8"        
         "c"  "int8"    "integer*1"     
         "s"  "int16"   "integer*2"     
         "l"  "int32"   "integer*4"     
         "?"  ""        "integer*8"     
         "?"  "intN"    ""                
         "?"  "uintN"   ""         ]       

[lhs,rhs]=argn(0)
select rhs
case 1 then
  sz=%inf
  prec="uchar"
case 2 then
  prec="uchar"
end
[l,k]=find(prec==prectbl)
if l==[] then
  error(msprintf(gettext("%s: The format ''%s'' is unknown.\n"), "mtlb_fread", prec))
end
Prec=prectbl(l,1)
if Prec=="?" then 
  error(msprintf(gettext("%s: The format ''%s'' is not yet handled.\n"), "mtlb_fread", prec))
end
if isinf(sz) then
  a=[]
  while %t
    x=mget(1,Prec,fid)
    if meof(fid)<>0 then
      mclearerr(fid)
      break
    end
    a=[a;x]
  end
  count=size(a,1)
else
  a=mget(prod(sz),Prec,fid)

  if meof(fid)<>0 then
    mclearerr(fid)
    error(msprintf(gettext("%s: End of file reached before all matrix is read.\n"), "mtlb_fread"))
  end
  if size(sz,"*")==1 then
    a=a(:)
  else
    a=matrix(a,sz(1),sz(2))
  end
  count=size(a,"*")
end
    
endfunction
