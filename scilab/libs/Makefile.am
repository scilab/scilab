##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

# The order DOES matter !!!
SUBDIRS =

# This is first to be sure that it is the first one to be compiled
if USE_EMBEDDED_BLAS
SUBDIRS += blas
endif

SUBDIRS += \
	MALLOC \
	hashtable \
	libst \
	dynamiclibrary \
	lapack \
	doublylinkedlist

if USE_EMBEDDED_LAPACK
SUBDIRS += lapack
endif 

SUBDIRS += .
