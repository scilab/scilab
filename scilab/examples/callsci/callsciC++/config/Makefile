# Copyright INRIA

FFLAGS = $(FC_OPTIONS) -lstdc++

myprog: $(OBJS) 
	@x=1;if test -f $@; then  \
		x=`find  $(OBJS) $(DEFAULTS) $(LIBR)  \( -name '*.a' -o -name '*.o' \) \
		-newer $@ -print | wc -l `; \
	fi;\
	if test $$x -ne 0; then \
		$(RM) $@; \
		echo "linking"; \
		$(FC) $(FFLAGS) -o $@  $(OBJS) $(DEFAULTS) $(FC_LDFLAGS) $(LIBR)\
			$(TCLTK_LIBS) $(XLIBS) $(TERMCAPLIB); \
	else \
		echo myprog is up to date ; \
	fi


show	:
	@echo $(FC) $(FFLAGS) -o $@  $(OBJS) $(DEFAULTS) $(FC_LDFLAGS) $(LIBR)\
		$(TCLTK_LIBS) $(XLIBS) $(TERMCAPLIB);
