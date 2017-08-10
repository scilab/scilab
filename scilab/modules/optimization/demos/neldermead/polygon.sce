// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

mprintf("Finds the largest n-polygon with diameter smaller than 1.\n");

function [ f , c , index ] = largesmallpolygon ( x , index )
    // Bibliography
    //   "Benchmarking Optimization Software with Cops"
    //   Dolan, Moré, 2001
    //   Section 1, "Largest Small Polygon"
    //
    //   "Biggest Little Polygon"
    //   http://mathworld.wolfram.com/BiggestLittlePolygon.html
    //
    //   Audet, C. "Optimisation globale structurée: propriétés, équivalences et résolution."
    //   Thèse de Doctorat. Montréal, Canada: École Polytechnique de Montréal, 1997.
    //   http://www.gerad.ca/Charles.Audet.
    //
    // Known optimal values are
    // A6 = 0.677981 (Wolfram Mathworld)
    // A8 = 0.726869 (Wolfram Mathworld)
    // A25 = 0.77974 (Dolan & Moré - SNOPT)
    // A50 = 0.784016 (Dolan & Moré - SNOPT)
    // A75 = 0.784769 (Dolan & Moré - SNOPT)
    // A100 = 0.785040 (Dolan & Moré - SNOPT)
    //
    nv = size(x,"*")/2
    f = []
    c = []
    // nv: number of vertices
    // x : a (2*nv)-by-1 matrix of doubles, where
    // r is in [0,1]
    // t is in [0,pi]
    r = x(1:nv)
    t = x(nv+1:2*nv)

    //polygon_draw(r ,t , nv);
    //pause

    if ( index == 2 | index == 6 ) then
        f = polygon_area(r ,t , nv)
        f = -f
    end
    if ( index == 5 | index == 6 ) then
        c = zeros(nv^2+nv-1,1)
        // Set the diameters
        d = polygon_diameters(r ,t , nv)
        d = matrix(d,nv^2,1)
        c(1:nv^2) = 1-d
        // Set the angles
        c(nv^2+1:nv^2+nv-1) = t(2:nv)-t(1:nv-1)
        // Transpose for neldermead
        c = c'
    end
endfunction

function polygon_draw(r ,t , nv)
    // Draws a polygon with nv vertices.
    // r : a nv-by-1 matrix of doubles, the radius
    // t : a nv-by-1 matrix of doubles, the angles
    x = r.*cos(t);
    y = r.*sin(t);
    i = 1;
    x($+1) = r(i)*cos(t(i));
    y($+1) = r(i)*sin(t(i));
    plot(x,y,"bo-")
    isoview(gcf(), "on")
endfunction

function polygon_update(h,r ,t , nv)
    // Draws a polygon with nv vertices.
    // r : a nv-by-1 matrix of doubles, the radius
    // t : a nv-by-1 matrix of doubles, the angles
    x = r.*cos(t);
    y = r.*sin(t);
    i = 1;
    x($+1) = r(i)*cos(t(i));
    y($+1) = r(i)*sin(t(i));
    h.data = [x y]
endfunction

function [r,t] = polygon_regular (nv)
    // Returns a regular polygon with nv vertices
    // and unit radius, centered at origin.
    // r : a nv-by-1 matrix of doubles, the radius
    // t : a nv-by-1 matrix of doubles, the angles
    r = ones(nv,1);
    tnv = 2*%pi/nv
    t = linspace(-%pi+tnv,%pi,nv)'
endfunction

function f = polygon_area(r ,t , nv)
    // Returns the area of a polygon with nv vertices.
    // r : a nv-by-1 matrix of doubles, the radius
    // t : a nv-by-1 matrix of doubles, the angles
    f = r(nv) * r(1) * sin(t(1)-t(nv))
    for i = 1: nv-1
        f = f + r(i+1) * r(i) * sin(t(i+1)-t(i))
    end
    f = 0.5 * f
endfunction



function d = polygon_diameters(r ,t , nv)
    // Returns the diameters of a polygon with nv vertices.
    // r : a nv-by-1 matrix of doubles, the radius
    // t : a nv-by-1 matrix of doubles, the angles
    d = zeros(nv,nv)
    for i = 1 : nv
        for j = 1 : nv
            d(i,j) = r(i)^2 + r(j)^2 - 2*r(i)*r(j)*cos(t(i)-t(j))
            d(i,j) = abs(d(i,j))
            d(i,j) = sqrt(d(i,j))
        end
    end
endfunction




