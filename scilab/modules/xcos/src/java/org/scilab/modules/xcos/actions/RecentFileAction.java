/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.actions;

import java.awt.event.ActionEvent;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;
import java.util.logging.Logger;

import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.configuration.ConfigurationManager;
import org.scilab.modules.xcos.configuration.model.DocumentType;
import org.scilab.modules.xcos.configuration.utils.ConfigurationConstants;

/**
 * Implement the recent file actions.
 *
 * This class doesn't follow standard action implementation.
 */
@SuppressWarnings(value = { "serial" })
public final class RecentFileAction extends DefaultAction implements PropertyChangeListener {
    /** Name of the action */
    public static final String NAME = "";
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = 0;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = 0;

    private static final Map<URI, RecentFileAction> INSTANCE_REGISTRY = new HashMap<URI, RecentFileAction>();

    private File recentFile;
    private MenuItem menu;

    /**
     * @param file
     *            new recent file
     */
    private RecentFileAction(File file) {
        super(null);
        recentFile = file;
    }

    /**
     * @param menu
     *            the menu to set
     */
    private void setMenu(MenuItem menu) {
        this.menu = menu;
    }

    /**
     * @param e
     *            parameter
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        try {
            Xcos.getInstance().open(recentFile.getCanonicalPath(), 0);
        } catch (IOException e1) {
            e1.printStackTrace();
        }
    };

    /**
     * @param file
     *            new recent file
     * @return menu item
     */
    public static MenuItem createMenu(URL file) {
        URI fileURI;
        File f;
        try {
            fileURI = file.toURI();
            f = new File(fileURI);
        } catch (URISyntaxException e) {
            Logger.getLogger(RecentFileAction.class.getName()).severe(e.toString());
            return null;
        }

        RecentFileAction action = INSTANCE_REGISTRY.get(fileURI);
        if (action == null) {
            action = new RecentFileAction(f);
        }

        ConfigurationManager manager = ConfigurationManager.getInstance();
        manager.addPropertyChangeListener(ConfigurationConstants.RECENT_FILES_CHANGED, action);

        MenuItem m = ScilabMenuItem.createMenuItem();
        m.setCallback(action);
        m.setText(f.getName());

        action.setMenu(m);
        return m;
    }

    /**
     * Update the file association when it has been moved on the configuration
     *
     * @param evt
     *            the event data
     * @see java.beans.PropertyChangeListener#propertyChange(java.beans.PropertyChangeEvent)
     */
    @Override
    public void propertyChange(PropertyChangeEvent evt) {
        assert evt.getPropertyName().equals(ConfigurationConstants.RECENT_FILES_CHANGED);

        /*
         * This is the case where a new menu will be created. That's not our job
         * there.
         */
        if (evt.getOldValue() == null) {
            return;
        }

        /*
         * Create URL instance;
         */
        final String oldURL = ((DocumentType) evt.getOldValue()).getUrl();
        final String newURL = ((DocumentType) evt.getNewValue()).getUrl();

        URL old;
        URL current;
        try {
            old = new URL(oldURL);
            current = recentFile.toURI().toURL();
        } catch (MalformedURLException e) {
            Logger.getLogger(RecentFileAction.class.getName()).severe(e.toString());
            return;
        }

        /*
         * Return when it is not our associated file.
         */
        if (!current.sameFile(old)) {
            return;
        }

        /*
         * Our job then
         */
        try {
            URL newUrl = new URL(newURL);
            final URI newURI = newUrl.toURI();

            recentFile = new File(newURI);
            menu.setText(recentFile.getName());

            INSTANCE_REGISTRY.remove(old.toURI());
            INSTANCE_REGISTRY.put(newURI, this);

        } catch (URISyntaxException e) {
            Logger.getLogger(RecentFileAction.class.getName()).severe(e.toString());
        } catch (MalformedURLException e) {
            Logger.getLogger(RecentFileAction.class.getName()).severe(e.toString());
        }
    }
}
