function ge_do_zoom()
//Copyright INRIA
//Author : Serge Steer 2002

   Z=[12.5 25:25:400]

   c=x_choose(string(Z),'Choose a a zoom factor')
   if c==0 then return,end
   
   execstr('global EGdata_'+w+';EGdata=EGdata_'+w)
   EGdata.Zoom=Z(c)/100;
   EGdata.Edited=%t
   GraphList=EGdata.GraphList
   
   execstr('EGdata_'+w+'=EGdata')
   
   xbasc()
   ge_set_winsize()
   ge_drawobjs(GraphList),
   
   

endfunction
