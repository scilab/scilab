function count=mtlb_fwrite(fid,a,prec)
// Copyright INRIA
// Emulation function for fwrite() Matlab function
// S.S. V.C.

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
if rhs<3 then prec="uchar";end

[l,k]=find(prec==prectbl)
if l==[] then
  error("The format: "+prec+" is unknown")
end
Prec=prectbl(l,1)
if Prec=="?" then 
  error("The format: "+prec+" is not yet handled")
end
mput(a,Prec,fid)
count=size(a,"*")
    
endfunction
