function params=FindSBParams(scs_m,params)
  Fun=scs_m.props.context;
  for i=1:size(scs_m.objs)
    o=scs_m.objs(i)
    if typeof(o)=='Block' then
      model=o.model
      if model.sim=='super'|model.sim=='csuper' then
	Fun=[Fun;'['+FindSBParams(model.rpar,params)+']']
      else
        if type(model.sim)==15 then
          if model.sim(2)==30004 then //modelica block
            for j=1:lstsize(o.graphics.exprs.paramv)
              Fun=[Fun;'['+o.graphics.exprs.paramv(j)+']']
            end
          else
  	    Fun=[Fun;'['+o.graphics.exprs(:)+']']
          end
        else
	  Fun=[Fun;'['+o.graphics.exprs(:)+']']
        end
      end
    end
  end
  deff('%Font3()',Fun)
  xx=macrovar(%Font3);
  params=xx(3)
endfunction

