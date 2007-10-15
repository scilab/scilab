function o=disconnect_ports(o)
// Copyright INRIA
  graphics=o.graphics
  graphics.pin=0*graphics.pin
  graphics.pout=0*graphics.pout
  graphics.pein=0*graphics.pein
  graphics.peout=0*graphics.peout
  o.graphics=graphics
endfunction
