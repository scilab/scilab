/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
/*--------------------------------------------------------------------------*/
#include "sci_malloc.h"
#include "bt-int.h"
/*--------------------------------------------------------------------------*/
int BTI_init( struct bti_node **n )
{
    *n = NULL;
    return 0;
}
/*--------------------------------------------------------------------------*/
int BTI_add( struct bti_node **n, int value )
{
    int collision = 0;
    int dir = 0;
    struct bti_node *p = NULL, *node = *n;

    /*	fprintf(stdout,"Adding %d to %p\n", value, *n);*/
    while (node != NULL)
    {
        if (value > node->data)
        {
            p = node;
            dir = 1;
            node = node->r;
        }
        else if (value < node->data)
        {
            p = node;
            dir = -1;
            node = node->l;
        }
        else if (value == node->data)
        {
            collision = 1;
            break;
        }
    }

    if (collision == 0)
    {
        struct bti_node *leaf = NULL;

        leaf = MALLOC(sizeof(struct bti_node));
        if (leaf == NULL)
        {
            return -1;
        }

        leaf->data = value;
        leaf->l = leaf->r = NULL;

        if (p != NULL)
        {
            switch (dir)
            {
                case 1:
                    p->r = leaf;
                    break;
                case -1:
                    p->l = leaf;
                    break;
            }
        }
        else
        {
            *n = leaf;
        }
    }

    return collision;
}
/*--------------------------------------------------------------------------*/
int BTI_dump( struct bti_node **n )
{
    struct bti_node *node = NULL;

    node = *n;

    if (node->l)
    {
        BTI_dump(&(node->l));
    }
    if (*n)
    {
        fprintf(stdout, "%d, ", node->data);
    }
    if (node->r)
    {
        BTI_dump(&(node->r));
    }

    return 0;

}
/*--------------------------------------------------------------------------*/
int BTI_done( struct bti_node **n )
{
    struct bti_node *node = NULL ;

    if (n == NULL)
    {
        return 0;
    }
    if (*n == NULL)
    {
        return 0;
    }

    node = *n;

    if (node->l)
    {
        BTI_done(&(node->l));
    }
    if (node->r)
    {
        BTI_done(&(node->r));
    }
    if (*n)
    {
        FREE(*n);
        *n = NULL;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
