function ok=check_mac(txt)
//ok=%t,return
//errcatch doesnt work poperly
// Copyright INRIA
ok=%t
errcatch(-1,'continue')
comp(mac)
errcatch(-1)
if iserror(-1)==1 then
  errclear(-1)
  message('Incorrect syntax: see message in Scilab window')
  ok=%f
end

