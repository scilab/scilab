function ok=do_fsave(scs_m)
//
// Copyright INRIA
scs_m=do_purge(scs_m)
ok=%f
fname=xgetfile('*.cosf')
if fname==emptystr() then return,end

[path,name,ext]=splitfilepath(fname)
select ext
case 'cosf' then
  ok=%t
else
  message('Only *.cosf  files allowed');
  ok=%f
  return
end

// Change the name in the structure
scs_m(1)(2)=[name,path]

//open the file
errcatch(240,'continue','nomessage')
[u,i]=file('open',fname,'unknown')
errcatch(-1)
if iserror(240)==1 then
  message('Directory write access denied')
  errclear(240)
  ok=%f
  return
end

//save 
errcatch(49,'continue','nomessage')
write(u,sci2exp(scicos_ver,'scicos_ver'),'(a)')
errcatch(-1)
if iserror(49)==1 then
  message('File write access denied')
  errclear(49)
  ok=%f
  file('close',u)
  return
end
disablemenus()
cos2cosf(u,scs_m)
file('close',u)
enablemenus()
ok=%t
if pal_mode then update_scicos_pal(path,scs_m(1)(2)(1),fname),end
    



