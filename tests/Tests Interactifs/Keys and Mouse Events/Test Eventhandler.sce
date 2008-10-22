function myeventhandler(win,x,y,ibut)
 xinfo(msprintf('window: %d, x:%d, y:%d, button: %d',win,x,y,ibut))
endfunction

seteventhandler('myeventhandler')
