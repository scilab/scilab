function %xload(fil,num)
  if  argn(2)==2 then xset("window",num),end
  if get('figure_style')=='old' then set('figure_style','new'),end
  f=gcf()
  xload_mode=%t
  res = execstr('load(fil)','errcatch') ;
  if res==0 then
    f.visible='on'
    f.immediate_drawing='on';
  else 
    //test if it is really an old style graphic save file
    ufil=mopen(fil,'rb')
    pos=mtell(ufil)
    n1=mget(1,'uib',ufil)
    if n1<>8 then 
      mclose(ufil),error('Given file is not a graphic one');
    end
    mseek(pos,ufil,'set')
    SciF_version=LoadVectC()
    if part(SciF_version,1:4)<>"SciG" then 
      mclose(ufil),error('Given file is not a graphic one'),
    end
    mclose(ufil),
    set('figure_style','old')
    xload(fil)
  end
//  draw(f)

endfunction
function str=LoadVectC()
// Copyright INRIA
// Author: Serge Steer

// Emulate the C procedure in routines/graphics/RecLoad.c
  
  n1=mget(1,'uib',ufil)
  //q=int(n1/4);r=n1-4*q
  n=n1
  if modulo(n1,4)<>0 then n=4*(int(n1/4)+1),end
  c=mget(n,'uc',ufil)
  if n1-1>size(c) then pause,end
  str=ascii(c(1:n1-1))
endfunction
