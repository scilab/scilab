cd(SCI+'/modules/others/demos/loop_demo');
if MSDOS then
  while %t
    unix_g(SCI+'/bin/Wscilex -nb -f loop.sce');
  end 
else
  while %t
    unix_g(SCI+'/bin/scilex -nb -f loop.sce');
  end
end