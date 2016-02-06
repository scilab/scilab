/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.modules.graphic_objects.axes;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

/**
 * Camera class
 * @author Manuel JULIACHS
 */
public class Camera {
    /** Default rotation angles */
    public static final double[] DEFAULT_ROTATION_ANGLES = {0.0, 270.0};

    /** Camera properties names */
    public enum CameraProperty { VIEW, ISOVIEW, CUBESCALING, ROTATIONANGLES, ROTATIONANGLES3D };

    /** View type */
    public enum ViewType { VIEW_2D, VIEW_3D;

                           /**
                            * Converts an integer to the corresponding enum
                            * @param intValue the integer value
                            * @return the view type type enum
                            */
    public static ViewType intToEnum(Integer intValue) {
        switch (intValue) {
            case 0:
                return ViewType.VIEW_2D;
            case 1:
                return ViewType.VIEW_3D;
            default:
                return null;
        }
    }
                         }

    /** Specifies whether the projection must be performed for a 2D or a 3D scene */
    private ViewType view;

    /** Specifies whether an isoview transformation must be applied or not */
    private boolean isoview;

    /** Specifies whether a unit cube transformation must be performed */
    private boolean cubeScaling;

    /** 2-element array (alpha and theta rotation angles) */
    private double[] rotationAngles;

    /**
     * Last know values of the rotation angles when View was equal to VIEW_3D
     * 2-element array (alpha and theta)
     */
    private double[] rotationAngles3d;

    /** Constructor */
    public Camera() {
        view = ViewType.VIEW_2D;
        isoview = false;
        cubeScaling = false;
        rotationAngles = new double[2];
        rotationAngles3d = new double[2];
    }

    /**
     * Copy constructor
     * @param camera the Camera to copy
     */
    public Camera(Camera camera) {
        view = camera.view;
        isoview = camera.isoview;
        cubeScaling = camera.cubeScaling;

        rotationAngles = new double[2];

        rotationAngles[0] = camera.rotationAngles[0];
        rotationAngles[1] = camera.rotationAngles[1];

        rotationAngles3d = new double[2];

        rotationAngles3d[0] = camera.rotationAngles3d[0];
        rotationAngles3d[1] = camera.rotationAngles3d[1];
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof Camera) {
            Camera c = (Camera) o;
            return view == c.view && isoview == c.isoview && cubeScaling == c.cubeScaling && rotationAngles[0] == c.rotationAngles[0] && rotationAngles[1] == c.rotationAngles[1] && rotationAngles3d[0] == c.rotationAngles3d[0] && rotationAngles3d[1] == c.rotationAngles3d[1];
        }

        return false;
    }

    /**
     * @return the cubeScaling
     */
    public Boolean getCubeScaling() {
        return cubeScaling;
    }

    /**
     * @param cubeScaling the cubeScaling to set
     */
    public UpdateStatus setCubeScaling(Boolean cubeScaling) {
        if (this.cubeScaling != cubeScaling) {
            this.cubeScaling = cubeScaling;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the isoview
     */
    public Boolean getIsoview() {
        return isoview;
    }

    /**
     * @param isoview the isoview to set
     */
    public UpdateStatus setIsoview(Boolean isoview) {
        if (this.isoview != isoview) {
            this.isoview = isoview;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the rotationAngles in degree
     */
    public Double[] getRotationAngles() {
        Double [] retRotationAngles = new Double[2];

        retRotationAngles[0] = rotationAngles[0];
        retRotationAngles[1] = rotationAngles[1];

        return retRotationAngles;
    }

    /**
     * @param rotationAngles the rotationAngles to set
     * @return true if the rotation angles have changed.
     */
    public boolean setRotationAngles(Double[] rotationAngles) {
        if ((this.rotationAngles[0] != rotationAngles[0]) || (this.rotationAngles[1] != rotationAngles[1])) {
            if (rotationAngles[0] == DEFAULT_ROTATION_ANGLES[0] && rotationAngles[1] == DEFAULT_ROTATION_ANGLES[1]) {
                view = ViewType.VIEW_2D;
            } else if (view == ViewType.VIEW_2D) {
                view = ViewType.VIEW_3D;
            }

            this.rotationAngles[0] = rotationAngles[0];
            this.rotationAngles[1] = rotationAngles[1];
            return true;
        } else {
            return false;
        }
    }

    /**
     * @return the 3d rotationAngles
     */
    public Double[] getRotationAngles3d() {
        Double [] retRotationAngles3d = new Double[2];

        retRotationAngles3d[0] = rotationAngles3d[0];
        retRotationAngles3d[1] = rotationAngles3d[1];

        return retRotationAngles3d;
    }

    /**
     * @param rotationAngles3d the 3d rotationAngles to set
     */
    public UpdateStatus setRotationAngles3d(Double[] rotationAngles3d) {
        if (this.rotationAngles3d[0] != rotationAngles3d[0] || this.rotationAngles3d[1] != rotationAngles3d[1]) {
            this.rotationAngles3d[0] = rotationAngles3d[0];
            this.rotationAngles3d[1] = rotationAngles3d[1];
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the view
     */
    public ViewType getView() {
        return view;
    }

    /**
     * @param view the view to set
     */
    public UpdateStatus setView(ViewType view) {
        if (this.view != view) {
            if (view == ViewType.VIEW_2D) {
                rotationAngles3d[0] = rotationAngles[0];
                rotationAngles3d[1] = rotationAngles[1];

                rotationAngles[0] = DEFAULT_ROTATION_ANGLES[0];
                rotationAngles[1] = DEFAULT_ROTATION_ANGLES[1];
            } else if (view == ViewType.VIEW_3D && this.view == ViewType.VIEW_2D) {
                rotationAngles[0] = rotationAngles3d[0];
                rotationAngles[1] = rotationAngles3d[1];
            }
            this.view = view;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }
}
