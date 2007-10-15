function [%scs_wgrid]=do_grid(%scs_wgrid)
// Copyright INRIA
 exprs = [string(%scs_wgrid(1)),string(%scs_wgrid(2)),string(%scs_wgrid(3))]
 while %t do
    [ok,b1,b2,colorr,exprs]=getvalue(['Set Grid'],..
	    ['x','y','color'],list('vec',1,'vec',1,'vec',1),exprs)
    if ~ok then
     break
    else
     %scs_wgrid(1)=b1
     %scs_wgrid(2)=b2
     %scs_wgrid(3)=colorr
     drawgrid();
     gh_window=gcf(%win)
     swap_handles(gh_window.children.children($),...
                gh_window.children.children(1));
     delete(gh_window.children.children(1));
     drawnow();
     show_pixmap();
     break
    end
  end
endfunction
