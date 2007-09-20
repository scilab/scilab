/**
   \file scoWindowScope.h
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief Header File of the scoWindowScope.c file
*/

#ifndef __SCO_WINDOWSCOPE_H__
#define __SCO_WINDOWSCOPE_H__

#include"scoBase.h"

/**
   \brief Initialization of the Window and its apparence
   \param pScopeMemory a pointer on a ScopeMemory
   \param dimension the dimension of the axes 2 or 3
   \param win_id the windows ID
   \param win_pos a table of 2 numbers which determines the position of the window on the screen
   \param win_dim a table of 2 numbers which determines the dimension of the window on the screen
   \param xmin a table of (number_of_subwin) size
   \param xmax a table of (number_of_subwin) size
   \param ymin a table of (number_of_subwin) size 
   \param ymax a table of (number_of_subwin) size
   \param zmin a table of (number_of_subwin) size
   \param zmax a table of (number_of_subwin) size
*/
void scoInitOfWindow(ScopeMemory * pScopeMemory, int dimension, int win_id, int * win_pos, int * win_dim, double * xmin, double * xmax, double * ymin, double * ymax, double * zmin, double * zmax);

/**
   \brief Allocate space and set block_number in the user_data of the window associated in the pScopeMemory
 **/
void scoSetInUserData(ScopeMemory * pScopeMemory,int block_number);

/**
   \brief Retrive UserData[0] (i.e. the block_number we have stocked)
   \param pTemp is a pointer on the figure
 **/
scoInteger scoGetUserData(scoGraphicalObject pTemp);

/*--------------------------CREATIONS FUNCTIONS-----------------------------*/
/*
All these functions create pointer of special objects. We could use directly the sci graphic library function but in Scicos we have some behaviors which doesn't need to be really tuned for our usage. For this reason we have redefined them by putting some default values for some parameters.
 */

/**
   \brief Create a polyline and return a pointer on it
   \param pAxes A pointer on the designated axe
   \param polyline_size Size of the polyline for pvx, pvy or pvz
   \param color The color of the polyline. If <= 0 it will be a mark else it will be a line.
   \attention There is a polyline_size to allocate the table but dont forget that the n1 value of pPOLYLINE_FEATURE is set to 0
*/
scoGraphicalObject scoCreatePolyline(scoGraphicalObject pAxes, scoInteger polyline_size,int color);

/**
   \brief Create a Filled Circle  by using the constructArc method of sgl and return a pointer on it
   \param pAxes A pointer on the designated axe
   \param radius the radius of the filled circle
   \param color the color of the filled circle
*/
scoGraphicalObject scoCreateSphere(scoGraphicalObject pAxes, double radius, int color);

/**
   \brief Create a Rectangle by using constructRectangle and return a pointer on it
   \param pAxes a pointer on the designated axe
   \param x the abscisse of the left bottom corner of the rectangle
   \param y the ordonate of the left bottom corner of the rectangle
   \param width width of the rectangle
   \param height height of the rectangle
*/
scoGraphicalObject scoCreateRectangle(scoGraphicalObject pAxes, double x, double y, double width, double height);

/**
   \brief Create a Grayplot using constructGrayplot and return a pointer on it
   \param pAxes a pointer on the designated axes
   \param size_x size of the grayplot for abscisses
   \param size_y size of the grayplot for ordinates
*/
scoGraphicalObject scoCreateGrayplot(scoGraphicalObject pAxes, int size_x, int size_y);

/**
   \brief Create a Plot3d using constructSurface (a param is for plot3d) and return a pointer on it
   \param pAxes a pointer on the designated axes
   \param size_x size of the plot3d for abscisses
   \param size_y size of the plot3d for ordinates
*/
scoGraphicalObject scoCreatePlot3d(scoGraphicalObject pAxes, int size_x, int size_y);

/*---------------------------ADDING FUNCTIONS------------------------------*/
/*
All these functions take a pointer of a graphical object and place it on axes i at position j. There is one for ShortDraw and one for LongDraw.
 */

/**
   \brief Add a polyline for a shortdraw
   \param pScopeMemory a pointer on the ScopeMemory
   \param i the number of the axes
   \param j the number of the designated shortdraw in the axes i
   \param color color of the polyline. <=0 is a mark >0 is a line
*/
void scoAddPolylineForShortDraw(ScopeMemory * pScopeMemory, int i, int j, int color);

/**
   \brief Add a polyline for a longdraw
   \param pScopeMemory  a pointer on the ScopeMemory
   \param i the number of the axes
   \param j the number of the designated longdraw in the axes i
   \param color color of the polyline. <=0 is a mark >0 is a line

*/
void scoAddPolylineForLongDraw(ScopeMemory * pScopeMemory, int i, int j, int color);

/**
   \brief Add a sphere to the axis as a shortdraw
   \param pScopeMemory  a pointer on the ScopeMemory
   \param i the number of the axes
   \param j the number of the designated shortdraw in the axes i
   \param radius the radius of the sphere
   \param color the color of the filled sphere
*/
void scoAddSphereForShortDraw(ScopeMemory * pScopeMemory, int i, int j, double radius, int color);

