function [scs_m,cpr,needcompile,ok]=do_eval(scs_m,cpr)
// This function (re)-evaluates blocks in the scicos data structure scs_m 
// Copyright INRIA
ok=%t
needcompile1=max(2,needcompile)
%mprt=funcprot()
funcprot(0) 
getvalue=setvalue;
deff('[ok,tt]=FORTR(funam,tt,i,o)','ok=%t')
deff('[ok,tt]=CFORTR(funam,tt,i,o)','ok=%t')
deff('[x,y,ok,gc]=edit_curv(x,y,job,tit,gc)','ok=%t')
deff('[ok,tt,dep_ut]=genfunc1(tt,ni,no,nci,nco,nx,nz,nrp,type_)',..
    'dep_ut=model(12);ok=%t')
funcprot(%mprt)
%nx=size(scs_m)



for %kk=2:%nx
  o=scs_m(%kk)
  if o(1)=='Block' then
    model=o(3)
    if model(1)=='super'|model(1)=='csuper' then
      sblock=model(8)
      [sblock,%w,needcompile2,ok]=do_eval(sblock,list())
      needcompile1=max(needcompile1,needcompile2)
      if ok then
	scs_m(%kk)(3)(8)=sblock
      end
    else
      model=o(3)
      execstr('o='+o(5)+'(''set'',o)')
      needcompile1=max(needcompile1,needcompile) // for scifunc_block
      model_n=o(3)
      if or(model(10)<>model_n(10))|.. // type 'c','d','z','l'
	        or(model(12)<>model_n(12)) then 
	needcompile1=4
      end
      scs_m(%kk)=o
    end
  end
end
needcompile=needcompile1
if needcompile==4 then cpr=list(),end


