function [a, count]=mtlb_fread(fid,sz,prec)
// Copyright INRIA
// Emulation function for fread() Matlab function
// V.C.

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
  error("The format: "+prec+" is unknown")
  end
Prec=prectbl(l,1)
if Prec=="?" then 
  error("The format: "+prec+" is not yet handled")
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
    error("End of file reached before all matrix is read")
  end
  if size(sz,"*")==1 then
    a=a(:)
  else
    a=matrix(a,sz(1),sz(2))
  end
  count=size(a,"*")
end
    
endfunction
