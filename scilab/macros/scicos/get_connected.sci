function connected=get_connected(scs_m,k,typ)
//return the vector of number of link connected to a given block
//   scs_m      :   structure of blocks and links
//   k          :   block_number
//   typ        :   'in','out','clkin','clkout'
//   connected  :   vector of connected link numbers
// Copyright INRIA
[lhs,rhs]=argn(0)
o=scs_m(k)
graphics=o(2)
[ip,op,cip,cop]=graphics(5:8)
connected=[]
if rhs<=2 then // all connected links
  if ip<>[] then connected=[connected ip(find(ip>0))'],end
  if op<>[] then connected=[connected op(find(op>0))'],end
  if cip<>[] then connected=[connected cip(find(cip>0))'],end
  if cop<>[] then connected=[connected cop(find(cop>0))'],end
else
  if typ=='in' then connected=[connected ip(find(ip>0))'],end
  if typ=='out' then connected=[connected op(find(op>0))'],end
  if typ=='clkin' then connected=[connected cip(find(cip>0))'],end
  if typ=='clkout' then connected=[connected cop(find(cop>0))'],end
end




