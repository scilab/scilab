function zoom_auto(h)
    // Zoom automatically to the visible values of all axes of the figure
    //
    // Calling Sequence
    //   zoom_auto()
    //   zoom_auto(h)
    //
    // Parameters
    // h: A Figure or Axes handle to zoom automatically
    //
    // Description
    // This function update the zoom_box of all axes of the figure when there is
    // some zoom applied to it or update the data bounds otherwise. After the call
    // the user will be able to view all visible data even if mis-configured
    // something.
    //
    // Examples
    // plot3d();
    // a=gca();
    // // hide some data to the user
    // a.data_bounds = [-1 -1 -0.5 ; 1 1 0.5]
    // sleep(1000)
    // // let the user view the whole dataset
    // zoom_auto()
    //
    // See also
    //  zoom_rect
    //  unzoom
    //  axes_properties
    //
    // Authors
    //  Clement DAVID - Scilab Enterprises

    rhs=argn(2);
    if rhs>1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "zoom_auto", 0, 1));
    end

    if (rhs == 0) then
        h=gcf();
    end
    if typeof(h)<>"handle" then
        error(sprintf(gettext("%s: Wrong type for argument %d: a handle expected.\n"), "zoom_auto", 1));
    end
    if size(h, "*")<>1 then
        error(sprintf(gettext("%s: Wrong size for input argument #%d: a handle expected.\n"), "zoom_auto" , 1));
    end

    visit(h, [0 0 0 ; 0 0 0])
endfunction

function [bounds]=visit(e, bounds)
    if isempty(e) then
        pause,
    end
    execstr("bounds=visit"+e.Type+"(e, bounds);");
endfunction

function [bounds]=visitFigure(e, bounds)
    for i=1:length(e.children)
        a=e.children(i);
        bounds = visit(a, bounds);

        if (a.zoom_state == "off") then
            a.data_bounds=bounds;
        else
            // the weird definition of zoom_box enforce us to perfom a projection
            M=[1 0 0 0 0 0 ;..
            0 0 1 0 0 0 ;..
            0 1 0 0 0 0 ;..
            0 0 0 1 0 0 ;..
            0 0 0 0 1 0 ;..
            0 0 0 0 0 1 ];
            a.zoom_box= M * bounds(:);
        end
    end
endfunction

function [bounds]=visitAxes(e, bounds)
    children = e.children;
    for i=1:length(children)
        bounds = visit(children(i), bounds);
    end
endfunction

function [bounds]=visitCompound(e, bounds)
    children = e.children;
    for i=1:length(children)
        bounds = visit(children(i), bounds);
    end
endfunction

function [bounds]=visitAxis(e, bounds)
    bounds(1,1)=min([a1.xtics_coord , bounds(1,1)]);
    bounds(2,1)=max([a1.xtics_coord , bounds(2,1)]);
    bounds(1,2)=min([a1.ytics_coord , bounds(1,2)]);
    bounds(2,2)=max([a1.ytics_coord , bounds(2,2)]);
endfunction

function [bounds]=visitPolyline(e, bounds)
    data = e.data;

    bounds(1,1)=min([data(:,1) ; bounds(1,1)]);
    bounds(2,1)=max([data(:,1) ; bounds(2,1)]);
    bounds(1,2)=min([data(:,2) ; bounds(1,2)]);
    bounds(2,2)=max([data(:,2) ; bounds(2,2)]);
    if (size(data,"c") > 2) then
        bounds(1,3)=min([data(:,3) ; bounds(1,3)]);
        bounds(2,3)=max([data(:,3) ; bounds(2,3)]);
    end
endfunction

function [bounds]=visitArc(e, bounds)
    data = e.data;

    bounds(1,1)=min([data(1) ; bounds(1,1)]);
    bounds(2,1)=max([data(1)+data(3) ; bounds(2,1)]);
    bounds(1,2)=min([data(2)-data(4) ; bounds(1,2)]);
    bounds(2,2)=max([data(2) ; bounds(2,2)]);
endfunction

function [bounds]=visitRectangle(e, bounds)
    data = e.data;

    bounds(1,1)=min([data(1) ; bounds(1,1)]);
    bounds(2,1)=max([data(1)+data(3) ; bounds(2,1)]);
    bounds(1,2)=min([data(2)-data(4) ; bounds(1,2)]);
    bounds(2,2)=max([data(2) ; bounds(2,2)]);
endfunction

function [bounds]=visitSurface(e, bounds)
    data = e.data;

    bounds(1,1)=min([min(data.x) ; bounds(1,1)]);
    bounds(2,1)=max([max(data.x) ; bounds(2,1)]);
    bounds(1,2)=min([min(data.y) ; bounds(1,2)]);
    bounds(2,2)=max([max(data.y) ; bounds(2,2)]);
    bounds(1,3)=min([min(data.z) ; bounds(1,3)]);
    bounds(2,3)=max([max(data.z) ; bounds(2,3)]);
endfunction

function [bounds]=visitFac3d(e, bounds)
    [bounds]=visitSurface(e, bounds)
endfunction

function [bounds]=visitPlot3d(e, bounds)
    [bounds]=visitSurface(e, bounds)
endfunction

function [bounds]=visitFec(e, bounds)
    data = e.data;

    // TODO: Fec zoom behavior is not clear to me, should be reviewed carefully
    bounds(1,1)=min([data(:,2) ; bounds(1,1)]);
    bounds(2,1)=max([data(:,2) ; bounds(2,1)]);
    bounds(1,2)=min([data(:,1) ; bounds(1,2)]);
    bounds(2,2)=max([data(:,1) ; bounds(2,2)]);
endfunction

function [bounds]=visitGrayplot(e, bounds)
    [bounds]=visitSurface(e, bounds)
endfunction

function [bounds]=visitMatplot(e, bounds)
    data = e.rect;

    bounds(1,1)=min([data(1) ; bounds(1,1)]);
    bounds(2,1)=max([data(3) ; bounds(2,1)]);
    bounds(1,2)=min([data(2) ; bounds(1,2)]);
    bounds(2,2)=max([data(4) ; bounds(2,2)]);
endfunction

function [bounds]=visitSegs(e, bounds)
    data = e.data;

    bounds(1,1)=min([data(:,1) ; bounds(1,1)]);
    bounds(2,1)=max([data(:,1) ; bounds(2,1)]);
    bounds(1,2)=min([data(:,2) ; bounds(1,2)]);
    bounds(2,2)=max([data(:,2) ; bounds(2,2)]);
    if (size(data,"c") > 2) then
        bounds(1,3)=min([data(:,3) ; bounds(1,3)]);
        bounds(2,3)=max([data(:,3) ; bounds(2,3)]);
    end
endfunction

function [bounds]=visitChamp(e, bounds)
    data = e.data;

    bounds(1,1)=min([data.x ; bounds(1,1)]);
    bounds(2,1)=max([data.x ; bounds(2,1)]);
    bounds(1,2)=min([data.y ; bounds(1,2)]);
    bounds(2,2)=max([data.y ; bounds(2,2)]);
endfunction

function [bounds]=visitText(e, bounds)
    data = e.data;

    bounds(1,1)=min([data(1) ; bounds(1,1)]);
    bounds(1,2)=min([data(2) ; bounds(1,2)]);
    if (size(data,"c") > 2) then
        bounds(1,3)=min([data(3) ; bounds(1,3)]);
    end
endfunction

function [bounds]=visitLegend(e, bounds)
    // TODO: implement something ; it might not be possible at all
endfunction

function [bounds]=visitLight(e, bounds)
    // nothing to do, a light is always visible
endfunction
