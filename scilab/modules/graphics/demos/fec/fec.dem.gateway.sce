
demopath = get_absolute_file_path("fec.dem.gateway.sce");

subdemolist=["Finite Element display with colorbar" ,"fec.ex4.dem.sce";
"Finite Element display with a mesh"      ,"fec.ex2.dem.sce";
"Sfgrayplot "                             ,"fec.ex3.dem.sce"];

subdemolist(:,2) = demopath + subdemolist(:,2);

clear demopath;