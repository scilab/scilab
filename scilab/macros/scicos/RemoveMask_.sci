function RemoveMask_()
// Copyright INRIA
Cmenu=[];%pt=[];
if size(Select,1)<>1 | curwin<>Select(1,2) then
   return
end
i=Select(1)
o=scs_m.objs(i)
if typeof(o)=='Block' then
   if o.model.sim=='csuper' & o.model.ipar==1 then  
      o.model.sim='super'
      o.model.ipar=[] 
      o.gui='SUPER_f'
      o.graphics.exprs=[]      
      scs_m_save = scs_m    ;
      scs_m.objs(i)=o;
      nc_save = needcompile ;
      needcompile=4  // this is perhaps too conservative
      enable_undo = %t
      edited=%t
   else
      message('This block is not masked.')
   end
else
  message('Select a block.')
end
endfunction
