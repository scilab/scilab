/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
* desc : Class containing the driver independant routine to draw a legend object
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "ConcreteDrawableLegend.hxx"
#include "getHandleDrawer.h"

extern "C"
{
#include "BuildObjects.h"
#include "DestroyObjects.h"
#include "GetProperty.h"
#include "GetJavaProperty.h"
#include "SetProperty.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "math_graphics.h"
#include "HandleManagement.h"
}

#define DEPTH_FRONT 0.01
#define DEPTH_OFFSET 0.001

namespace sciGraphics
{
	using namespace std;

	/*---------------------------------------------------------------------------------*/
	ConcreteDrawableLegend::ConcreteDrawableLegend(sciPointObj * pLegend)
		: DrawableLegend(pLegend)
	{
		m_pNames = NULL;
		m_aLines = NULL;
		m_aBox = NULL;
	}
	/*---------------------------------------------------------------------------------*/
	ConcreteDrawableLegend::~ConcreteDrawableLegend(void)
	{
		destroyText();
		destroyLines();
		destroyBox();
	}
	/*---------------------------------------------------------------------------------*/
	void ConcreteDrawableLegend::familyHasChanged(void)
	{
		int nbLegends = getNbLegend();
		DrawableObject::familyHasChanged();

		if (m_aBox) {
			getHandleDrawer(m_aBox)->familyHasChanged();
		}

		if (m_pNames) {
			getHandleDrawer(m_pNames)->familyHasChanged();
		}

		if (m_aLines) {
			for (int i = 0; i < nbLegends; i++)
			{
				getHandleDrawer(m_aLines[i])->familyHasChanged();
			}
		}

	}
	/*---------------------------------------------------------------------------------*/
	void ConcreteDrawableLegend::drawLegend(void)
	{
		createBox();
		setBoxParameters();
		setTextParameters();
		createLines();
		setLinesParameters();

		double corner1[3];
		double corner2[3];
		double corner3[3];
		double corner4[3];
		double boxCorner1[3];
		double boxCorner2[3];
		double boxCorner3[3];
		double boxCorner4[3];

		// compute the box in which to put the legend
		getBoxes("draw",corner1, corner2, corner3, corner4, boxCorner1, boxCorner2, boxCorner3, boxCorner4);

		// place box before to draw it behind
		placeText(corner1, corner2, corner3, corner4);
		placeLines(corner1, corner2, corner3, corner4);
		placeBox(boxCorner1, boxCorner2, boxCorner3, boxCorner4);

		// draw box
		getHandleDrawer(m_aBox)->hasChanged();
		getHandleDrawer(m_aBox)->display();

		// draw names
		getHandleDrawer(m_pNames)->hasChanged();
		getHandleDrawer(m_pNames)->display();

		// draw lines
		int nbLegends = getNbLegend();
		for (int i = 0; i < nbLegends; i++)
		{
			getHandleDrawer(m_aLines[i])->hasChanged();
			getHandleDrawer(m_aLines[i])->display();
		}
		
	}
	/*---------------------------------------------------------------------------------*/
	int ConcreteDrawableLegend::updateLegend(void)
	{
		//check if all legended objects still exist and update Legend object if necessary
		int i1 = 0;

		StringMatrix * Text=pLEGEND_FEATURE(m_pDrawed)->text.pStrings ;

		int nblegends = getNbLegend();

		/* The line handles and text data arrays are in reverse order of one another, which requires them
		to be searched and updated in reverse directions */
		for (int i = 0; i < nblegends; i++) {
			sciPointObj * legendedObject = sciGetPointerFromHandle((long)(pLEGEND_FEATURE(m_pDrawed)->tabofhandles[i]));
			if (legendedObject != NULL) {
				if (i != i1) {
					pLEGEND_FEATURE(m_pDrawed)->tabofhandles[i1]= pLEGEND_FEATURE(m_pDrawed)->tabofhandles[i];
					Text->data[nblegends-1-i1]=Text->data[nblegends-1-i];
				}

				i1++;
			}

		}

		/* Shift the text data array remaining elements in order to finish updating it */
		for (int i = 0; i < i1; i++) {
			Text->data[i] = Text->data[i+nblegends-i1];
		}

		//at least one legended object has been destroyed, update Legend object and recreate display
		if (i1 < nblegends) {
			destroyText();
			destroyLines();
			destroyBox();
			Text->nbRow=i1;
			pLEGEND_FEATURE(m_pDrawed)->nblegends=i1;
			drawLegend();
			return 1;
		}
		return 0;
	}
	/*---------------------------------------------------------------------------------*/

