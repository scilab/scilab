function [m] = tabul(X, order)
   //
   //  PURPOSE
   //     This function computes the frequency of values of 
   //     the components of a vector or matrix X of numbers or
   //     string characters.
   //
   //      If X  is a numerical  vector or matrix then
   //      m  is a  two column matrix who contains in 
   //      the first column the distinct values of X 
   //      and  in the other column the number of occurrences 
   //      of those values (m(i,2) is the number of occurrences
   //      of m(i,1)).
   //
   //      If X is a vector or matrix of strings, m is  a list 
   //      whose  first member is a string vector composed with 
   //      the distinct values of X and the second member is a 
   //      vector whose components are the number of occurrences
   //      of those values ( m(i)(2) is the number of occurrences
   //      of the string m(i)(1) ).
   //
   //      The optional parameter order must be "d" or "i" 
   //      (by default order = "d") and it gives the order
   //      of the distinct vector values of X (first column
   //      or first part of m) :
   //         order = "d" means that these values are sorted
   //                     in decreasing order
   //               = "i" means by increasing order 
   //
   //  AUTHORS
   //      Original version by Carlos Klimann
   //      This version by Jean-Sebastien Giet & Bruno Pincon
   //
   //  date: 1999-04-09 (original version)
   //        2003-Mars-26 (new version)
   //
   //  NOTES
   //      The new version :
   //          is faster (by using no more loop)
   //          performs a complete check of the input arguments
   //          add the order option
   //          used gsort in place of sort : the sort function
   //          sorts strings vector not the usual way !
 
   rhs = argn(2)
   if rhs<1 | 2<rhs then
      error(" tabul : 1 or 2 input argument(s) needed")
   elseif rhs == 1 then
      order = "d"
   end
   typeX = type(X)
   if typeX ~= 1 & typeX ~= 10 then
      error(" tabul : 1st input argument must be a vector/matrix of numbers or strings")
   end
   if type(order) ~= 10 then
      error(" tabul : 2d input argument (order) must be ""i"" or ""d""")
   end
   if order~="i" &  order~="d" then
      error(" tabul : 2d input argument (order) must be ""i"" or ""d""")
   end
   if ( X == [] ) then
      m = %nan
      return
   end
   
   X = X(:)
   X = gsort(X,"g",order)
   n = size(X,"*")
   ind = [find(X(1:$-1)~=X(2:$)) n]

   val = X(ind)
   occ = diff([0 ind])'
   
   if typeX == 1 then
      m = [val occ]
   else // X (and so val) is a vector of strings
      m = list(val,occ)
   end
   
endfunction
