function out=generic_i_st(i,in,out)
// Authors: F. Delebecque, S. Steer, Copyright INRIA 
// out.i=in  
// this function is called only if the i field is not already defined in out
//
  if prod(out.dims)>1 then error(1),end 
  T=getfield(1,out);
  T($+1)=i;setfield(1,T,out)
  setfield($+1,in,out)
  if double(out.dims)==[0 0] then out.dims=int32([1 1]),end
endfunction