	void ConcreteDrawableLegend::showLegend(void)
	{
		//check if all legended objects still exist and update Legend object if necessary
		if (updateLegend()) return;

		// just do that in order to be able to retrieve bounding box
		sciSetTextPos(m_pNames, 1.0, 1.0, 1.0);
		// line parameters may have changed
		setLinesParameters();
		// Box parameters may have changed
		setBoxParameters();

		// position always change
		double corner1[3];
		double corner2[3];
		double corner3[3];
		double corner4[3];
		double boxCorner1[3];
		double boxCorner2[3];
		double boxCorner3[3];
		double boxCorner4[3];

		// compute the box in which to put the legend
		getBoxes("show",corner1, corner2, corner3, corner4,boxCorner1, boxCorner2, boxCorner3, boxCorner4);
		placeText(corner1, corner2, corner3, corner4);
		placeLines(corner1, corner2, corner3, corner4);
		placeBox(boxCorner1, boxCorner2, boxCorner3, boxCorner4);

		// draw box
		getHandleDrawer(m_aBox)->hasChanged();
		getHandleDrawer(m_aBox)->display();

		// draw names
		getHandleDrawer(m_pNames)->hasChanged();
		getHandleDrawer(m_pNames)->display();

		// draw lines
		int nbLegends = getNbLegend();
		for (int i = 0; i < nbLegends; i++)
		{
			getHandleDrawer(m_aLines[i])->hasChanged();
			getHandleDrawer(m_aLines[i])->display();
		}

	}
	/*---------------------------------------------------------------------------------*/
	void ConcreteDrawableLegend::setTextParameters(void)
	{
		destroyText();

		sciLegend * ppLegend = pLEGEND_FEATURE(m_pDrawed);
		int nbLegends = ppLegend->nblegends;

		// get the text of the legend
		char ** textMat = getStrMatData(sciGetText(m_pDrawed));

		// create a text object and set its data from the legend object
		int background = sciGetBackground(m_pDrawed);
		int foreground = sciGetForeground(m_pDrawed);

		m_pNames = allocateText(sciGetParentSubwin(m_pDrawed), textMat, nbLegends, 1,
			0.0, 0.0, TRUE, NULL, FALSE, &foreground, &background,
			FALSE, FALSE, FALSE, ALIGN_LEFT);

		sciInitFontSize(m_pNames, sciGetFontSize(m_pDrawed));
		sciInitFontForeground(m_pNames, sciGetFontForeground(m_pDrawed));
		sciInitFontBackground(m_pNames, sciGetBackground(m_pDrawed));
		sciSetFontStyle(m_pNames, sciGetFontStyle(m_pDrawed));
		sciSetFontOrientation(m_pNames, 0.0); // always horizontal
		sciInitIsClipping(m_pNames, sciGetIsClipping(m_pDrawed));
		sciSetClipping(m_pNames, sciGetClipping(m_pDrawed));

		sciSetTextPos(m_pNames, 1.0, 1.0, 1.0);
	}
	/*---------------------------------------------------------------------------------*/
	/*---------------------------------------------------------------------------------*/
	void ConcreteDrawableLegend::setBoxParameters(void)
	{
		// set same mark parameters as the polyline
		sciInitMarkSize(m_aBox, sciGetMarkSize(m_pDrawed));
		sciInitMarkSizeUnit(m_aBox, sciGetMarkSizeUnit(m_pDrawed));
		sciInitMarkForeground(m_aBox, sciGetMarkForeground(m_pDrawed));
		sciInitMarkBackground(m_aBox, sciGetMarkBackground(m_pDrawed));
		sciInitIsMark(m_aBox, sciGetIsMark(m_pDrawed));
		sciInitMarkStyle(m_aBox,sciGetMarkStyle(m_pDrawed));

		// same for lines
		sciInitForeground(m_aBox, sciGetForeground(m_pDrawed));
		sciInitBackground(m_aBox, sciGetBackground(m_pDrawed));
		sciInitLineWidth(m_aBox, sciGetLineWidth(m_pDrawed));
		sciInitLineStyle(m_aBox, sciGetLineStyle(m_pDrawed));
		sciInitIsLine(m_aBox, sciGetIsLine(m_pDrawed));
		sciInitIsFilled(m_aBox,sciGetIsFilled(m_pDrawed));
		// use clip state of legend object
		sciInitIsClipping(m_aBox, sciGetIsClipping(m_pDrawed));
		sciSetClipping(m_aBox, sciGetClipping(m_pDrawed));



	}
	/*---------------------------------------------------------------------------------*/
	void ConcreteDrawableLegend::createLines(void)
	{
		// create nbLegends polylines
		destroyLines();
		int nbLegends = pLEGEND_FEATURE(m_pDrawed)->nblegends;
		sciPointObj * parentSubwin = sciGetParentSubwin(m_pDrawed);
		double defaultCoords[4] = {0.0, 0.0, 0.0, 0.0};

		m_aLines = new sciPointObj *[nbLegends];

		for (int i = 0; i < nbLegends; i++)
		{
			m_aLines[i] = allocatePolyline(parentSubwin, defaultCoords, defaultCoords, defaultCoords, 0, 4, 1,
				NULL, NULL, NULL, NULL, NULL, FALSE, FALSE, FALSE, FALSE);
		}

	}
	/*---------------------------------------------------------------------------------*/
	void ConcreteDrawableLegend::createBox(void)
	{
		// create the box rectangle
		destroyBox();
		sciPointObj * parentSubwin = sciGetParentSubwin(m_pDrawed);
		double defaultCoords[4] = {0.0, 0.0, 0.0, 0.0};
		m_aBox = allocatePolyline(parentSubwin, defaultCoords, defaultCoords, defaultCoords, 1, 4, 1,
			NULL, NULL, NULL, NULL, NULL, FALSE, FALSE, FALSE, FALSE);

	}

