exec(get_absolute_file_path("car.dem.sce") + "/car.sci",-1);

initial               =[3;3;0;0];
final                 =[0;0;0;0];
my_handle             = scf(100001);
clf(my_handle,"reset");
my_handle.pixmap      = "off" ;
toolbar(my_handle.figure_id,"off");
display_truck_trajectory(state);
state=car_solve(initial,final);
display_car_trajectory(state);
