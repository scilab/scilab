#ifndef __BT_INT__
#define __BT_INT__

struct bti_node {
	int data;
	struct bti_node *l,*r;
};

int BTI_init( struct bti_node **n );
int BTI_add( struct bti_node **n, int value );
int BTI_dump( struct bti_node **n );
int BTI_done( struct bti_node **n );

#endif
