function needreplay = replayifnecessary()
// Copyright INRIA

///** This is basically a dummy function (will be eliminated very soon)

  if needreplay & slevel==1 then //** 
     // xclear();
     // xtape('replay',xget('window'))
     needreplay = %f
  end

endfunction