	/*---------------------------------------------------------------------------------*/
	void ConcreteDrawableLegend::setLinesParameters(void)
	{
		int nbLegends = getNbLegend();
		sciLegend * ppLegend = pLEGEND_FEATURE(m_pDrawed);
		for (int i = 0; i < nbLegends; i++)
		{
			// current object we want to set the legend
			sciPointObj * legendedObject = sciGetPointerFromHandle((long)(ppLegend->tabofhandles[i]));
			// set same mark parameters as the polyline
			sciInitMarkSize(m_aLines[i], sciGetMarkSize(legendedObject));
			sciInitMarkSizeUnit(m_aLines[i], sciGetMarkSizeUnit(legendedObject));
			sciInitMarkForeground(m_aLines[i], sciGetMarkForeground(legendedObject));
			sciInitMarkBackground(m_aLines[i], sciGetMarkBackground(legendedObject));
			sciInitIsMark(m_aLines[i], sciGetIsMark(legendedObject));
			sciInitMarkStyle(m_aLines[i],sciGetMarkStyle(legendedObject));

			// same for lines
			if (sciGetPolylineStyle(legendedObject)==4){ //arrowed polyline
				sciInitPolylineStyle(m_aLines[i],sciGetPolylineStyle(legendedObject));
				sciInitArrowSize(m_aLines[i],sciGetArrowSize(legendedObject));
			}
			sciInitForeground(m_aLines[i], sciGetForeground(legendedObject));
			sciInitBackground(m_aLines[i], sciGetBackground(legendedObject));
			sciInitLineWidth(m_aLines[i], sciGetLineWidth(legendedObject));
			sciInitLineStyle(m_aLines[i], sciGetLineStyle(legendedObject));

			if (   sciGetIsDisplayingLines(legendedObject)
				  || sciGetPolylineStyle(legendedObject) == 3
					|| sciGetPolylineStyle(legendedObject) == 6)
			{
				// polyline with line drawing or bar plot
				sciInitIsLine(m_aLines[i], TRUE);
			}
			else
			{
				sciInitIsLine(m_aLines[i], FALSE);
			}

			// use clip state of legend object
			sciInitIsClipping(m_aLines[i], sciGetIsClipping(m_pDrawed));
			sciSetClipping(m_aLines[i], sciGetClipping(m_pDrawed));
		}
	}
	/*---------------------------------------------------------------------------------*/
	void ConcreteDrawableLegend::destroyLines(void)
	{
		if (m_aLines != NULL)
		{

			for (int i = 0; i < pLEGEND_FEATURE(m_pDrawed)->nblegends; i++)
			{
				deallocatePolyline(m_aLines[i]);
				m_aLines[i] = NULL;
			}
			delete[] m_aLines;
			m_aLines = NULL;
		}

	}
	/*---------------------------------------------------------------------------------*/
	void ConcreteDrawableLegend::destroyText(void)
	{
		if (m_pNames != NULL)
		{
			deallocateText(m_pNames);
			m_pNames = NULL;
		}
	}
	/*---------------------------------------------------------------------------------*/
	void ConcreteDrawableLegend::destroyBox(void)
	{
		if (m_aBox != NULL)
		{
			deallocatePolyline(m_aBox);
			m_aBox = NULL;
		}
	}

