demopath = get_absolute_file_path("fec.ex4.dem.sce");
demo_run(demopath+"/fec.ex4");

f=mopen(demopath+"/fec.ex4");
demo_viewCode("fec.ex4");
mclose(f);
