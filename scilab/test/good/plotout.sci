function plotout(zoom)
  global count
  if count==[] then count=0,end
  if argn(2)<1 then
    zoom=1/2
  end

  count=count+1
  DATA_BEGIN=ascii(2);
  DATA_END=ascii(5)
  win=xget('window')
  output='/tmp/foo'+string(count)
  opts='""'+[output+'.eps' '*'+string(zoom) '*'+string(zoom) '' '' '' '']+'""';

  xs2ps(win,output) 

  unix_s(SCI+'/bin/BEpsf -portrait '+output)

  //[DATA_BEGIN]scheme:(postscript "/tmp/foo.eps" "*1/2" "*1/2" "" "" "" "")[DATA_END]

  write(%io(2),DATA_BEGIN+'scheme:(postscript '+strcat(opts,' ')+')'+ ...
	DATA_END,'(a)')

endfunction