	/*---------------------------------------------------------------------------------*/
	void ConcreteDrawableLegend::getBoxes(const string & opt, double upperLeftCorner[3], double lowerLeftCorner[3],
		double lowerRightCorner[3], double upperRightCorner[3],
		double upperBoxLeftCorner[3], double lowerBoxLeftCorner[3],
		double lowerBoxRightCorner[3], double upperBoxRightCorner[3])
	{
		sciPointObj * parentSubwin = sciGetParentSubwin(m_pNames);

		// get viewing area used by axes
		int axesXpos;
		int axesYpos;
		int axesWidth;
		int axesHeight;
		sciGetViewingArea(parentSubwin, &axesXpos, &axesYpos, &axesWidth, &axesHeight);
		// get text bounding box
		double corner1[3];
		double corner2[3];
		double corner3[3];
		double corner4[3];
		if (opt == "draw")
		{
			getTextDrawer(m_pNames)->getScreenBoundingBox(corner1, corner2, corner3,corner4);
		}
		else
		{
			//sciGetPixelBoundingBox(m_pNames, corner1, corner2, corner3, corner4);
			getTextDrawer(m_pNames)->getScreenBoundingBox(corner1, corner2, corner3,corner4);
		}
		// the height of the box equals the text height
		double textBoxPixelHeight = Abs(corner1[1] - corner3[1]);
		double textBoxPixelWidth  = Abs(corner3[0] - corner1[0]);

		// length in pixel of the lines
		double linePixelLength = axesWidth / 10.0;
		// offset used to separate lines and text as well as bounding box
		double xOffset = linePixelLength/8.0;
		double yOffset = axesHeight/100.0;

		// the legend bounding box width
		double pixelBoxWidth = xOffset + linePixelLength + xOffset + textBoxPixelWidth + xOffset;
		double pixelBoxHeight = yOffset +  textBoxPixelHeight + yOffset;

		// we need to find a suitable z pixel coordinate which won't be clipped
		// by near or far plane. We chose one of the axes bounds.
		double axesBounds[6];
		sciGetDataBounds(parentSubwin, axesBounds);

		// put the Z coordinate in front of Z buffer
		double pixelDepth = DEPTH_FRONT;

		// we got all the information we need, we can compute the 4 corners

		// First set the upper left point of the bounding box according to selected place option
		upperBoxLeftCorner[2] = pixelDepth;
		switch(sciGetLegendPlace(m_pDrawed))  {
		case  SCI_LEGEND_OUT_UPPER_RIGHT: 
			upperBoxLeftCorner[0] = axesXpos + axesWidth + xOffset;
			upperBoxLeftCorner[1] = axesYpos;
			if (sciGetyLocation(parentSubwin)=='r') 
				upperBoxLeftCorner[0] = upperBoxLeftCorner[0] + 10*xOffset; //should be improved computing the ticks_label boundingbox
			break;
		case  SCI_LEGEND_OUT_UPPER_LEFT: 
			upperBoxLeftCorner[0] = axesXpos - pixelBoxWidth - xOffset  ;
			upperBoxLeftCorner[1] = axesYpos; 
			if (sciGetyLocation(parentSubwin)=='l') 
				upperBoxLeftCorner[0] = upperBoxLeftCorner[0] - 10*xOffset; //should be improved  computing the ticks_label boundingbox
			break;
		case  SCI_LEGEND_OUT_LOWER_RIGHT: 
			upperBoxLeftCorner[0] = axesXpos + axesWidth  + xOffset;
			upperBoxLeftCorner[1] = axesYpos + axesHeight - pixelBoxHeight;
			if (sciGetyLocation(parentSubwin)=='r') 
				upperBoxLeftCorner[0] = upperBoxLeftCorner[0] + 11*xOffset; //should be improved  computing the ticks_label boundingbox
			break;
		case SCI_LEGEND_OUT_LOWER_LEFT: 
			upperBoxLeftCorner[0] = axesXpos - pixelBoxWidth - xOffset ;
			upperBoxLeftCorner[1] = axesYpos + axesHeight - pixelBoxHeight;
			if (sciGetyLocation(parentSubwin)=='l') 
				upperBoxLeftCorner[0] = upperBoxLeftCorner[0] - 11*xOffset; //should be improved computing the ticks_label boundingbox
			break;
		case  SCI_LEGEND_IN_UPPER_RIGHT: 
			upperBoxLeftCorner[0] = axesXpos + axesWidth  - pixelBoxWidth - xOffset;
			upperBoxLeftCorner[1] = axesYpos + yOffset;
			break;
		case  SCI_LEGEND_IN_UPPER_LEFT: 
			upperBoxLeftCorner[0] = axesXpos + xOffset;
			upperBoxLeftCorner[1] = axesYpos + yOffset;
			break;
		case  SCI_LEGEND_IN_LOWER_RIGHT: 
			upperBoxLeftCorner[0] = axesXpos + axesWidth  - pixelBoxWidth - xOffset ;
			upperBoxLeftCorner[1] = axesYpos + axesHeight - pixelBoxHeight - yOffset;
			break;
		case  SCI_LEGEND_IN_LOWER_LEFT: 
			upperBoxLeftCorner[0] = axesXpos + xOffset;
			upperBoxLeftCorner[1] = axesYpos + axesHeight - pixelBoxHeight - yOffset;
			break;
		case  SCI_LEGEND_LOWER_CAPTION: 
			upperBoxLeftCorner[0] = axesXpos;
			upperBoxLeftCorner[1] = axesYpos + axesHeight + yOffset; 
			if (sciGetxLocation(parentSubwin)=='d') 
				upperBoxLeftCorner[1] = upperBoxLeftCorner[1] +8*yOffset; //should be improved computing the ticks_label boundingbox
			break;
		case  SCI_LEGEND_UPPER_CAPTION: 
			upperBoxLeftCorner[0] = axesXpos;
			upperBoxLeftCorner[1] = axesYpos - pixelBoxHeight - yOffset;
			if (sciGetxLocation(parentSubwin)=='u') 
				upperBoxLeftCorner[1] = upperBoxLeftCorner[1] - 8*yOffset; //should be improved
			break;
		case  SCI_LEGEND_BY_COORDINATES:
			{ //The upper left corner of the legend position is given by its relative position with 
				//respect to the upper left corner of the axes frame.
				int size[2] ;
				sciGetJavaFigureSize(sciGetParent(parentSubwin), size) ;//get canvas width and height
				double *wrect=sciGetAxesBounds(parentSubwin);//get canvas portion used by current axes
				double pos[2];
				sciGetLegendPos (m_pDrawed,pos);
				upperBoxLeftCorner[0] = (wrect[0]+ pos[0]*(wrect[2]))*size[0] ;
				upperBoxLeftCorner[1] = (wrect[1]+ pos[1]*(wrect[3]))*size[1] ;
				break;
			}
		}
		if (sciGetLegendPlace(m_pDrawed) != SCI_LEGEND_BY_COORDINATES) {
			//set the computed position in Legend data structure
			int size[2] ;
			sciGetJavaFigureSize(sciGetParent(parentSubwin), size) ;//get canvas width and height
			double *wrect=sciGetAxesBounds(parentSubwin);//get canvas portion used by current axes
			double pos[2];
			pos[0]=(upperBoxLeftCorner[0]/size[0]-wrect[0])/(wrect[2]);
			pos[1]=(upperBoxLeftCorner[1]/size[1]-wrect[1])/(wrect[3]);
			sciSetLegendPos (m_pDrawed,pos);

		}

		// lower left point of the bounding box
		lowerBoxLeftCorner[0] = upperBoxLeftCorner[0];
		lowerBoxLeftCorner[1] = upperBoxLeftCorner[1] + pixelBoxHeight;
		lowerBoxLeftCorner[2] = pixelDepth;

		//  lower right point of the bounding box
		lowerBoxRightCorner[0] = upperBoxLeftCorner[0] + pixelBoxWidth;
		lowerBoxRightCorner[1] = lowerBoxLeftCorner[1];
		lowerBoxRightCorner[2] = pixelDepth;

		// upper right point  of the bounding box
		upperBoxRightCorner[0] = lowerBoxRightCorner[0];
		upperBoxRightCorner[1] = upperBoxLeftCorner[1];
		upperBoxRightCorner[2] = pixelDepth;


		// upper left point of the Lines box
		upperLeftCorner[0] = upperBoxLeftCorner[0] + xOffset;
		upperLeftCorner[1] = upperBoxLeftCorner[1] + yOffset;
		upperLeftCorner[2] = pixelDepth;

		// lower left point of the Lines box
		lowerLeftCorner[0] = upperLeftCorner[0];
		lowerLeftCorner[1] = upperLeftCorner[1] + textBoxPixelHeight;
		lowerLeftCorner[2] = pixelDepth;

		// lower right point of the Lines box
		lowerRightCorner[0] = upperLeftCorner[0] + linePixelLength;
		lowerRightCorner[1] = lowerLeftCorner[1];
		lowerRightCorner[2] = pixelDepth;

		// upper right point of the Lines box
		upperRightCorner[0] = lowerRightCorner[0];
		upperRightCorner[1] = upperLeftCorner[1];
		upperRightCorner[2] = pixelDepth;

	}
	/*---------------------------------------------------------------------------------*/
	void ConcreteDrawableLegend::placeLines(const double upperLeftCorner[3], const double lowerLeftCorner[3],
		const double lowerRightCorner[3], const double upperRightCorner[3])
	{

		int nblegends = getNbLegend();

		Camera * curCam = getCamera();

		// interpolate between top to bottom
		for (int i = 0; i < nblegends; i++)
		{
			sciPointObj * legendedObject = sciGetPointerFromHandle((long)(pLEGEND_FEATURE(m_pDrawed)->tabofhandles[i]));

			// polyline used for legend
			sciPolyline * curPoly = pPOLYLINE_FEATURE(m_aLines[i]);

			// our polylines are composed of at most four points
			double curPolyData[4][3];

			// interpolate on left side
			double interpolationFactor = (i + 0.5) / nblegends;

			if (   sciGetPolylineStyle(legendedObject) == 5
				  || sciGetPolylineStyle(legendedObject) == 6
					|| sciGetIsFilled(legendedObject))
			{
				// filled polyline or bar
				double shift=0.25/nblegends;
				curPolyData[0][0] = lowerLeftCorner[0] + (upperLeftCorner[0] - lowerLeftCorner[0]) * (interpolationFactor+shift);
				curPolyData[0][1] = lowerLeftCorner[1] + (upperLeftCorner[1] - lowerLeftCorner[1]) * (interpolationFactor+shift);
				// use depth offset to avoid depth fitting with box background
				curPolyData[0][2] = lowerLeftCorner[2] + (upperLeftCorner[2] - lowerLeftCorner[2]) * (interpolationFactor+shift) - DEPTH_OFFSET;

				curPolyData[1][0] = lowerRightCorner[0] + (upperRightCorner[0] - lowerRightCorner[0]) * (interpolationFactor+shift);
				curPolyData[1][1] = lowerRightCorner[1] + (upperRightCorner[1] - lowerRightCorner[1]) * (interpolationFactor+shift);
				curPolyData[1][2] = lowerRightCorner[2] + (upperRightCorner[2] - lowerRightCorner[2]) * (interpolationFactor+shift) - DEPTH_OFFSET;

				curPolyData[2][0] = lowerRightCorner[0] + (upperRightCorner[0] - lowerRightCorner[0]) * (interpolationFactor-shift);
				curPolyData[2][1] = lowerRightCorner[1] + (upperRightCorner[1] - lowerRightCorner[1]) * (interpolationFactor-shift);
				curPolyData[2][2] = lowerRightCorner[2] + (upperRightCorner[2] - lowerRightCorner[2]) * (interpolationFactor-shift) - DEPTH_OFFSET;

				curPolyData[3][0] = lowerLeftCorner[0] + (upperLeftCorner[0] - lowerLeftCorner[0]) * (interpolationFactor-shift);
				curPolyData[3][1] = lowerLeftCorner[1] + (upperLeftCorner[1] - lowerLeftCorner[1]) * (interpolationFactor-shift);
				curPolyData[3][2] = lowerLeftCorner[2] + (upperLeftCorner[2] - lowerLeftCorner[2]) * (interpolationFactor-shift) - DEPTH_OFFSET;

				curPoly->n1 = 4;//set polyline length to the max allocated size
				curPoly->closed = 1;
				sciInitIsFilled(m_aLines[i],TRUE);
			}
			else
			{
				// other kind of polylines
				curPolyData[0][0] = lowerLeftCorner[0] + (upperLeftCorner[0] - lowerLeftCorner[0]) * interpolationFactor;
				curPolyData[0][1] = lowerLeftCorner[1] + (upperLeftCorner[1] - lowerLeftCorner[1]) * interpolationFactor;
				curPolyData[0][2] = lowerLeftCorner[2] + (upperLeftCorner[2] - lowerLeftCorner[2]) * interpolationFactor;

				curPolyData[2][0] = lowerRightCorner[0] + (upperRightCorner[0] - lowerRightCorner[0]) * interpolationFactor;
				curPolyData[2][1] = lowerRightCorner[1] + (upperRightCorner[1] - lowerRightCorner[1]) * interpolationFactor;
				curPolyData[2][2] = lowerRightCorner[2] + (upperRightCorner[2] - lowerRightCorner[2]) * interpolationFactor;

				curPolyData[1][0] = 0.5 * (curPolyData[0][0] + curPolyData[2][0]);
				curPolyData[1][1] = 0.5 * (curPolyData[0][1] + curPolyData[2][1]);
				curPolyData[1][2] = 0.5 * (curPolyData[0][2] + curPolyData[2][2]);

				curPoly->n1 = 3; //restrict polyline length the  first three points
				curPoly->closed = 0;
				sciInitIsFilled(m_aLines[i],FALSE);
			}

			// convert polyline to user coords
			for (int i = 0; i < curPoly->n1; i++)
			{
				curCam->getSceneCoordinates(curPolyData[i], curPolyData[i]);
				curPoly->pvx[i] = curPolyData[i][0];
				curPoly->pvy[i] = curPolyData[i][1];
				curPoly->pvz[i] = curPolyData[i][2];
			}

		}

	}
	/*---------------------------------------------------------------------------------*/
	void ConcreteDrawableLegend::placeText(const double upperLeftCorner[3], const double lowerLeftCorner[3],
		const double lowerRightCorner[3], const double upperRightCorner[3])
	{
		// use the lowerLeftcorner, but move a little to the right
		double xOffset = 0.2 * (lowerRightCorner[0] - lowerLeftCorner[0]);

		double finalCoord[3] = {lowerRightCorner[0] + xOffset,
			                      lowerRightCorner[1],
														lowerRightCorner[2]};
		
		// convert coordinates to scene ones
		getCamera()->getSceneCoordinates(finalCoord, finalCoord);

		sciSetTextPos(m_pNames, finalCoord[0], finalCoord[1], finalCoord[2]);

	}
	/*---------------------------------------------------------------------------------*/
	void ConcreteDrawableLegend::placeBox(const double upperLeftCorner[3], const double lowerLeftCorner[3],
		const double lowerRightCorner[3], const double upperRightCorner[3])
	{
		sciPolyline * curPoly = pPOLYLINE_FEATURE(m_aBox);

		// retrieve scene coordinates
		double tempsPos[3];
		Camera * curCam = getCamera();
		curCam->getSceneCoordinates(lowerLeftCorner, tempsPos);

		curPoly->pvx[0] = tempsPos[0]; 
		curPoly->pvy[0] = tempsPos[1]; 
		curPoly->pvz[0] = tempsPos[2];

		curCam->getSceneCoordinates(upperLeftCorner, tempsPos);

		curPoly->pvx[1] = tempsPos[0]; 
		curPoly->pvy[1] = tempsPos[1]; 
		curPoly->pvz[1] = tempsPos[2];

		curCam->getSceneCoordinates(upperRightCorner, tempsPos);


		curPoly->pvx[2] = tempsPos[0];
		curPoly->pvy[2] = tempsPos[1];
		curPoly->pvz[2] = tempsPos[2];

		curCam->getSceneCoordinates(lowerRightCorner, tempsPos);

		curPoly->pvx[3] = tempsPos[0];
		curPoly->pvy[3] = tempsPos[1];
		curPoly->pvz[3] = tempsPos[2];
	}
	/*---------------------------------------------------------------------------------*/
	Camera * ConcreteDrawableLegend::getCamera(void)
	{
		return getSubwinDrawer(sciGetParentSubwin(m_pDrawed))->getCamera();
	}
	/*---------------------------------------------------------------------------------*/
}
