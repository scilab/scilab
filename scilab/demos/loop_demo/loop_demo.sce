cd(SCI+'/demos/loop_demo/loop_demo_eng');
if MSDOS then
  while %t
    unix_g(SCI+'/bin/Wscilex -ng -f loop.sce');
  end 
else
  while %t
    unix_g(SCI+'/bin/scilex -ng -f loop.sce');
  end
end