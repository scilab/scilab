function r=ge_compute(kmen,win)
//Copyright INRIA
//Author : Serge Steer 2002
  w=string(win)
  mess=['Select the head and tail nodes']
  mens=["Shortest path"]

  r=%f
  xinfo(mess(kmen))
  select mens(kmen)
  case "Shortest path" then  
    old=xget('window');xset('window',win);seteventhandler("")  
    ge_do_shortestpath()
    seteventhandler("ge_eventhandler") ;xset('window',old)
  end
endfunction
