##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

# The order DOES matter !!!
SUBDIRS = \
	MALLOC \
	hashtable \
	libst \
	dynamiclibrary \
	lapack \
	doublylinkedlist

if USE_EMBEDDED_BLAS
SUBDIRS += blas
endif

if USE_EMBEDDED_LAPACK
SUBDIRS += lapack
endif 

SUBDIRS += .
