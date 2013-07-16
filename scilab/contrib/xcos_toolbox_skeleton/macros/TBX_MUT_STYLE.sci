//
// This file is part of the Xcos skeleton toolbox
//
// see license.txt for more licensing information

function [x,y,typ]=TBX_MUT_STYLE(job,arg1,arg2)
    x=[];y=[];typ=[]
    select job
    case "set" then
        x=arg1;
        model = x.model;
        graphics = x.graphics;

        // no parameters yet, just change the style of the block and the ports
        hexa = [string(1:9) "A" "B" "C" "D" "E" "F"];
        graphics.style = "BIGSOM_f;fillColor=#" + strcat(hexa(15 * rand(1,6) + 1));

        port_styles = ["actor" "cloud" "connector" "cylinder" "curve" "doubleEllipse" "ellipse" "hexagon" "image" "label" "line" "rectangle" "rhombus" "triangle"];
        graphics.in_style = "ExplicitInputPort;shape=" + port_styles(size(port_styles, "*") * rand(model.in) + ones(model.in));
        graphics.out_style = "ExplicitOutputPort;shape=" + port_styles(size(port_styles, "*") * rand(model.out) + ones(model.out));

        disp(graphics.style);
        disp(graphics.in_style);
        disp(graphics.out_style);

        x.model = model;
        x.graphics = graphics;

    case "define" then
        model=scicos_model()
        model.sim=list("block_sum",4)
        // n inputs with a single "double" element
        model.in=ones(4, 1);
        model.intyp=ones(model.in);
        // one output with a single "double" element
        model.out=1
        model.outtyp=1

        model.blocktype="c"
        model.dep_ut=[%f %f]

        exprs=string([]);
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i);
        x.graphics.style="BIGSOM_f";
    end
endfunction

