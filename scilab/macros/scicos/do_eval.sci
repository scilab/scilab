function [scs_m,cpr,needcompile,ok]=do_eval(scs_m,cpr)
// This function (re)-evaluates blocks in the scicos data structure scs_m 
// Copyright INRIA
ok=%t
needcompile1=max(2,needcompile)
%mprt=funcprot()
funcprot(0) 
getvalue=setvalue;

deff('message(txt)','x_message(''In block ''+o.gui+'': ''+txt);%scicos_prob=resume(%t)')

%scicos_prob=%f
deff('[ok,tt]=FORTR(funam,tt,i,o)','ok=%t')
deff('[ok,tt]=CFORTR(funam,tt,i,o)','ok=%t')
deff('[x,y,ok,gc]=edit_curv(x,y,job,tit,gc)','ok=%t')
deff('[ok,tt,dep_ut]=genfunc1(tt,ni,no,nci,nco,nx,nz,nrp,type_)',..
    'dep_ut=model.dep_ut;ok=%t')
funcprot(%mprt)
%nx=lstsize(scs_m.objs)



for %kk=1:%nx
  o=scs_m.objs(%kk)
  if typeof(o)=='Block' then
    model=o.model
    if model.sim=='super'|model.sim=='csuper' then
      sblock=model.rpar
      context=sblock.props.context
      if execstr(context,'errcatch')<>0 then
	%now_win=xget('window')
        message(['Cannot evaluate a context';lasterror()])
	xset('window',%now_win)
      else
        [sblock,%w,needcompile2,ok]=do_eval(sblock,list())
        needcompile1=max(needcompile1,needcompile2)
        if ok then
          scs_m.objs(%kk).model.rpar=sblock
        end
      end        
    else
      model=o.model
      execstr('o='+o.gui+'(''set'',o)')
      needcompile1=max(needcompile1,needcompile) // for scifunc_block
      model_n=o.model
      if or(model.blocktype<>model_n.blocktype)|.. // type 'c','d','z','l'
	        or(model.dep_ut<>model_n.dep_ut) then 
	needcompile1=4
      end
      scs_m.objs(%kk)=o
    end
  end
end
needcompile=needcompile1
if needcompile==4 then cpr=list(),end
endfunction


