/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
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

package org.scilab.modules.gui.tabfactory;

import java.awt.Component;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

import javax.swing.JTextArea;

import org.flexdock.docking.DockableFactory;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.jvm.LoadClassPath;
import org.scilab.modules.localization.Messages;

/**
 * The main Tab factory.
 * A component which needs to restore a Tab with a given uuid must register its factory.
 *
 * @author Calixte DENIZET
 */
public class ScilabTabFactory extends DockableFactory.Stub {

    private static final ScilabTabFactory instance = new ScilabTabFactory();
    private static final String EMPTYTAB = Messages.gettext("Empty tab");
    private static final String NULLUUID = new UUID(0L, 0L).toString();
    private static final String ERROR = Messages.gettext("The tab with uuid %s cannot be restored.\nPlease report a bug with the previous message and in attaching the file %s.");

    /*
      Certains components could depend an other Tab and this last one could be created before
      to be requested by the Tab restorator. So we cache it to be sure to have the same instance.
    */
    private Map<String, SwingScilabDockablePanel> cache = new HashMap<String, SwingScilabDockablePanel>();
    private Map<String, AbstractScilabTabFactory> factories = new HashMap<String, AbstractScilabTabFactory>();

    /**
     * Default constructor
     */
    protected ScilabTabFactory() { }

    /**
     * {@inheritDoc}
     */
    public Component getDockableComponent(String uuid) {
        return getTab(uuid);
    }

    /**
     * Adds a new Tab factory
     * @param factory the factory to add
     */
    public void addTabFactory(AbstractScilabTabFactory factory) {
        if (!factories.containsKey(factory.getClassName())) {
            factories.put(factory.getClassName(), factory);
        }
    }

    /**
     * Adds a new Tab factory
     * @param pack the package to load in using LoadClassPath
     * @param className the name of the factory
     * @return the newly added factory
     */
    public AbstractScilabTabFactory addTabFactory(String pack, String className) {
        AbstractScilabTabFactory factory = null;
        if (!factories.containsKey(className)) {
            if (pack != null && !pack.isEmpty()) {
                LoadClassPath.loadOnUse(pack);
            }
            try {
                Class clazz = Class.forName(className);
                factory = (AbstractScilabTabFactory) clazz.newInstance();
                addTabFactory(factory);
            } catch (ClassNotFoundException e) {
                System.out.println(e);
            } catch (IllegalAccessException e) {
                System.out.println(e);
            } catch (InstantiationException e) {
                System.out.println(e);
            }
        }

        return factory;
    }

    /**
     * Removes a Tab factory
     * @param factory the factory to remove
     */
    public void removeTabFactory(AbstractScilabTabFactory factory) {
        factories.remove(factory);
    }

    /**
     * Creates a tab given an uuid
     * @param uuid the uuid
     * @return the corresponding tab
     */
    public SwingScilabDockablePanel getTab(String uuid) {
        SwingScilabDockablePanel tab = cache.get(uuid);
        if (tab != null) {
            return tab;
        }

        for (String name : factories.keySet()) {
            AbstractScilabTabFactory factory = factories.get(name);
            if (factory.isAValidUUID(uuid)) {
                tab = factory.getTab(uuid);
                if (tab != null) {
                    cache.put(uuid, tab);
                    return tab;
                }
            }
        }

        WindowsConfigurationManager.removeNode(uuid);
        return makeEmptyTab(uuid);
    }

    /**
     * {@inheritDoc}
     */
    public String getPackage(String uuid) {
        for (String name : factories.keySet()) {
            if (factories.get(name).isAValidUUID(uuid)) {
                return factories.get(name).getPackage();
            }
        }
        return "";
    }

    /**
     * {@inheritDoc}
     */
    public String getClassName(String uuid) {
        for (String name : factories.keySet()) {
            if (factories.get(name).isAValidUUID(uuid)) {
                return factories.get(name).getClassName();
            }
        }
        return "";
    }

    /**
     * {@inheritDoc}
     */
    public String getApplication(String uuid) {
        for (String name : factories.keySet()) {
            if (factories.get(name).isAValidUUID(uuid)) {
                return factories.get(name).getApplication();
            }
        }
        return "";
    }

    /**
     * Clear the cache
     */
    public void clearCache() {
        cache.clear();
    }

    public void removeFromCache(String uuid) {
        cache.remove(uuid);
    }

    public SwingScilabDockablePanel getFromCache(String uuid) {
        return cache.get(uuid);
    }

    public void addToCache(SwingScilabDockablePanel tab) {
        if (tab != null) {
            cache.put(tab.getPersistentId(), tab);
        }
    }

    /**
     * @return the instace (this class should be used as a singleton)
     */
    public static ScilabTabFactory getInstance() {
        return instance;
    }

    /**
     * Make an empty tab
     * @return an empty tab
     */
    private static final SwingScilabDockablePanel makeEmptyTab(String uuid) {
        String u = uuid;
        if (uuid == null || uuid.isEmpty()) {
            u = NULLUUID;
        }

        SwingScilabDockablePanel tab = new SwingScilabDockablePanel(EMPTYTAB, u);
        String text = String.format(ERROR, u, System.getProperty("user.home"));
        JTextArea textarea = new JTextArea(text);
        textarea.setEditable(false);
        tab.setContentPane(textarea);

        return tab;
    }
}
