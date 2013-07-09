
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Test du cas 'define' des fonctions gui des blocs
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function r=checkdefine()
    r=%f
    for i=1:nb
        execstr("gui="+Blocs(i))
        ierr=execstr("x=gui(''define'')","errcatch")
        if ierr<>0 then
            mprintf("define failed for block "+Blocs(i));
            r=%t
            return;
        end
        clear gui
    end
endfunction
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Test du cas 'inputs' des fonctions gui des blocs
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function r=checkinputs()
    r=%f
    for i=1:nb
        execstr("gui="+Blocs(i))
        o=gui("define")
        ierr=execstr("[x,y,typ]=gui(''getinputs'',o)","errcatch")
        if ierr<>0 then
            mprintf("getinputs failed for block "+Blocs(i));
            r=%t
            return;
        end
        clear gui
    end
endfunction

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Test du cas 'outputs' des fonctions gui des blocs
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function r=checkoutputs()
    r=%f
    for i=1:nb
        execstr("gui="+Blocs(i))
        o=gui("define")
        ierr=execstr("[x,y,typ]=gui(''getoutputs'',o)","errcatch")
        if ierr<>0 then
            mprintf("getoutputs failed for block "+Blocs(i));
            r=%t
            return;
        end
        clear gui
    end
endfunction
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Test du cas 'origin' des fonctions gui des blocs
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function r=checkorigin()
    r=%f
    for i=1:nb
        execstr("gui="+Blocs(i))
        o=gui("define")
        ierr=execstr("[x,y,typ]=gui(''getorigin'',o)","errcatch")
        if ierr<>0 then
            mprintf("getorigin failed for block "+Blocs(i));
            r=%t
            return;
        elseif x<>0|y<>0 then
            mprintf("getorigin failed for block "+Blocs(i));
            r=%t
            return;
        end
        clear gui
    end
endfunction
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Test de la taille du vecteur dep_ut
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function r=checkdeput()
    r=%f
    for i=1:nb
        execstr("gui="+Blocs(i))
        o=gui("define");
        dpt=o.model.dep_ut;
        if length(o.model.dep_ut)~=2 then
            mprintf("check dep_ut failed for block "+Blocs(i));
            r=%t
            return;
        end
        clear gui
    end
endfunction

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Test de la taille du vecteur firing
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function r=checkfiring()
    r=%f
    for i=1:nb
        execstr("gui="+Blocs(i))
        o=gui("define");
        dpt=o.model.firing;
        if length(o.model.firing)==[] then
            mprintf("check firing failed for block "+Blocs(i));
            r=%t
            return;
        end
        clear gui
    end
endfunction

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Test de coherence des ports entre model et graphic cas define
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function r=check_define_ports()
    r=%f
    for i=1:nb
        execstr("gui="+Blocs(i))
        o=gui("define");
        opim=o.model.in;
        opom=o.model.out;
        opeim=o.model.evtin;
        opeom=o.model.evtout;
        opig=o.graphics.pin;
        opog=o.graphics.pout;
        opeig=o.graphics.pein;
        opeog=o.graphics.peout;

        if size(opim,1)<>size(opig,1) | ..
            size(opom,1)<>size(opog,1)  | ..
            size(opeim,1)<>size(opeig,1)  | ..
            size(opeom,1)<>size(opeog,1) then
            mprintf("check  define_ports failed for block "+Blocs(i));
            r=%t
            return;
        end
        clear gui
    end
endfunction

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Test de coherence des ports entre model et graphic : cas set
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function r=check_set_ports()
    r=%f
    prot=funcprot();
    funcprot(0);
    scicos_getvalue=setvalue;
    funcprot(prot);
    alreadyran=%f;  %scicos_prob=%f
    for i=1:nb
        if and(Blocs(i)~=["SUPER_f" "func_block" "scifunc_block" "GENERAL_f" "SOM_f" "CURV_f" "LOOKUP_f"]) then
            if Blocs(i)=="fortran_block"|Blocs(i)=="c_block" then
                exec("SCI/modules/scicos_blocks/macros/"+Blocs(i)+".sci")
            else
                exec("SCI/modules/scicos_blocks/macros/"+Blocs(i)+".sci")
            end
            execstr("gui="+Blocs(i))
            o=gui("define");
            o_n=gui("set",o);
            o_npim=o_n.model.in;
            o_npom=o_n.model.out;
            o_npeim=o_n.model.evtin;
            o_npeom=o_n.model.evtout;
            o_npig=o_n.graphics.pin;
            o_npog=o_n.graphics.pout;
            o_npeig=o_n.graphics.pein;
            o_npeog=o_n.graphics.peout;
            if size(o_npim,1)<>size(o_npig,1) | ..
                size(o_npom,1)<>size(o_npog,1)  | ..
                size(o_npeim,1)<>size(o_npeig,1)| ..
                size(o_npeom,1)<>size(o_npeog,1) then
                mprintf("check set_ports failed for block "+Blocs(i));
                r=%t
                return;
            end
            clear gui
        end
    end
endfunction
