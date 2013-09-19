/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.texture;

/**
 * @author Pierre Lando
 */
public class AbstractTexture implements Texture {

    /**
     * Texture data provider.
     */
    protected TextureDataProvider textureDataProvider;

    /**
     * True if the data are up to date.
     */
    protected boolean upToDate;

    /**
     * Current magnification filtering method.
     */
    private Filter magnificationFilter = Filter.NEAREST;

    /**
     * Current minifying filtering method.
     */
    private Filter minifyingFilter = Filter.NEAREST;

    private Wrap sWrappingMode = Wrap.CLAMP;
    private Wrap tWrappingMode = Wrap.CLAMP;

    /**
     * Default constructor.
     */
    public AbstractTexture() {
        upToDate = false;
    }

    @Override
    public synchronized boolean isValid() {
        return (textureDataProvider != null) && (textureDataProvider.isValid());
    }

    @Override
    public Wrap getSWrappingMode() {
        return sWrappingMode;
    }

    @Override
    public void setSWrappingMode(Wrap sWrappingMode) {
        this.sWrappingMode = sWrappingMode;
    }

    @Override
    public Wrap getTWrappingMode() {
        return tWrappingMode;
    }

    @Override
    public void setTWrappingMode(Wrap tWrappingMode) {
        this.tWrappingMode = tWrappingMode;
    }

    @Override
    public Filter getMinifyingFilter() {
        return minifyingFilter;
    }

    @Override
    public void setMinifyingFilter(Filter minifyingFilter) {
        this.minifyingFilter = minifyingFilter;
    }

    @Override
    public Filter getMagnificationFilter() {
        return magnificationFilter;
    }

    @Override
    public void setMagnificationFilter(Filter magnificationFilter) {
        this.magnificationFilter = magnificationFilter;
    }

    @Override
    public synchronized TextureDataProvider getDataProvider() {
        return textureDataProvider;
    }

    @Override
    public synchronized void setDataProvider(TextureDataProvider provider) {
        if (textureDataProvider != null) {
            textureDataProvider.removeDataUser(this);
        }

        textureDataProvider = provider;
        upToDate = false;

        if (textureDataProvider != null) {
            textureDataProvider.addDataUser(this);
        }
    }

    @Override
    public void setDrawer(TextureDrawer textureDrawer) {
        setDataProvider(new DrawnTextureDataProvider(textureDrawer));
    }

    @Override
    public void dataUpdated() {
        upToDate = false;
    }

    @Override
    public double getSScaleFactor() {
        return 1;
    }

    @Override
    public double getTScaleFactor() {
        return 1;
    }
}
