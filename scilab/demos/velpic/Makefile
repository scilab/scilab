

FFLAGS=-O -Nl50 $(FC_PICFLAGS)

OBJS=/tmp/testpt.o /tmp/id_rgn.o

all: $(OBJS)

/tmp/testpt.o	: testpt.f
	$(FC) $(FFLAGS) -c testpt.f -o $@

/tmp/id_rgn.o	: id_rgn.f
	$(FC) $(FFLAGS) -c id_rgn.f -o $@
 
clean	:
	rm *.o 


