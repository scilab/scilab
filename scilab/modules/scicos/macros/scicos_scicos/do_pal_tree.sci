//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//


function scs_m = do_pal_tree(scicos_pal)

  //scs_m = scicos_diagram(version=get_scicos_version());
  scs_m = get_new_scs_m();
  scs_m.props.title(1) = 'Palettes';
  sup = SUPER_f('define');

  for i=1:size(scicos_pal,1)
disp(scicos_pal(i,:))
    o = sup;
    o.model.rpar = charge(scicos_pal(i,:)); //** local functions:
                                            //** see below in this file
    scs_m.objs(i) = o;

  end

  tt = pal_TreeView(scs_m); //** local functions: see below in this file

 

endfunction
//**-----------------------------------------------------------------------

function scs_m = charge(pal)

  [ok,scs_m,cpr,edited]=do_load(pal(2),'palette')
  if ok & size(scs_m.objs)>0 then
    scs_m.props.title(1)=pal(1)
  else
    //scs_m= scicos_diagram(version=get_scicos_version())
    scs_m = get_new_scs_m();
    scs_m.props.title(1)='error loading '+pal(1)
  end
endfunction



function java = pal_TreeView(scs_m)

  Pgif = %scicos_gif; //** GIF path
  GIFT = listfiles(Pgif+'*.gif');
  GIFT = strsubst(GIFT,'\','/');
  GIF  = [];

  for i=1:size(GIFT,1)
    [jxpath,Gname,jext] = splitfilepath_cos(GIFT(i));
    GIF = [GIF;Gname];
  end

  Path = 'root'
  java = createNode("Palettes")
  java = crlist(scs_m, Path, java);
  displaytree(java);
endfunction

function java = crlist(scs_m, Path, java)
  blocks_to_remove=['CLKSPLIT_f' 'SPLIT_f' 'IMPSPLIT_f']

  for i=1:size(scs_m.objs)

    o=scs_m.objs(i);
    path=Path+','+string(i)

    if and(typeof(o)<>["Link" "Deleted"]) then
      //** Blocks and Super Blocks :)

      if (o.model.sim=='super'&(o.model.rpar.props.title(1)<>'Super Block')) |...
	    (o.gui=='PAL_f') then
        //** Super Blocks
	titre2 = o.model.rpar.props.title(1);
	//-- tt = [tt;'$wxx.t insert end '+Path+' '+path+' -image [Bitmap::get folder] -text '"'+titre2+''"']

	subTree = createNode(titre2, "default");
	//-- [subTree,tt] = crlist(o.model.rpar,path,tt,subTree);
	subTree = crlist(o.model.rpar, path, subTree);
	java = concatTree(java, subTree);

      else
        //** Standard Blocks
	if (find(o.gui==blocks_to_remove(:)))==[] then
	  titre2 = o.gui;
	  ind = find(titre2==GIF);
	  if ind<>[] then
	    //** a valid icon (GIF) is found
	    java = concatTree(java, createNode(titre2, GIFT(ind), "global %pa_;%pa_="""+path+""";Cmenu=''XcosMenuPlaceinDiagram''"));

	  else
	    //** NO icon is found: use the block's name
	    java = concatTree(java, createNode(titre2, "default", "global %pa_;%pa_="""+path+""";Cmenu=''XcosMenuPlaceinDiagram''"));

	  end
	end

      end //**... Blocks and Super Blocks

    end //**.. object filter

  end //**.. object loop

endfunction

