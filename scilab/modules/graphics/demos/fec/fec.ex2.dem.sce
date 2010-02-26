demopath = get_absolute_file_path("fec.ex2.dem.sce");
demo_run(demopath+"/fec.ex2");

f=mopen(demopath+"/fec.ex2");
demo_viewCode("fec.ex2");
mclose(f);
