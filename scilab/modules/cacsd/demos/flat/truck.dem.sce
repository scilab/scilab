exec(get_absolute_file_path("truck.dem.sce") + "/truck.sci",-1);

initial   = [-2;3;0.5235988;0;0;1];
final     = [0;0;0;0;0;0];
state     = truck_solve(initial,final);
my_handle = scf(100001);
clf(my_handle,"reset");
my_handle.pixmap = "off" ;
toolbar(my_handle.figure_id,"off") ;
display_truck_trajectory(state);
