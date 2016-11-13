/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos CARDINOT
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
import org.scilab.modules.graphic_objects.graphicView.GraphicView;

/**
 *
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class GEDView implements GraphicView {
    private String lastID = null;

    private static boolean visible = true;
    private static boolean autoresize = true;
    private static Integer pixelDrawingMode = 3;
    private static Integer antialiasing = 0;
    private static Integer rotationType = 0;
    private static Integer clipState = 1;
    private static boolean lineMode = true;
    private static Integer lineStyle = 0;
    private static boolean markMode = false;
    private static Integer markSizeUnit = 1;
    private static Integer dataMapping = 0;
    private static boolean closed = false;
    private static boolean surfaceMode = true;
    private static boolean colored = true;
    private static Integer datatipOrientation = 0;
    private static String datatipDisplayComponents = "";
    private static boolean datatipAutoOrientation = true;
    private static boolean datatipBoxMode = true;
    private static boolean datatipLabelMode = true;
    private static Integer fontStyle = 0;
    private static boolean fontFractional = false;
    private static Integer legendLocation = 0;
    private static boolean autoPosition = true;
    private static boolean autoRotation = true;

    public GEDView() {
        GraphicController.getController().register(this);
    }

    @Override
    public void updateObject(Integer id, int property) {
        if (SwapObject.getLastType() == (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__)) {
            boolean isValid = false;
            switch (property) {
                case GraphicObjectProperties.__GO_VISIBLE__:
                    boolean c_visible = (Boolean) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_VISIBLE__);
                    if (visible != c_visible) {
                        isValid = true;
                        visible = c_visible;
                    }
                    break;
                case GraphicObjectProperties.__GO_AUTORESIZE__:
                    boolean c_autoresize = (Boolean) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_AUTORESIZE__);
                    if (autoresize != c_autoresize) {
                        isValid = true;
                        autoresize = c_autoresize;
                    }
                    break;
                case GraphicObjectProperties.__GO_PIXEL_DRAWING_MODE__:
                    Integer c_pixelDrawingMode = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_PIXEL_DRAWING_MODE__);
                    if (pixelDrawingMode != c_pixelDrawingMode) {
                        isValid = true;
                        pixelDrawingMode = c_pixelDrawingMode;
                    }
                    break;
                case GraphicObjectProperties.__GO_ANTIALIASING__:
                    Integer c_antialiasing = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_ANTIALIASING__);
                    if (antialiasing != c_antialiasing) {
                        isValid = true;
                        antialiasing = c_antialiasing;
                    }
                    break;
                case GraphicObjectProperties.__GO_ROTATION_TYPE__:
                    Integer c_rotationType = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_ROTATION_TYPE__);
                    if (rotationType != c_rotationType) {
                        isValid = true;
                        rotationType = c_rotationType;
                    }
                    break;
                case GraphicObjectProperties.__GO_CLIP_STATE__:
                    Integer c_clipState = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_CLIP_STATE__);
                    if (clipState != c_clipState) {
                        isValid = true;
                        clipState = c_clipState;
                    }
                    break;
                case GraphicObjectProperties.__GO_LINE_MODE__:
                    boolean c_lineMode = (Boolean) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_LINE_MODE__);
                    if (lineMode != c_lineMode) {
                        isValid = true;
                        lineMode = c_lineMode;
                    }
                    break;
                case GraphicObjectProperties.__GO_LINE_STYLE__:
                    Integer c_lineStyle = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_LINE_STYLE__);
                    if (lineStyle != c_lineStyle) {
                        isValid = true;
                        lineStyle = c_lineStyle;
                    }
                    break;
                case GraphicObjectProperties.__GO_MARK_MODE__:
                    boolean c_markMode = (Boolean) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_MARK_MODE__);
                    if (markMode != c_markMode) {
                        isValid = true;
                        markMode = c_markMode;
                    }
                    break;
                case GraphicObjectProperties.__GO_MARK_SIZE_UNIT__:
                    Integer c_markSizeUnit = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_MARK_SIZE_UNIT__);
                    if (markSizeUnit != c_markSizeUnit) {
                        isValid = true;
                        markSizeUnit = c_markSizeUnit;
                    }
                    break;
                case GraphicObjectProperties.__GO_DATA_MAPPING__:
                    Integer c_dataMapping = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_DATA_MAPPING__);
                    if (dataMapping != c_dataMapping) {
                        isValid = true;
                        dataMapping = c_dataMapping;
                    }
                    break;
                case GraphicObjectProperties.__GO_CLOSED__:
                    boolean c_closed = (Boolean) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_CLOSED__);
                    if (closed != c_closed) {
                        isValid = true;
                        closed = c_closed;
                    }
                    break;
                case GraphicObjectProperties.__GO_SURFACE_MODE__:
                    boolean c_surfaceMode = (Boolean) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_SURFACE_MODE__);
                    if (surfaceMode != c_surfaceMode) {
                        isValid = true;
                        surfaceMode = c_surfaceMode;
                    }
                    break;
                case GraphicObjectProperties.__GO_COLORED__:
                    boolean c_colored = (Boolean) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_COLORED__);
                    if (colored != c_colored) {
                        isValid = true;
                        colored = c_colored;
                    }
                    break;
                case GraphicObjectProperties.__GO_DATATIP_ORIENTATION__:
                    Integer c_datatipOrientation = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_DATATIP_ORIENTATION__);
                    if (datatipOrientation != c_datatipOrientation) {
                        isValid = true;
                        datatipOrientation = c_datatipOrientation;
                    }
                    break;
                case GraphicObjectProperties.__GO_DATATIP_DISPLAY_COMPONENTS__:
                    String c_datatipDisplayComponents = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_DATATIP_DISPLAY_COMPONENTS__);
                    if (datatipDisplayComponents != c_datatipDisplayComponents) {
                        isValid = true;
                        datatipDisplayComponents = c_datatipDisplayComponents;
                    }
                    break;
                case GraphicObjectProperties.__GO_DATATIP_AUTOORIENTATION__:
                    boolean c_datatipAutoOrientation = (Boolean) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_DATATIP_AUTOORIENTATION__);
                    if (datatipAutoOrientation != c_datatipAutoOrientation) {
                        isValid = true;
                        datatipAutoOrientation = c_datatipAutoOrientation;
                    }
                    break;
                case GraphicObjectProperties.__GO_DATATIP_BOX_MODE__:
                    boolean c_datatipBoxMode = (Boolean) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_DATATIP_BOX_MODE__);
                    if (datatipBoxMode != c_datatipBoxMode) {
                        isValid = true;
                        datatipBoxMode = c_datatipBoxMode;
                    }
                    break;
                case GraphicObjectProperties.__GO_DATATIP_LABEL_MODE__:
                    boolean c_datatipLabelMode = (Boolean) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_DATATIP_LABEL_MODE__);
                    if (datatipLabelMode != c_datatipLabelMode) {
                        isValid = true;
                        datatipLabelMode = c_datatipLabelMode;
                    }
                    break;
                case GraphicObjectProperties.__GO_FONT_STYLE__:
                    Integer c_fontStyle = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_FONT_STYLE__);
                    if (fontStyle != c_fontStyle) {
                        isValid = true;
                        fontStyle = c_fontStyle;
                    }
                    break;
                case GraphicObjectProperties.__GO_FONT_FRACTIONAL__:
                    boolean c_fontFractional = (Boolean) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_FONT_FRACTIONAL__);
                    if (fontFractional != c_fontFractional) {
                        isValid = true;
                        fontFractional = c_fontFractional;
                    }
                    break;
                case GraphicObjectProperties.__GO_LEGEND_LOCATION__:
                    Integer c_legendLocation = (Integer) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_LEGEND_LOCATION__);
                    if (legendLocation != c_legendLocation) {
                        isValid = true;
                        legendLocation = c_legendLocation;
                    }
                    break;
                case GraphicObjectProperties.__GO_AUTO_POSITION__:
                    boolean c_autoPosition = (Boolean) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_AUTO_POSITION__);
                    if (autoPosition != c_autoPosition) {
                        isValid = true;
                        autoPosition = c_autoPosition;
                    }
                    break;
                case GraphicObjectProperties.__GO_AUTO_ROTATION__:
                    boolean c_autoRotation = (Boolean) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_AUTO_ROTATION__);
                    if (autoRotation != c_autoRotation) {
                        isValid = true;
                        autoRotation = c_autoRotation;
                    }
                    break;
                default:
                    isValid = true;
            }
            if (isValid) {
                Inspector.getInspector(id);
            }
        }
    }

    @Override
    public void createObject(Integer id) {
    }

    @Override
    public void deleteObject(Integer id) {
    }

    public void close() {
        GraphicController.getController().unregister(this);
    }
}
