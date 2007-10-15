function CustomizeMask_()
// Copyright INRIA
Cmenu=[];%pt=[];
if size(Select,1)<>1 | curwin<>Select(1,2) then
   return
end
i=Select(1)
o=scs_m.objs(i)
if typeof(o)=='Block' then
   if o.model.sim=='csuper' & o.model.ipar==1 then 
      items=o.graphics.exprs(2)(1)
      result=tk_mdialog(['Customize block GUI:';'Modify title and menu labels.'],..
      ['Title of the GUI';items],[o.graphics.exprs(2)(2);items])

      if result<>[] & ~isequal(items,result) then
         o.graphics.exprs(2)(2)=result;
         scs_m.objs(i)=o;
         edited=%t
      end
   else
      message('This block is not masked.')
   end
else
  message('Select a block.')
end
endfunction
