function [t] = cotg(z)
  // 
  //  PURPOSE 
  //     element wise cotangent
  //
  if type(z)<>1 then error(53),end
  t = 1 ./tan(z)
