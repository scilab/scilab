function [btn,%pt,win,Cmenu]=cosclick(flag)
// Copyright INRIA POO
  [lhs,rhs]=argn(0)
  Cmenu_orig=Cmenu
  Cmenu=[];%pt=[];
  if ~or(winsid()==curwin) then  Cmenu='Quit',return,end    
  if rhs==1 then
    [btn,xc,yc,win,str]=xclick(flag)
  else
    [btn,xc,yc,win,str]=xclick()
  end
  %pt=[xc,yc]
  
  if btn==2 then
    if win ==curwin then
      [k,wh]=getobj(scs_m,[xc;yc])
      if k<>[] then
	j=1
	hilite_obj(scs_m.objs(k));
	xpause(300000)
	hilite_obj(scs_m.objs(k));
      else
	j=2
      end
    else
      j=3
    end

    Cmenu=mpopup(%scicos_lhb_list(j));

    if Cmenu==[] then %pt=[];end
    
  elseif btn==-100 then  
      if win==curwin then
	Cmenu='Quit',
      else
	Cmenu='Open/Set'
	%pt=[]
      end
      return
    
  elseif btn==-2 then

    // click in a dynamic menu
    win=curwin
    if strindex(str,'_'+string(curwin)+'(')<>[] then
      // click in a scicos dynamic menu
      %pt=[]
      execstr('Cmenu='+part(str,9:length(str)-1))
      execstr('Cmenu='+Cmenu)
      return
    else // click in an other dynamic menu
      execstr(str,'errcatch')
      return
    end
    
  elseif btn==0&(win<>curwin) then
    jj=find(windows(:,2)==win)
    if jj <> [] then
      if Cmenu_orig=='Copy Region' then
	Cmenu=[]
      else
	Cmenu='Copy' //btn=99  //mode copy
      end
      if or(windows(jj,1)==100000) then
	Cmenu='Open/Set'//btn=111  //mode open-set (cliquer dans navigator)
      end
    else
      %pt=[]
    end
  elseif btn>31 then
    Cmenu=%tableau(min(100,btn-31));
    if Cmenu==emptystr() then Cmenu=[];%pt=[];end
  end
//  disp(Cmenu)
  
endfunction


