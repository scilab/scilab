function out=%sp_i_struct(i,in,out)
T=getfield(1,out);T($+1)=i;
setfield(1,T,out)
setfield($+1,in,out)

  
