function EGdata=ge_do_zoom(EGdata)
//Copyright INRIA
//Author : Serge Steer 2002

   Z=[12.5 25:25:400]
   c=x_choose(['Other value',string(Z)+'%'],'Choose a a zoom factor')
   if c==0 then return,end
   c=c-1;
   if c==0 then 
     cl=string(100*EGdata.Zoom)
     while c<=0 then
       [ok,c,cl]=getvalue('Enter a zoom factor (%)','Factor',list('vec',1),cl)
       if ~ok then return,end
     end
   else
     c=Z(c)
   end
   
   EGdata.Zoom=c/100;
   EGdata.Edited=%t
   ge_do_replot(EGdata.GraphList)
endfunction
