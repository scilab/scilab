function CreateMask_()
// Copyright INRIA
Cmenu=[];%pt=[];
if size(Select,1)<>1 | curwin<>Select(1,2) then
   return
end
i=Select(1)
o=scs_m.objs(i)
if typeof(o)=='Block' then
   model=o.model
   graphics=o.graphics;
   if model.sim=='super' then  //
      [params,param_types]=FindSBParams(model.rpar,[])
      bname=model.rpar.props.title(1)
      model.sim='csuper'
      model.ipar=1 ;  // specifies the type of csuper (mask)
      graphics.exprs=list(params,list(params,..
                    ["Set block parameters";params],param_types));     
      graphics.gr_i=list('xstringb(orig(1),orig(2),'"'+..
        bname+''",sz(1),sz(2),''fill'');',8)
      o.model=model;
      o.graphics=graphics;
      o.gui='DSUPER';
      scs_m_save = scs_m    ;
      scs_m.objs(i)=o;
      nc_save = needcompile ;
      needcompile=4  // this is perhaps too conservative
      enable_undo = %t
      edited=%t
      o_size = size(gh_current_window.children.children);
      gh_k=get_gri(i,o_size(1))
      drawlater();
      update_gr(gh_k,o)
      draw(gh_current_window.children);
      show_pixmap() ;
      //Cmenu='Replot'
   else
      message('Mask can only be created for Super Blocks.')
   end
else
  message('Mask can only be created for Super Blocks.')
end
endfunction
