// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function  datatipContextMenu(ax)
//datatip utility function
  f=gcf();

  curve_handles=datatipGetEntities(ax);
  [curve,dmin,ptmin,l]=datatipGetNearestEntity(pt,curve_handles)
  if curve<>[]&dmin<10 then
    ud=datatipGetStruct(curve);
    items=[_("Delete all datatips")
           _("Delete all datatips for the selected curve")
           _("Delete nearest datatip")
           _("Delete last datatip")
           _("Delete selected datatip")
           _("Open curve tip style editor")
           _("Redraw all datatips")];
  else
    items=[_("Delete all datatips");
           _("Delete selected datatip")
           _("Redraw all datatips")];
  end
  sel=x_choose(items,'');
  if sel>0 then
    select items(sel)
    case _("Delete all datatips") then
      datatipRemoveAll(curve_handles);
    case _("Delete all datatips for the selected curve") then;
      datatipRemoveAll(curve);
    case _("Delete nearest datatip") then
      datatipRemoveNearest(curve,pt);
    case _("Delete last datatip") then
      //l=size(ud.tips,'*');
      l=size(ud.tips.children,'*');
      if l<>0 then
        datatipRemove(curve,1)
      end
    case _("Delete selected datatip") then
      datatipDeleteSelected(curve_handles)
    case _("Edit the curve tip display function") then
      datatipSetDisplay(curve);
    case _("select the curve tip display function")  then
      datatipSelectFunction(curve);
    case _("Enable interpolation") then
      datatipSetInterp(curve,%T);
    case _("Disable interpolation") then
      datatipSetInterp(curve,%F);
    case _("Multiple datatips mode") then
      datatipSetReplaceMode(curve,%F);
    case _("Unique datatip mode") then
      datatipSetReplaceMode(curve,%t);
    case _("Redraw all datatips") then
      datatipRedraw(curve_handles);
    case _("Open curve tip style editor")  then
      datatipsGUI(curve)
      scf(f);
      //datatipSetStyle(curve);
    end
  end
endfunction
