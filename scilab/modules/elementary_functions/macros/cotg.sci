function [t] = cotg(z)
// 
//  PURPOSE 
//     element wise cotangent
//
  if type(z)<>1 then error(53,1),end
  t = 1 ./tan(z)
endfunction
