function r=%hm_h_hm(a,b)
   // define the operation a & b for boolean hypermatrices a and b
   // bruno (Jan 5 2006)
   // a & b is defined if a and b are boolean hypermatrices
   //                  and if a and b have the same dimensions
   //                      or if one have only one element
   if type(a.entries) ~= 4 | type(b.entries) ~= 4 then
      error("operand not a boolean hypermat")
   end
   if and(a.dims==b.dims) then
      r=hypermat(a.dims,a.entries & b.entries)
   elseif prod(a.dims) == 1 then
      r=hypermat(b.dims,a.entries & b.entries)
   elseif prod(b.dims) == 1 then
      r=hypermat(a.dims,a.entries & b.entries)
   else
      error("incompatible operand dimensions")
   end
endfunction
