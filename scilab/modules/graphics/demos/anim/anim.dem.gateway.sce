//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("anim.dem.gateway.sce");

subdemolist=[ ..
	"Rotation of a 3D Surface (plot3d1)" ,"anim1/anim1_gateway.sce"; ..
	"3D curve (param3d)"                 ,"anim2/anim2_gateway.sce"; ..
	"Lorentz curve (param3d) "           ,"anim3/anim3_gateway.sce"; ..
	"Rotation of a 3D contour"           ,"anim4/anim4_gateway.sce"; ..
	"Evolution of a 3D surface (plot3d)" ,"anim5/anim5_gateway.sce"; ..
	"N link pendulum movement"           ,"anim6/anim6_gateway.sce"; ..
	"Shell"                              ,"anim7/anim7_gateway.sce"];

if ~ usecanvas() then
	subdemolist = [ subdemolist ; "Riemann surface","anim8/anim8_gateway.sce" ];
end

subdemolist(:,2) = demopath + subdemolist(:,2);
