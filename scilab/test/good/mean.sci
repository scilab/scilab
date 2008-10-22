function [y] = mean(x,orient)
// Copyright INRIA
//
// NOTES
//    - modified by bruno to handle the case when x is an hypermatrix
//    - restricted to matrix or hypermatrix of "constants" (that is
//      real or complex numbers)
//    - mean([]) return nan but mean([],orient) return [] (this
//      is strange but corresponds to matlab behavior)
   
   [lhs,rhs] = argn()
   // some arguments checking
   if rhs == 0 | rhs > 2 then
      error("mean: bad number of arguments (must be 1 or 2)")
   else
      if type(x) ~= 1  &  typeof(x) ~= "hypermat" then
	 error("mean: bad type of the first argument")
      end
      if type(x) ~= 1 then  // that is x is an hypermat !
	 if type(x.entries) ~= 1 then
	    error("mean: bad type of the first argument")
	 end
      end
      if rhs == 2 then
	 if orient == "r" then, orient = 1, end 
	 if orient == "c" then, orient = 2, end
	 if orient == "m" then 
	   flag=find(size(x)>1,1)
	   if flag==[] then
	     y=mean(x)
	     return
	   else
	     y=mean(x,flag)
	     return
	   end
	 end
	 if floor(orient)~=orient | orient < 1 | orient > length(size(x)) then
	    error("mean: bad second argument")
	 end
      end 
   end

   if rhs==1 then
      if x==[] then y=%nan;return,end
      y=sum(x)/size(x,'*')
   else
      if x==[] then y=[];return,end
      y=sum(x,orient)/size(x,orient)
   end
endfunction