/////////////////////////////////////////////////
//
// Maximize the size of the polynomial
//
//
function stop = myoutputcmd(state, data)
    stop = %f
    iter = data.iteration
    if ( state == "init" ) then
        mprintf ( "=================================\n");
        mprintf ( "Initialization\n");
    elseif ( state == "done" ) then
        mprintf ( "=================================\n");
        mprintf ( "End of Optimization\n");
    end
    fc = data.funccount
    fval = data.fval
    x = data.x
    simplex = data.simplex
    step = data.step
    ssize = optimsimplex_size ( simplex )
    //
    // Plot current solution
    if ( %t & modulo(iter,10) == 0) then
        h = findobj ( "user_data" , "nmpolygon" );
        nv = size(x,"*")/2
        r = x(1:nv)
        t = x(nv+1:$)
        polygon_update(h.children.children,r ,t , nv)
        a = polygon_area(r ,t , nv);
        str = msprintf("Largest Small Polygon - Area=%f",a);
        h.title.text = str
    end
    //
    if ( modulo(iter,10) == 0 ) then
        mprintf ( "Iter. #%3d, Feval #%3d, Fval = %f, S = %.1e\n", ..
        iter, fc, fval, ssize);
    end
endfunction





function [A,r,t] = findlargestpolygon (nv)
    // Finds the largest smallest polygon with nv vertices
    // A : a 1-by-1 matrix of doubles, the area
    // r : a nv-by-1 matrix of doubles, the radius
    // t : a nv-by-1 matrix of doubles, the angle
    radius = 0.45;
    [r,t] = polygon_regular (nv);
    r = radius*r;
    scf()
    polygon_draw(r ,t , nv);
    h = gcf();
    h.children.data_bounds = [
    -0.6 -0.6
    0.6 0.6
    ];
    h.children.user_data = "nmpolygon";
    x0 = [r;t];
    index = 6;
    [ f0 , c0 , index ] = largesmallpolygon ( x0 , index );
    mprintf("Current area = %f\n",-f0);
    mprintf("Constraint satisfaction = %f (expected positive)\n",min(c0));
    //
    // Setup bounds
    rmin = zeros(nv,1);
    rmax = ones(nv,1);
    tmin = -ones(nv,1)*%pi;
    tmax = ones(nv,1)*%pi;
    xmin=[rmin;tmin];
    xmax=[rmax;tmax];
    //
    nm = neldermead_new ();
    nm = neldermead_configure(nm,"-numberofvariables",2*nv);
    nm = neldermead_configure(nm,"-function",largesmallpolygon);
    nm = neldermead_configure(nm,"-x0",x0);
    nm = neldermead_configure(nm,"-maxiter",2000);
    nm = neldermead_configure(nm,"-maxfunevals",2000);
    nm = neldermead_configure(nm,"-method","box");
    nm = neldermead_configure(nm,"-boundsmin",xmin');
    nm = neldermead_configure(nm,"-boundsmax",xmax');
    nm = neldermead_configure(nm,"-simplex0method","randbounds");
    nm = neldermead_configure(nm,"-nbineqconst",nv^2+nv-1);
    nm = neldermead_configure(nm,"-outputcommand",myoutputcmd);
    //
    // Check that the cost function is correctly connected.
    [ nm , result ] = neldermead_function ( nm , x0 );
    //
    // Perform optimization
    nm = neldermead_search(nm, "off");
    fopt = neldermead_get(nm,"-fopt")
    A = -fopt
    xopt = neldermead_get(nm,"-xopt")
    r = xopt(1:nv)
    t = xopt(nv+1:$)
    nm = neldermead_destroy(nm)
endfunction

////////////////////////////////////////////////////////
// Checking area for nv=6
// Use a regular hexagon (see Graham, page 5, Fig. 5).
// A = 0.64952...
nv = 6;
radius = 0.5;
[r,t] = polygon_regular (nv);
r = radius*r;
h = scf();
polygon_draw(r ,t , nv);
f = polygon_area(r ,t , nv);
//
// Check this
// A = 3*sqrt(3)*s^2/2, where s is the side length, i.e. the radius
A = 3*sqrt(3)*radius^2/2;

mprintf("Area =%f (expected = %f)\n",f,A);

d = polygon_diameters(r ,t , nv);
dmax = max(d);
mprintf("Maximum diameter=%f (expected = %f)\n",dmax,2*radius);
dmin = min(d(d<>0));
mprintf("Minimum diameter=%f (expected=%f)\n",dmin,radius);
close(h);

////////////////////////////////////////////////////////
//
// Solve problem
//
nv = 6;
rand("seed" , 0);
[A,r,t] = findlargestpolygon (nv);
mprintf("Maximum Area =%f (expected = %f)\n",A,0.677981);
