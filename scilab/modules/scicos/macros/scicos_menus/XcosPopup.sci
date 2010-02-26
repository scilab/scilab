function Cmenu=XcosPopup(ptree)
  main = uicontextmenu();
  for Id=ptree
    if type(Id)==10 then //a leaf
      Label=XcosGetMenuLabel(Id,%scicos_menu)
      uimenu("Parent",main, "label", Label,'Callback','global XcosPop;XcosPop='+sci2exp(Id));
    else //a sub menu
      XcosSubPopup(main,Id)
    end
  end
  if argn(2)<2 then 
    uiwait(main);
    global XcosPop;
    Cmenu=XcosPop
    clearglobal XcosPop
  end
endfunction

function XcosSubPopup(h,tree)
  Id=tree(1)
  Label=XcosGetMenuLabel(Id,%scicos_menu)
  if type(Label)<>10 then pause,end
  main=uimenu("Parent",h, "label", Label);
  for k=2:length(tree)
    Id=tree(k)
    if type(Id)==10 then      
      Label=XcosGetMenuLabel(Id,%scicos_menu)
      uimenu("Parent",main, "label", Label,'Callback','global XcosPop;XcosPop='+sci2exp(Id));
    else
      XcosSubPopup(main,Id)
    end
  end
endfunction
