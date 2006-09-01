function B=%sp_p_s(A,p)
   // handle A^p for scalar integer p only
   // added by bruno (jan 19 2006)
   if length(p) ~= 1 then
      error("A^p with A sparse is not handled for non scalar p")
   end
   if floor(p) ~= p then
      error("A^p with A sparse is not handled for non integer p")
   end
   if p == 0 then
      B = speye(A)
   elseif p < 0 then
      A = inv(A)
      p = -p
   end
   
   select p
     case 1
       B = A
     case 2
       B = A*A
     case 3
       B = A*(A*A)
     case 4
       B = A*A
       B = B*B
   else
      B = A
      for i=2:p   // slow method (we could use the power algorithm)
	 B = A*B
      end
   end
   
endfunction

  
      