function o=mark_prt(o,prt_number,inout,typ,v)
// Copyright INRIA
//mark a port of a block free or used
// o           : block data structure
// prt_number  : port number
// inout       : port orientation ('in' or 'out')
// typ         : port type
//               1  : standard port
//              -1  : event port
// v           : value to assign
//               k>0 : port is connected to link #k
//               0 : port is free

  if inout=='out' then //set an output port
    if typ==1 | typ==2 then  //standard (regular or implicit) port
      o.graphics.pout(prt_number)=v;
    else //clock port
      o.graphics.peout(prt_number)=v;
    end
  else //set an input port
    if typ==1 | typ==2 then  //standard (regular or implicit) port
      o.graphics.pin(prt_number)=v;
    else //clock port
      o.graphics.pein(prt_number)=v;
    end
  end
endfunction
