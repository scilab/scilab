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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function tree_show(x,titletop)

    if argn(2) < 1 then
        error(sprintf(_("%s: Wrong number of input argument(s): %d to %d expected.\n"), "tree_show", 1, 2));
    end

    if type(x)<>16 & type(x)<>17 & type(x)<>15 & type(x)<>128 then
        error("Wrong type; input must be a list or an Xcos object.")
    end

    //-- tt = ["set BWpath [file dirname '"$env(SCIPATH)/modules/tclsci/tcl/BWidget-1.8.0'"] "
    //--       "if {[lsearch $auto_path $BWpath]==-1} { set auto_path [linsert $auto_path 0 $BWpath] }"
    //--       "package require BWidget 1.8.0"
    //-- //      'proc ppx {label} {global xmind; set xmind $label;ScilabEval '"%tcl_par=''1'''"}'
    //-- //      'proc qqx {label} {global xmind; set xmind $label;ScilabEval '"%tcl_par=''3'''"}'
    //--       'catch {destroy .ss}'
    //--       'toplevel .ss'
    //--       'scrollbar .ss.ysb -command {.ss.t yview}'
    //--       'scrollbar .ss.xsb -command {.ss.t xview} -orient horizontal'
    //--       'Tree .ss.t -xscrollcommand '".ss.xsb set'" -yscrollcommand '".ss.ysb set'" "+...
    //--       " -width 30'
    //--       'grid .ss.t .ss.ysb -sticky nsew'
    //--       ' grid .ss.xsb -sticky ew'
    //--       ' grid rowconfig    .ss 0 -weight 1'
    //--       ' grid columnconfig .ss 0 -weight 1'
    //--      ];

    //-- for i=1:size(tt,1)
    //--   TCL_EvalStr(tt(i))
    //-- end


    //-- if argn(2)>1 then
    //--    tt = 'wm title .ss {'+titletop+'}';
    //-- elseif type(x)<>15 then
    //--    v = getfield(1,x);
    //--    tt = 'wm title .ss '+v(1);
    //-- else
    //--    tt = 'wm title .ss list';
    //-- end

    //-- TCL_EvalStr(tt)
    Path = "root"
    tree = uiCreateNode("Root");
    tree = crlist3(x,Path, tree);
    uiDisplayTree(tree);

    //-- TCL_EvalStr(' .ss.t bindText <Double-1> {ppx}')
    //-- TCL_EvalStr(' .ss.t bindText <3> {qqx}')
endfunction


function java = crlist3(x,Path, java)
    if type(x)==15 then
        II=1:size(x);v=string(II);
    else
        v=getfield(1,x);
        if type(x)==17 & v(1)=="st" then
            II=3:size(v,"*");
        else
            II=2:size(v,"*");
        end
    end
    for i=II
        path=Path+","+string(i)
        titre=v(i);
        o=getfield(i,x);
        if type(o)==16 | type(o)==17 | type(o)==128 then
            w=getfield(1,o);
            titre2=titre+" ("+w(1)+")";
            //-- 	TCL_EvalStr('.ss.t insert end '+Path+' '+path+' -image [Bitmap::get folder] -text {'+titre2+'}')
            currentNode = uiCreateNode(titre2);
            currentNode = crlist3(o,path,currentNode); //* recursive
            java = uiConcatTree(java, currentNode);

        elseif type(o)==15 then
            titre2=titre;
            //-- 	TCL_EvalStr('.ss.t insert end '+Path+' '+path+' -image [Bitmap::get folder] -text {'+titre2+'}')
            currentNode = uiCreateNode(titre2);
            currentNode = crlist3(o,path,currentNode); //* recursive
            java = uiConcatTree(java, currentNode);
        else
            try
                if size(o, "*") > 40 then
                    tts = typeof(o) + " of size " + sci2exp(size(o))
                else
                    tts = sprintf("%s", sci2exp(o))
                end,
            catch
                tts = typeof(o);
            end
            titre2=titre+": "+tts  ;
            //--       TCL_EvalStr('set yy {'+titre2+'}')
            //--       TCL_EvalStr('.ss.t insert end '+Path+' '+path+' -text $yy')
            java = uiConcatTree(java, uiCreateNode(titre2));
        end
    end

endfunction

