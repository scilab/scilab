GPERF=gperf

all :: Xcall1.c Xcall.c 

Xcall1.c: Xcall1.gperf
	$(GPERF) -C -t -k 2,3,4,'$$' Xcall1.gperf > Xcall1.c

Xcall.c: Xcall.gperf
	$(GPERF) -C -t -k 2,3,4,'$$' Xcall.gperf > Xcall.c.prov 
	sed -e "s/in_word_set/xcall_in_word_set/" Xcall.c.prov > Xcall.c 
	$(RM) Xcall.c.prov 



