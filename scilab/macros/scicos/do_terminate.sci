function do_terminate(scs_m,%cpr,%tcur)
// Copyright INRIA
if prod(size(%cpr))<2 then return,end
par=scs_m(1);
if alreadyran then
  //terminate current simulation
  errcatch(888,'continue')
    win=xget('window')
  [state,t]=scicosim(%cpr(1),%tcur,par(4),%cpr(2),'finish',par(3))
  xset('window',win)
  errcatch(-1)
  if iserror(888)==1 then
    errclear(888)
    message('End problem, see message in scilab window')
  end
end
