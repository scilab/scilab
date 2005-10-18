function [btn,%pt,win,Cmenu]=cosclick(flag)
  [lhs,rhs] = argn(0)
  btn=0 // not used anyway
  Cmenu_orig = Cmenu
  Cmenu=[]; %pt=[]; //** Clear the variables
  if ~or(winsid()==curwin) then  win=xget('window');Cmenu='Quit',return,end   
  
  if rhs==1 then
    [btn,xc,yc,win,str]=xclick(flag)
  else
    [btn,xc,yc,win,str]=xclick()
  end

  %pt=[xc,yc]
  //** -----------------------------------------------------------
  if or(btn==[2 5]) then // button 2 (right) pressed or clicked
    Cmenu='Popup',return
    
    //**------------------------------------------------------------
  elseif btn==-100 then  // The window has been closed 
    if win==curwin then
      Cmenu='Quit',
    else
      Cmenu=[]
      %pt=[]
    end
    return
    
    //**-------------------------------------------------------------    
  elseif btn==-2 then  			// click in a dynamic menu
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
    
    //**-------------------------------------------------------------    
  elseif (btn==10) & (win<>curwin) then //** button event (in a palette or navigator)
    jj=find(windows(:,2)==win)
    if jj <> [] then
      if or(windows(jj,1)==100000) then
	Cmenu='Open/Set'  //mode open-set (cliquer dans navigator)
      else
	Cmenu='Duplicate'
      end
    else
      Cmenu=[];%pt=[];
    end
    
    //**-------------------------------------------------------------    
  elseif (btn==10) & (win==curwin) then
    Cmenu='Open/Set'
  elseif btn==3 then
    Cmenu='SelectLink'
  elseif  (btn==0) then
    Cmenu='MoveLink'
    
    //** -----------------------------------------------------------
    //** Single key Shortcut 
  elseif btn>31 then
    if btn==1120 then 
      Cmenu='Cut';%pt=[];
    elseif btn==1099 then
       Cmenu='Copy';%pt=[];
    elseif btn==1118 then
       Cmenu='Paste';%pt=[];
    elseif btn==65288|btn==255 then
      Cmenu='Delete';%pt=[];
    else
      Cmenu=%tableau(min(100,btn-31));
      if Cmenu==emptystr() then Cmenu=[];%pt=[];end
    end
  end
endfunction


