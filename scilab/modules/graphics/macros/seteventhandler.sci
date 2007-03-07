function seteventhandler(name)
// Copyright INRIA 2007
// seteventhandler
// specify the name of the eventhandler for current window
// Author: Jean-Baptiste Silvy

[lhs,rhs] = argn(0) ;

if ( rhs <> 1 ) then
  error( 39 ) ;
  return ;
end

fig = get("current_figure") ;
fig.event_handler = name ;
if ( name == "" ) then
  fig.event_handler_enable = "off" ;
else
  fig.event_handler_enable = "on" ;
end


endfunction
