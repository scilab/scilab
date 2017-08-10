/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
package org.scilab.modules.gui.ged;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
* Handler's Color Map
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class ColorMapHandler {
    private static final double MAXRGB = 255.0;

    /**
    * Get the index corresponding to an RGB code.
    *
    * @param red color.
    * @param green color.
    * @param blue color.
    * @param figureID Enters the identification of Figure.
    * @return Index of the color map corresponding to the color selected by the user.
    */
    public static Integer getScilabColor(double red, double green, double blue, Integer figureID) {
        Double[] selectedColor = rgbTOscilab(red, green, blue);
        Integer scilabColor = searchScilabColor(figureID, selectedColor);
        return scilabColor;
    }

    /**
    * Get the RGB code corresponding to an index.
    *
    * @param scilabColor Index of the color map corresponding to the color current.
    * @param figureID Enters the identification of Figure.
    * @return Code RGB.
    */
    public static Double[] getRGBcolor(Integer figureID, Integer scilabColor) {
        Double[] scilabRGBcolor = searchRGBcolor(figureID, scilabColor);
        Double[] rgbColor = scilabTOrgb(scilabRGBcolor[0], scilabRGBcolor[1], scilabRGBcolor[2]);
        return rgbColor;
    }

    /**
    * Converts the RGB code for scilabRGB.
    *
    * @param red color in RGB code.
    * @param green color in RGB code.
    * @param blue color in RGB code.
    * @return Color in the new format.
    */
    private static Double[] rgbTOscilab(double red, double green, double blue) {
        Double[] selectedColor = new Double[3];
        selectedColor[0] = red / MAXRGB;
        selectedColor[1] = green / MAXRGB;
        selectedColor[2] = blue / MAXRGB;
        return selectedColor;
    }

    /**
    * Converts the scilabRGB code for RGB.
    *
    * @param redSci color.
    * @param greenSci color.
    * @param blueSci color.
    * @return Color in the new format.
    */
    private static Double[] scilabTOrgb(Double redSci, Double greenSci, Double blueSci) {
        Double[] rgbColor = new Double[3];
        rgbColor[0] = redSci * MAXRGB;
        rgbColor[1] = greenSci * MAXRGB;
        rgbColor[2] = blueSci * MAXRGB;
        return rgbColor;
    }

    /**
    * Adds a new color in the colormap.
    *
    * @param size Enters the identification of Figure.
    * @param colormap Current colormap.
    * @param selectedColor Selected color by user.
    * @return New color map with a color insert.
    */
    private static Double[] addNewColor(int size, Double[] colormap, Double[] selectedColor) {
        int newsize = 3 * size + 3;
        Double[] newColorMap = new Double[newsize];
        newColorMap[size] = selectedColor[0];
        newColorMap[2 * size + 1] = selectedColor[1];
        newColorMap[3 * size + 2] = selectedColor[2];
        for (int j = 0; j < newsize - 3; j++) {
            for (int w = 0; w < 3; w++) {
                if (newColorMap[j + w] == null) {
                    newColorMap[j + w] = colormap[j];
                    break;
                }
            }
        }
        return newColorMap;
    }

    /**
    * Checks if there is a color in the color map, if it does not exist, will be added.
    *
    * @param figureID Enters the identification of Figure.
    * @param selectedColor Selected color by user.
    * @return Index of the color map corresponding to the color selected by the user.
    */
    private static Integer searchScilabColor(Integer figureID, Double[] selectedColor) {
        int size = (Integer) GraphicController.getController()
                   .getProperty(figureID, GraphicObjectProperties.__GO_COLORMAP_SIZE__);

        Double[] colormap = (Double[]) GraphicController.getController()
                            .getProperty(figureID, GraphicObjectProperties.__GO_COLORMAP__);

        int scilabColor = 0;

        if (size == 0) {
            GraphicController.getController()
            .setProperty(figureID, GraphicObjectProperties.__GO_COLORMAP__, selectedColor);
            scilabColor = 1;
        }

        for (int i = 0; i < size; i++) {
            if (selectedColor[0].equals(colormap[i])
                    && selectedColor[1].equals(colormap[size + i])
                    && selectedColor[2].equals(colormap[2 * size + i])) {
                scilabColor = i + 1;
                break;
            } else {
                if (i == size - 1) {
                    Double[] newColorMap = addNewColor(size, colormap, selectedColor);
                    GraphicController.getController()
                    .setProperty(figureID, GraphicObjectProperties.__GO_COLORMAP__, newColorMap);
                    scilabColor = i + 2;
                }
            }
        }
        return scilabColor;
    }

    /**
    * Checks the RGB code of an index of a color map.
    *
    * @param figureID Enters the identification of Figure.
    * @param scilabColor Index of the color map corresponding to the color current.
    * @return Color in RGB format converted to the color map of the scilab.
    */
    private static Double[] searchRGBcolor(Integer figureID, Integer scilabColor) {
        int size = (Integer) GraphicController.getController()
                   .getProperty(figureID, GraphicObjectProperties.__GO_COLORMAP_SIZE__);

        Double[] colormap = (Double[]) GraphicController.getController()
                            .getProperty(figureID, GraphicObjectProperties.__GO_COLORMAP__);

        Double[] scilabRGBcolor = new Double[3];
        if (scilabColor < 1 || scilabColor > size) {
            if (scilabColor.equals(-2) || scilabColor.equals(size + 2)) {
                scilabRGBcolor[0] = 1.0;
                scilabRGBcolor[1] = 1.0;
                scilabRGBcolor[2] = 1.0;
            } else {
                scilabRGBcolor[0] = 0.0;
                scilabRGBcolor[1] = 0.0;
                scilabRGBcolor[2] = 0.0;
            }
        } else {
            scilabRGBcolor[0] = colormap[scilabColor - 1];
            scilabRGBcolor[1] = colormap[size + scilabColor - 1];
            scilabRGBcolor[2] = colormap[2 * size + scilabColor - 1];
        }
        return scilabRGBcolor;
    }
}
