function connected=get_connected(scs_m,k,typ)
//return the vector of number of link connected to a given block
//   scs_m      :   structure of blocks and links
//   k          :   block_number
//   typ        :   'in','out','clkin','clkout'
//   connected  :   vector of connected link numbers
// Copyright INRIA
  [lhs,rhs]=argn(0)

  connected=[]

//disp('get_connected')
// pause
  if rhs<=2 then // all connected links
    graphics=scs_m.objs(k).graphics
    
    ip=graphics.pin
    connected=[connected ip(find(ip>0))']
    
    op=graphics.pout
    connected=[connected op(find(op>0))']
    
    cip=graphics.pein
    connected=[connected cip(find(cip>0))']
    
    cop=graphics.peout
    connected=[connected cop(find(cop>0))']
  else
    
    if typ=='in' then 
      ip=scs_m.objs(k).graphics.pin
      connected=[connected ip(find(ip>0))'],
    elseif typ=='out' then 
      op=scs_m.objs(k).graphics.pout
      connected=[connected op(find(op>0))'],
    elseif typ=='clkin' then 
      cip=scs_m.objs(k).graphics.pein
      connected=[connected cip(find(cip>0))'],
    elseif typ=='clkout' then 
      cop=scs_m.objs(k).graphics.peout
      connected=[connected cop(find(cop>0))'],
    end
  end

endfunction
