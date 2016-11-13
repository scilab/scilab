//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright (C) DIGITEO - 2010 - Jérôme PICARD
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

function [scs_m,cpr,needcompile,ok]=do_eval(scs_m,cpr,%scicos_context)
    // This function (re)-evaluates blocks in the scicos data structure scs_m

    ok=%t
    needcompile1=max(2,needcompile)
    %mprt=funcprot()
    funcprot(0)
    scicos_getvalue=setvalue;
    getvalue=setvalue;

    function scicosmessage(txt)
        messagebox(["In block " + o.gui + ": ";
        txt;
        "current parameter value kept"],"Warning","modal");
        %scicos_prob = resume(%t)
    endfunction

    // overload message
    // This function is used to alert the user on setvalue
    // (invalid exprs we keep the previous parameters)
    function message(str)
        if length(arg1.model.uid) >= 1 then
            uid = arg1.model.uid;
            uid = [full_uids uid];

            html = "<html><body>";
            html = html + "<em>" + gettext("Evaluation problem: value not updated from context.") + "</em><br/>";
            html = html + strcat(str, "<br/>") + "<br/>";
            html = html + "</body></html>";
            warnBlockByUID(uid, html);
        else
            txt = gettext("Evaluation problem: value not updated from context.");
            txt = [txt ; str];
            disp(txt);
        end

        %scicos_prob = resume(%t)
    endfunction

    // path on do_eval hierarchy loop
    if ~isdef("full_uids") then
        full_uids = [];
    end

    global %scicos_prob
    %scicos_prob=%f

    //## overload some functions used in GUI
    deff("[ok,tt]        =  FORTR(funam,tt,i,o)","ok=%t")
    deff("[ok,tt,cancel] =  CFORTR2(funam,tt,i,o)","ok=%t,cancel=%f")
    deff("[ok,tt,cancel] =  CC4(funam,tt,i,o)","ok=%t,cancel=%f")
    deff("[ok,tt]        =  CFORTR(funam,tt,i,o)","ok=%t")
    deff("[x,y,ok,gc]    =  edit_curv(x,y,job,tit,gc)","ok=%t")
    deff("[ok,tt,dep_ut] = genfunc1(tt,ni,no,nci,nco,nx,nz,nrp,type_)",..
    "dep_ut=model.dep_ut;ok=%t")
    deff("result         = dialog(labels,valueini)","result=valueini")
    deff("[result,Quit]  = scstxtedit(valueini,v2)","result=valueini,Quit=0")
    deff("[ok,tt]        = MODCOM(funam,tt,vinp,vout,vparam,vparamv,vpprop)",..
    "[dirF, nameF, extF] = fileparts(funam);"+..
    "[modelica_path, modelica_directory] = getModelicaPath();"+..
    "funam1 = []; "+..
    "if (extF == """")  then, "+..
    "    funam1 = modelica_directory + nameF + "".mo""; "+..
    "elseif fileinfo(funam) == [] then, "+..
    "    funam1 = funam; "+..
    "end; "+..
    "if funam1 <> [] then, "+..
    "    mputl(tt, funam1); "+..
    "end");
    %nx=lstsize(scs_m.objs)
    funcprot(%mprt)
    for %kk=1:%nx
        o=scs_m.objs(%kk)
        if typeof(o)=="Block" then
            model=o.model
            if model.sim=="super"| (model.sim=="csuper"& ~isequal(model.ipar,1)) then  //exclude mask
                sblock=model.rpar
                context=sblock.props.context
                [scicos_context1,ierr]=script2var(context,%scicos_context)
                if ierr <>0 then
                    %now_win=xget("window")
                    scicosmessage(["Cannot evaluate a context";lasterror()])
                    xset("window",%now_win)
                else
                    previous_full_uids = full_uids;
                    if length(o.model.uid) >= 1 then
                        full_uids = [full_uids o.model.uid];
                    end
                    [sblock,%w,needcompile2,ok]=do_eval(sblock,list(),scicos_context1)
                    needcompile1=max(needcompile1,needcompile2)
                    full_uids = previous_full_uids;
                    if ok then
                        scs_m.objs(%kk).model.rpar=sblock
                    else
                        return
                    end
                end
            elseif o.model.sim(1)=="asuper" then
            else
                model=o.model
                if ~isdef(o.gui) | ~or(type(evstr(o.gui) == [13 11])) then
                    if length(o.model.uid) >= 1 then
                        uid = [full_uids o.model.uid];

                        html = "<html><body>";
                        html = html + "<em>" + gettext("Evaluation problem: Unknown block") + "</em><br/>";
                        html = html + "</body></html>";
                        warnBlockByUID(uid, html);
                    else
                        error("do_eval: " + gettext("Evaluation problem: Unknown block") + " " + o.gui);
                    end
                end

                %scicos_prob=%f
                ier=execstr("o="+o.gui+"(''set'',o)","errcatch", "m")
                if ier==0& %scicos_prob==%f then
                    needcompile1=max(needcompile1,needcompile) // for scifunc_block
                    model_n=o.model
                    if or(model.blocktype<>model_n.blocktype)|.. // type 'c','d','z','l'
                        or(model.dep_ut<>model_n.dep_ut)|..
                        (model.nzcross<>model_n.nzcross)|..
                        (model.nmode<>model_n.nmode) then
                        needcompile1=4
                    end
                    if (size(model.in,"*")<>size(model_n.in,"*"))|..
                        (size(model.out,"*")<>size(model_n.out,"*"))|..
                        (size(model.evtin,"*")<>size(model_n.evtin,"*")) then
                        // number of input (evt or regular ) or output  changed
                        needcompile1=4
                    end
                    if model.sim=="input"|model.sim=="output" then
                        if model.ipar<>model_n.ipar then
                            needcompile1=4
                        end
                    end

                    itisanMBLOCK=%f
                    if prod(size(model.sim))>1 then
                        if (model.sim(2)==30004) then
                            itisanMBLOCK=%t
                        end
                    end

                    if (prod(size(model.sim))==1 & ~(model.equations==list())) | itisanMBLOCK then
                        if ~isequal(model.equations.parameters,model_n.equations.parameters) then
                            param_name   = model.equations.parameters(1);
                            param_name_n = model_n.equations.parameters(1);
                            if ~isequal(param_name,param_name_n) then
                                needcompile1=4
                            else
                                for i=1:length(model.equations.parameters(2))
                                    if or((model.equations.parameters(2)(i))<>(model_n.equations.parameters(2)(i))) then
                                        needcompile=0

                                        // the initialization XML files is re-created by translator if needed
                                        // XML=TMPDIR+'/'+stripblanks(scs_m.props.title(1))+'_imf_init.xml';
                                        // if ~deletefile(XML) then
                                        //   messagebox(msprintf(_('Unable to delete the file: %s'),XML),'error','modal');
                                        // end

                                        // the XML simulation is no more created with Xcos
                                        // XMLTMP=TMPDIR+'/'+stripblanks(scs_m.props.title(1))+'_imSim.xml'
                                        // if ~deletefile(XMLTMP) then
                                        //   messagebox(msprintf(_('Unable to delete the file: %s'),XMLTMP),'error','modal');
                                        // end

                                        break;
                                    end
                                end
                            end
                        end
                    end

                    scs_m.objs(%kk)=o
                else
                    error(msprintf(gettext("%s: Error while calling block %s [uid=''%s'']: invalid parameter (ier=%f, %%scicos_prob=%%%s).\n"), "do_eval", o.gui, o.model.uid, ier, string(%scicos_prob)));
                    ok=%f
                    return
                end
            end
        end
    end
    needcompile=needcompile1
    if needcompile==4 then cpr=list(),end
endfunction
