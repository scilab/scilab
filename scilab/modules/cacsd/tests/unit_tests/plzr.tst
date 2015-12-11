// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// Unit tests for plzr()

function checkplzr(h,zrs,pls)

    if typeof(h)=="rational" then
        plzr(h);
        h=tf2ss(h);
        [a,b,c,d]=h(2:5);
        dt=h.dt;
    elseif typeof(h)=="state-space" then
        plzr(h);
        [a,b,c,d]=h(2:5)
        dt=h.dt;
    else
        plzr(h(1),h(2),h(3),h(4));
        a=h(1);
        b=h(2);
        c=h(3);
        d=h(4);
        dt=[];
    end
    fig=gcf();
    ax=fig.children;
    dr=spec(a)
    [al,be]=tr_zer(a,b,c,d)
    nr=al./be;
    nr=real(nr);
    dr=real(dr);

    //nb_child=size
    assert_checkequal(ax.type, "Axes");

    // Check axis name
    assert_checkequal(ax.x_label.text, _("Real axis"));
    assert_checkequal(ax.y_label.text, _("Imaginary axis"));

    // Check legend
    assert_checkequal(ax.children(1).type,"Legend");

    // Check stability
    if dt == "d" then
        assert_checkequal(ax.children(2).type,"Arc");
    else
        assert_checkequal(ax.children(2).type,"Segs");
    end

    // Check zeros
    if size(nr,"*") <> 0 & size(dr,"*") == 0 then
        assert_checkequal(ax.children(1).text, _("Zeros"));
        assert_checkequal(ax.children(3).type, "Polyline");
        assert_checktrue(ax.children(3).data-zrs<1d-5);
    end
    // Check poles
    if size(dr,"*") <> 0 & size(nr,"*") == 0 then
        assert_checkequal(ax.children(1).text, _("Poles"));
        assert_checkequal(ax.children(3).type, "Polyline");
        assert_checktrue(ax.children(3).data-pls<1d-5);
    end
    // Check zeros and poles
    if size(nr,"*")<> 0 & size(dr,"*") <> 0 then
        assert_checkequal(ax.children(1).text,[_("Poles");_("Zeros")]);
        assert_checkequal(ax.children(3).type,"Polyline");
        assert_checkequal(ax.children(4).type,"Polyline");
        assert_checktrue(ax.children(3).data-zrs<1d-5);
        assert_checktrue(ax.children(4).data-pls<1d-5);
    end
    close
endfunction

// With zeros and poles
s=poly(0,"s");
n=1+s;
d=1+3*s;
// continuous transfert fct
hc=syslin("c",n./d);
checkplzr(hc,[-1/3 0],[-1 0]);
// discete transfert fct
hd=syslin("d",n./d);
checkplzr(hd,[-1/3 0],[-1 0]);
// undifined time domain transfert fct
h=syslin([],n./d);
checkplzr(h,[-1/3 0],[-1 0]);
// continuous state-space
ssc=tf2ss(hc);
checkplzr(ssc,[-1/3 0],[-1 0]);
checkplzr([ssc(2),ssc(3),ssc(4),ssc(5)],[-1/3 0],[-1 0]);
// discrete state-space
ssd=tf2ss(hd);
checkplzr(ssd,[-1/3 0],[-1 0]);
// undifined time domain state-space
ss=tf2ss(h);
checkplzr(ss,[-1/3 0],[-1 0]);
checkplzr([ss(2),ss(3),ss(4),ss(5)],[-1/3 0],[-1 0]);

// With poles
s=poly(0,"s");
n=[1+s 2+3*s+4*s^2];
d=[1+3*s 5-s^3];
// continuous transfert fct
hc=syslin("c",n./d);
checkplzr(hc,[],[1.7099759,0;-0.8549880,1.4808826;-0.8549880,-1.4808826;-1/3 0]);
// discete transfert fct
hd=syslin("d",n./d);
checkplzr(hd,[],[1.7099759,0;-0.8549880,1.4808826;-0.8549880,-1.4808826;-1/3 0]);
// undifined time domain transfert fct
h=syslin([],n./d);
checkplzr(h,[],[1.7099759,0;-0.8549880,1.4808826;-0.8549880,-1.4808826;-1/3 0]);
// continuous state-space
ssc=tf2ss(hc);
checkplzr(ssc,[],[1.7099759,0;-0.8549880,1.4808826;-0.8549880,-1.4808826;-1/3 0]);
// discrete state-space
ssd=tf2ss(hd);
checkplzr(ssd,[],[1.7099759,0;-0.8549880,1.4808826;-0.8549880,-1.4808826;-1/3 0]);
// undifined time domain state-space
ss=tf2ss(h);
checkplzr(ss,[],[1.7099759,0;-0.8549880,1.4808826;-0.8549880,-1.4808826;-1/3 0]);

// error messages
errmsg1=msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"plzr",4);
assert_checkerror("plzr(1)",errmsg1);
errmsg2=msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"plzr",1);
assert_checkerror("plzr(h,1)",errmsg2);
errmsg3=msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"plzr",1);
assert_checkerror("plzr(ss,1)",errmsg3);
errmsg4=msprintf(gettext("%s: Wrong type for input argument #%d: Linear dynamical system expected.\n"),"plzr",1);
assert_checkerror("plzr(n)",errmsg4);
errmsg5=msprintf(gettext("%s: Wrong type of input argument #%d: Array of floating point numbers expected.\n"),"plzr",1);
assert_checkerror("plzr(""s"",""d"")",errmsg5);