/**
   \brief Add a sphere to the axis as a longdraw
   \param pScopeMemory  a pointer on the ScopeMemory
   \param i the number of the axes
   \param j the number of the designated longdraw in the axes i
   \param radius the radius of the sphere
   \param color the color of the filled sphere
*/
void scoAddSphereForLongDraw(ScopeMemory * pScopeMemory, int i, int j, double radius, int color);

/**
   \brief Add a rectangle for a longdraw
   \param pScopeMemory  a pointer on the ScopeMemory
   \param i the number of the axes
   \param j the number of the designated longdraw in the axes i
   \param x the abscisse of the left bottom corner of the rectangle
   \param y the ordonate of the left bottom corner of the rectangle
   \param width width of the rectangle
   \param height height of the rectangle

*/
void scoAddRectangleForLongDraw(ScopeMemory * pScopeMemory, int i, int j, double x, double y, double width, double height);

/**
   \brief Add a grayplot for a shortdraw
   \param pScopeMemory  a pointer on the ScopeMemory
   \param i the number of the axes
   \param j the number of the designated shortdraw in the axes i
   \param size_x size of the grayplot for abscisses
   \param size_y size of the grayplot for ordinates
 */
void scoAddGrayplotForShortDraw(ScopeMemory * pScopeMemory, int i, int j, int size_x, int size_y);

/**
   \brief Add a plot3d for a ShortDraw
   \param pScopeMemory  a pointer on the ScopeMemory
   \param i the number of the axes
   \param j the number of the designated shortdraw in the axes i
   \param size_x size of the plot3d for abscisses
   \param size_y size of the plot3d for ordinates
*/
void scoAddPlot3dForShortDraw(ScopeMemory * pScopeMemory, int i, int j, int size_x, int size_y);

/*-------------------------------ADDING COUPLE FUNCTIONS-----------------------------------*/

/**
   \brief Add a couple of polylines one for shortdraw and one for longdraw
   \param pScopeMemory  a pointer on the ScopeMemory
   \param colors a table of colors of all curves in the ScopeMemory
   \attention this function use the value of number_of_subwin and number_of_curves_by_subwin to fill all allocated space with shortdraw and longdraw typed with polylines
*/
void scoAddCoupleOfPolylines(ScopeMemory * pScopeMemory, int * colors);

/**
   \brief Add a couple of polyline one for the ShortDraw and one for the LongDraw they are lined
   \param pScopeMemory a pointer on a ScopeMemory
   \param color a table of (number_of_curves_by_subwin[0]) size to colorize segments
   \attention this function use the value of number_of_subwin and number_of_curves_by_subwin to fill all allocated space with shortdraw and longdraw typed with segments
*/
void scoAddCoupleOfSegments(ScopeMemory * pScopeMemory, int * color);

/**
   \brief Add a couple of spheres to the axis
   \param pScopeMemory a pointer on a ScopeMemory
   \param radius a table of radii for all spheres
   \param colors a table of colors
   \attention this function use the value of number_of_subwin and number_of_curves_by_subwin to fill all allocated space with shortdraw and longdraw typed with segment
*/
void scoAddCoupleOfSpheres(ScopeMemory * pScopeMemory, double * radius, int * colors);

/*-------------------------------DELETING FUNCTIONS---------------------------*/
/**
   \brief Del a couple of polyline one for the ShortDraw and one for the LongDraw they are linked
   \param pScopeMemory a pointer on a ScopeMemory
*/
void scoDelCoupleOfPolylines(ScopeMemory * pScopeMemory);

/**
   \brief Del a couple of polyline one for the ShortDraw and one for the LongDraw they are linked
   \param pScopeMemory a pointer on a ScopeMemory
*/
void scoDelCoupleOfSegments(ScopeMemory * pScopeMemory);

/* ----------------------DRAWING FUNCTIONS --------------------------------*/
/*
These functions modify the view of the scope. They all have at a moment an instruction to redraw a graphical objet or the whole window.
 */
/**
   \brief Draw a Scope like CSCOPE : Amplitude + Time Based
   \param pScopeMemory a pointer on a ScopeMemory
   \param t the scicos time (get_scicos_time())
*/
void scoDrawScopeAmplitudeTimeStyle(ScopeMemory * pScopeMemory, double t);

/**
   \brief Draw a Scope like CSCOPXY or CSCOPXY3D
   \param pScopeMemory a pointer on a ScopeMemory
 */
void scoDrawScopeXYStyle(ScopeMemory * pScopeMemory);
/**
   \brief Add Titles and Backgound on the scope
   \param pScopeMemory a pointer on a ScopeMemory
   \param x a string to be printed on x
   \param y a string to be printed on y
   \param z a string to be printed on z (can be NULL)
 */
void scoAddTitlesScope(ScopeMemory * pScopeMemory,char * x, char * y, char * z);

/**
   \brief Draw a Scope libe ANIMXY or ANIMXY3D
   \param pScopeMemory a pointer on a ScopeMemory
   \param u1 values on first entry 
   \param u2 values on second entry
   \param u3 values on third entry (can be NULL)
 */
void scoDrawScopeAnimXYStyle(ScopeMemory * pScopeMemory, double * u1, double * u2, double * u3);

/**
   \brief Refresh the DataBounds of X if we have touched the end of the x-axes
   \param pScopeMemory a pointer on a ScopeMemory
   \param t the scicos time (get_scicos_time())
*/
void scoRefreshDataBoundsX(ScopeMemory * pScopeMemory,double t);

#endif
