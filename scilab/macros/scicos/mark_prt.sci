function o=mark_prt(o,prt_number,inout,typ,v)
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

// Copyright INRIA

graphics=o(2),
if inout=='out' then //set an output port
  if typ==1 then  //standard port
    o(2)(6)(prt_number)=v;
else //clock port
  o(2)(8)(prt_number)=v;
  end
else //set an input port
  if typ==1 then  //standard port
    o(2)(5)(prt_number)=v;
  else //clock port
    o(2)(7)(prt_number)=v;
  end
end



