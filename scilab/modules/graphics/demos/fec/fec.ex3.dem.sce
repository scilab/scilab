demopath = get_absolute_file_path("fec.ex3.dem.sce");
demo_run(demopath+"/fec.ex3");

f=mopen(demopath+"/fec.ex3");
demo_viewCode("fec.ex3");
mclose(f);
