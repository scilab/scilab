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

function %model_p(model)

    sim=model.sim
    if type(sim)==15 then
        if type(sim(1))==13 then
            txt="scifunc"
        else
            txt=sim(1)+" type: "+string(sim(2))
        end
    else
        txt=sim+" type: 0"
    end
    mprintf("%s\n", txt)

    fn=getfield(1,model)

    if size(fn, "*") > 4 then // Rule out the Annotations
        if or(fn == "rpar") && typeof(model.rpar) == "diagram" then // Do nothing if model("rpar") is already a mlist
            rpar = diagram2mlist(model.rpar);
            // Define a new model omitting 'rpar' because writing 'model.rpar=l' triggers cloning.
            newModel = scicos_model( sim=model.sim,in=model.in,in2=model.in2,intyp=model.intyp,out=model.out,out2=model.out2,outtyp=model.outtyp,evtin=model.evtin,evtout=model.evtout,state=model.state,dstate=model.dstate,odstate=model.odstate,ipar=model.ipar,opar=model.opar,blocktype=model.blocktype,firing=model.firing,dep_ut=model.dep_ut,label=model.label,nzcross=model.nzcross,nmode=model.nmode,equations=model.equations,uid=model.uid );
            newModel.rpar = rpar;

            for i=1:size(newModel.rpar.objs)
                newModelObj = newModel.rpar.objs(i);
                if typeof(newModelObj) == "Block" && typeof(newModelObj.model.rpar) == "diagram"
                    subRpar = diagram2mlist(newModelObj.model.rpar);
                    // Define a new model omitting 'rpar' because writing 'model.rpar=l' triggers cloning.
                    newSubModel = scicos_model( sim=newModel.sim,in=newModel.in,in2=newModel.in2,intyp=newModel.intyp,out=newModel.out,out2=newModel.out2,outtyp=newModel.outtyp,evtin=newModel.evtin,evtout=newModel.evtout,state=newModel.state,dstate=newModel.dstate,odstate=newModel.odstate,ipar=newModel.ipar,opar=newModel.opar,blocktype=newModel.blocktype,firing=newModel.firing,dep_ut=newModel.dep_ut,label=newModel.label,nzcross=newModel.nzcross,nmode=newModel.nmode,equations=newModel.equations,uid=newModel.uid );
                    newSubModel.rpar = subRpar;
                    newModelObj.model = newSubModel;
                    newModel.rpar.objs(i) = newModelObj;
                end
            end

            for k=3:size(fn,"*")
                mprintf("%s\n", sci2exp(newModel(fn(k)),fn(k)))
            end
            return
        end
    end
    for k=3:size(fn,"*")
        mprintf("%s\n", sci2exp(eval("model."+fn(k)),fn(k)))
    end
endfunction

function ml = diagram2mlist(d)
    ml = mlist(["diagram", "props", "objs", "version", "contrib"], d.props, [], d.version, d.contrib);
    // Add 'objs' later to prevent cloning
    ml.objs = d.objs;

    // Also convert the sub-blocks and sub-links
    listObjs=list(ones(1, size(ml.objs)));
    for k=1:size(ml.objs)
        select typeof(ml.objs(k))
        case "Block" then
            b = ml.objs(k);
            listObjs(k) = mlist(["Block", "graphics", "model", "gui", "doc"], b.graphics, b.model, b.gui, b.doc);
        case "Link" then
            l = ml.objs(k);
            listObjs(k) = mlist(["Link", "xx", "yy", "id", "thick", "ct", "from", "to"], l.xx, l.yy, l.id, l.thick, l.ct, l.from, l.to);
        case "Annotation" then
            t = ml.objs(k);
            listObjs(k) = mlist(["Annotation", "graphics", "model", "void", "gui"], t.graphics, t.model, t.void, t.gui);
        case "Deleted" then
            listObjs(k) = mlist(["Deleted"]);
        else
            error(msprintf(_("Wrong type for diagram element #%d: %s, %s or %s expected.\n"), k, "Block", "Link", "Annotation"));
        end
    end
    ml.objs = listObjs;
endfunction
