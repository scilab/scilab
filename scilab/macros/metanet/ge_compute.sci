function r=ge_compute(kmen,win)
//Copyright INRIA
//Author : Serge Steer 2002
  w=string(win)

  mens=["Shortest path"]

  r=%f

  select mens(kmen)
  case "Shortest path" then  
    old=xget('window');xset('window',win);seteventhandler("")  
    ge_do_shortestpath()
    seteventhandler("ge_eventhandler") ;xset('window',old)
  end
endfunction
