function inference=can_infer(index)
// Copyright INRIA
// Scilab Project - V. Couvert
// Determine if inference can be done with data contained in index
// All index values must be of 'cste' type
inference=%T
for k=1:lstsize(index)
  if typeof(index(k))<>"cste" & typeof(index(k))<>"list" then
    inference=%F
    break
  elseif typeof(index(k))=="list" then
    for l=1:lstsize(index(k))
      if typeof(index(k)(l))<>"cste" then
	inference=%F
	break
      elseif index(k)(l).value==":" then
	inference=%F
	break
      end	
    end
  else
    if index(k).value==":" then
      inference=%F
      break
    end   
  end
end
endfunction
