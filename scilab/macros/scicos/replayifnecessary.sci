function needreplay=replayifnecessary()
if needreplay&slevel==1 then
  xclear();xtape('replay',xget('window'))
  needreplay=%f
end
endfunction
