function scicos_pal=update_scicos_pal(path,name,fname)
// Copyright INRIA
scicos_pal;

inde=find(scicos_pal(:,1)==name);
if size(inde,'*')>=2 then 
  message(['More than one palette named '+name;
  'This is not allowed, do an Pal Editor to correct'])
  return
end

if inde<>[] then
  javab=message(['The palette '+name+' already exists';
      'Do you want to replace it?'],['Yes','No'])
  if javab==2 then 
    return;
  else
    scicos_pal(inde,2)=fname
    errcatch(-1,'continue')
    if MSDOS then 
      unix_s('del '+TMPDIR+'\'+name+'.pal')
    else
      unix_s('\rm -f '+TMPDIR+'/'+name+'.pal')
    end
    errcatch(-1)
    if iserror(-1)==1 then
      errclear(-1)
      x_message(['I was not able to delete '+name+'.pal';
      'in '+TMPDIR+'. You must do it now!'])
    end
  end
else
  scicos_pal=[scicos_pal;[name,fname]]
end
errcatch(-1,'continue')
save('.scicos_pal',scicos_pal)
errcatch(-1)
if iserror(-1)==1 then
  errclear(-1)
  x_message(['I was not able to write in .scicos_pal'])
end

