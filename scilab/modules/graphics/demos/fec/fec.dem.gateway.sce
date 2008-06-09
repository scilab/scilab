
demopath = get_absolute_file_path("fec.dem.gateway.sce");

subdemolist=["Finite Element display with colorbar"  ,"fec.ex4.dem.gateway.sce";
          "Finite Element display with a mesh"    ,"fec.ex2.dem.gateway.sce";
          "Sfgrayplot "                           ,"fec.ex3.dem.gateway.sce"];

subdemolist(:,2) = demopath + subdemolist(:,2);
