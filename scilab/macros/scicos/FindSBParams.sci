function [params,param_types]=FindSBParams(scs_m,params)
// Copyright INRIA
  prot = funcprot();
  funcprot(0);
  deff('varargout=getvalue(a,b,c,d)',..
    ['global par_types'
     'par_types=c'
     'x=1;y=x(2)'])
  funcprot(prot); 
      
  global par_types

  Fun=scs_m.props.context;
  for i=1:size(scs_m.objs)
    o=scs_m.objs(i);
    if typeof(o)=='Block' then
      model=o.model;
      if model.sim=='super'|model.sim=='csuper' then
        Funi='['+FindSBParams(model.rpar,params)+']'
      else
        if typeof(o.graphics.exprs)=="MBLOCK" then //modelica block
          Funi=[];
          for j=1:lstsize(o.graphics.exprs.paramv)
             Funi=[Funi;
                   '['+o.graphics.exprs.paramv(j)+']'];
          end
        else
          if type(o.graphics.exprs)==15 then
            Funi='['+o.graphics.exprs(1)(:)+']';
          else
            Funi='['+o.graphics.exprs(:)+']';
          end
          par_types=[];
          execstr('blk='+o.gui+'(''define'')')
          execstr(o.gui+'(''set'',blk)','errcatch')

          Del=[];kk=1;
          for jj=1:2:length(par_types)
            if par_types(jj)=='str' then Del=[Del,kk],end
            kk=kk+1
          end
          Funi(Del)=[]
        end
      end
      Fun=[Fun;Funi]
    end
  end
  deff('%Font3()',Fun)
  xx=macrovar(%Font3);
  params=xx(3)
  param_types=list()
  for X=params'
    select evstr('type('+X+')')
    case 1
      param_types($+1)='pol'
      param_types($+1)=-1
    case 2
      param_types($+1)='pol'
      param_types($+1)=-1
    case 8
      param_types($+1)='mat'
      param_types($+1)=[-1,-1]
    case 15
      param_types($+1)='lis'
      param_types($+1)=-1
    case 16
      param_types($+1)='lis'
      param_types($+1)=-1
    case 17
      param_types($+1)='lis'
      param_types($+1)=-1
    end
  end
//  clearglobal('par_types')  //recursive call, so it cannot be cleared here
endfunction

