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
dr=driver()
driver('Pos')
output='/tmp/foo'+string(count)
xinit(output)
xtape('replay',win)
xend()
driver(dr)
unix_s(SCI+'/bin/BEpsf -portrait '+output)
opts='""'+[output+'.eps' '*'+string(zoom) '*'+string(zoom) '' '' '' '']+'""';

//[DATA_BEGIN]scheme:(postscript "/tmp/foo.eps" "*1/2" "*1/2" "" "" "" "")[DATA_END]

write(%io(2),DATA_BEGIN+'scheme:(postscript '+strcat(opts,' ')+')'+DATA_END,'(a)')
endfunction
