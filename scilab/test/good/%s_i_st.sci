function out=%s_i_st(varargin)
// Copyright INRIA
// Modified by Vincent COUVERT (16/08/2004) so that insertion of an empty matrix is understood as an element deletion
// Only one non-colon index can be used 

// Used for struct part deletion
  if lstsize(varargin)>=3 & type(varargin(1))==1 & isempty(varargin($-1)) then
    out=varargin($);
    [Ndims,I]=convertindex(list(double(out.dims),[0 0]),varargin(1:$-2));
    
    Fout=getfield(1,out)
    Fout=Fout(3:$)
    for f=Fout
      for ki=I($:-1:1)'
	out(f)(ki)=null()
      end
    end
    
    // Which dim has to be decremented
    if or(size(varargin($))==1) then // Struct vector
      dim=find(size(varargin($))<>1);
    else
      for kd=1:lstsize(varargin)-2
	if min(size(varargin(kd)))>=0 then
	  dim=kd
	  break
	end
      end
    end
    out.dims(dim)=out.dims(dim)-size(I,"*");
    // If one dim is 0 then all dims are set to 0
    if double(out.dims(dim))==0 then 
      out.dims=int32([0 0]);
    end
  elseif lstsize(varargin)==3 & type(varargin(1))==10 then // out.i=in
    i=varargin(1);
    in=varargin(2);
    out=varargin(3);
    out=generic_i_st(i,in,out)
  else
    error("Not yet implemented")
  end
endfunction
