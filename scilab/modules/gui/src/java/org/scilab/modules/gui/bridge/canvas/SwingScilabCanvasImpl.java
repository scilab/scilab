/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
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

package org.scilab.modules.gui.bridge.canvas;

import java.awt.Component;
import java.awt.Frame;
import java.awt.HeadlessException;
import java.util.Calendar;
import java.util.StringTokenizer;

import javax.media.opengl.GL;
import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLException;
import javax.media.opengl.GLProfile;
import javax.media.opengl.awt.GLCanvas;
import javax.media.opengl.awt.GLJPanel;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.commons.OS;
import org.scilab.modules.gui.utils.Debug;
import org.scilab.modules.localization.Messages;

public class SwingScilabCanvasImpl {

    static boolean forceGLCanvas = false;
    static boolean noGLJPanel = false;
    static boolean testCanvasAtStartup = false;

    static {
        if (testCanvasAtStartup && OS.get() != OS.MAC) {
            long lastTime = Calendar.getInstance().getTimeInMillis();

            Debug.DEBUG("SwingScilabCanvasImpl", "=======================================");
            String OS_NAME = System.getProperty("os.name");
            Debug.DEBUG("SwingScilabCanvasImpl", "os.name=" + OS_NAME);
            String OS_ARCH = System.getProperty("os.arch");
            Debug.DEBUG("SwingScilabCanvasImpl", "os.arch=" + OS_ARCH);
            Debug.DEBUG("SwingScilabCanvasImpl", "=======================================");

            /*
             * Bug #7526: moved out of the try/catch block since it contains
             * no OpenGL calls.
             */

            if ( OS_NAME.contains("Windows") && OS_ARCH.equals("amd64") ) {
                // bug 3919 : JOGL x64 doesn't like x64 remote desktop on Windows
                // @TODO : bug report to JOGL
                String REMOTEDESKTOP = System.getenv("SCILAB_MSTS_SESSION");
                if (REMOTEDESKTOP != null) {
                    if ( REMOTEDESKTOP.equals("OK") ) {
                        noGLJPanel = true;
                    }
                }

                if (noGLJPanel) {
                    /** Inform the users */
                    InterpreterManagement.requestScilabExec(
                        String.format("disp(\"%s\"), disp(\"%s\")",
                                      String.format(Messages.gettext("WARNING: Due to your configuration limitations, Scilab switched in a mode where mixing uicontrols and graphics is not available. Type %s for more information."), "\"\"help usecanvas\"\""),
                                      String.format(Messages.gettext("In some cases, %s fixes the issue"), "\"\"system_setproperty(''jogl.gljpanel.nohw'','''');\"\"")
                                     )
                    );
                }
            }

            /*
             * Bug #7526: this code block causes a crash on Windows and Intel Graphic HD cards
             * with graphics drivers older than 8.15.10.2279 and is therefore not executed if
             * Windows is the OS (the tests on driver and OpenGL versions are Linux-specific).
             * Consequently, updateMaxCanvasSize, which determines the canvas size, is not called
             * any more on Windows. However, it is still called when an actual graphics window is created
             * (by the renderer module's SciRenderer init method), which still allows getting the maximum
             * canvas dimensions, so this should not be an issue.
             */
            if (!OS_NAME.contains("Windows")) {

                try {
                    GLCanvas tmpCanvas = new GLCanvas(new GLCapabilities(GLProfile.getDefault()));
                    Frame tmpFrame = new Frame();
                    tmpFrame.add(tmpCanvas);
                    tmpFrame.setVisible(true);

                    tmpCanvas.getContext().makeCurrent();
                    GL gl = tmpCanvas.getGL();

                    String GL_VENDOR = gl.glGetString(GL.GL_VENDOR);
                    Debug.DEBUG("SwingScilabCanvasImpl", "GL_VENDOR=" + GL_VENDOR);
                    String GL_RENDERER = gl.glGetString(GL.GL_RENDERER);
                    Debug.DEBUG("SwingScilabCanvasImpl", "GL_RENDERER=" + GL_RENDERER);
                    String GL_VERSION = gl.glGetString(GL.GL_VERSION);
                    Debug.DEBUG("SwingScilabCanvasImpl", "GL_VERSION=" + GL_VERSION);
                    //Debug.DEBUG("SwingScilabCanvasImpl", "GL_EXTENSIONS="+gl.glGetString(GL.GL_EXTENSIONS));
                    Debug.DEBUG("SwingScilabCanvasImpl", "=======================================");
                    //System.getProperties().list(System.err);

                    // get maximum axes size
                    //RenderingCapabilities.updateMaxCanvasSize(gl);

                    tmpCanvas.getContext().release();
                    tmpFrame.remove(tmpCanvas);
                    tmpFrame.setVisible(false);
                    tmpFrame.dispose();
                    Debug.DEBUG("SwingScilabCanvasImpl", "Testing time = " + (Calendar.getInstance().getTimeInMillis() - lastTime) + "ms");

                    noGLJPanel = false;

                    // By default disable GLJPanel on Linux
                    if (OS_NAME.contains("Linux")) {
                        noGLJPanel = true;
                        // Linux && NVIDIA
                        if (GL_VENDOR.contains("NVIDIA")) {
                            noGLJPanel = false;
                        }
                        // Linux && ATI
                        if (GL_VENDOR.contains("ATI")) {
                            StringTokenizer stSpace = new StringTokenizer(GL_VERSION, " ");
                            StringTokenizer stDot = new StringTokenizer(stSpace.nextToken(), ".");
                            int majorVersion = Integer.parseInt(stDot.nextToken());
                            int minorVersion = Integer.parseInt(stDot.nextToken());
                            int releaseVersion = Integer.parseInt(stDot.nextToken());
                            // Only OpenGL version newer than 2.1.7873 works
                            // available through ATI 8.8 installer
                            // and driver newer than 8.52.3
                            Debug.DEBUG("SwingScilabCanvasImpl", "majorVersion = "
                                        + majorVersion + " minorVersion = " + minorVersion
                                        + " releaseVersion = " + releaseVersion);
                            if (majorVersion > 2
                                    || majorVersion == 2 && minorVersion > 1
                                    || majorVersion == 2 && minorVersion == 1 && releaseVersion >= 7873) {
                                noGLJPanel = false;
                            }
                        }
                    }

                    if (noGLJPanel) {
                        /** Inform the users */
                        InterpreterManagement.requestScilabExec(
                            String.format("disp(\"%s\"), disp(\"%s\")",
                                          String.format(Messages.gettext("WARNING: Due to your configuration limitations, Scilab switched in a mode where mixing uicontrols and graphics is not available. Type %s for more information."), "\"\"help usecanvas\"\""),
                                          String.format(Messages.gettext("In some cases, %s fixes the issue"), "\"\"system_setproperty(''jogl.gljpanel.nohw'','''');\"\"")
                                         )
                        );
                    }
                } catch (GLException e) {
                    noGLJPanel = true;
                    /** Inform the users */
                    InterpreterManagement.requestScilabExec(
                        String.format("disp(\"%s\"), disp(\"%s\")",
                                      Messages.gettext("Due to your video card drivers limitations, that are not able to manage OpenGL, Scilab will not be able to draw any graphics. Please update your driver."),
                                      String.format(Messages.gettext("In some cases, %s fixes the issue"), "\"\"system_setproperty(''jogl.gljpanel.nohw'','''');\"\"")
                                     )
                    );
                } catch (HeadlessException e) {
                    // do not print anything on a CLI only environment
                    noGLJPanel = true;
                }

            }
        }
    }

    static boolean enableGLCanvas = forceGLCanvas || noGLJPanel;

    /**
     * Change Global property forceGLCanvas
     * if no GLJPanel is available, GLCanvas is forced
     */
    public static boolean switchToGLCanvas(boolean onOrOff) {
        Debug.DEBUG("SwingScilabCanvasImpl", "switchToGLCanvas " + onOrOff);
        if (!onOrOff && noGLJPanel) {
            InterpreterManagement.requestScilabExec(Messages.gettext("disp(\"WARNING: Despite of our previous warning, you choose to use Scilab with advanced graphics capabilities. Type \"\"help usecanvas\"\" for more information.\")"));
        }
        enableGLCanvas = onOrOff;
        return enableGLCanvas;
    }

    /**
     * Get Global property forceGLCanvas
     * if no GLJPanel is available, GLCanvas is forced
     */
    public static boolean isGLCanvasEnabled() {
        return enableGLCanvas;
    }

    /*
     * Using SafeGLJPanel for all platform to catch some EDT deletion/creation.
     * Some buffer can be lost causing JoGL to crash
     * http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=6503420
     */
    private final class SafeGLJPanel extends GLJPanel {
        private static final long serialVersionUID = -6166986369022555750L;

        public void display() {
            try {
                super.display();
            } catch (Exception e) {
                // Catch JoGL Exceptions and hide it ...
                // Make another try
                //System.err.println("[SafeGLJPanel.display] catching "+e.toString());
                super.reshape(getX(), getY(), getWidth(), getHeight());
                super.display();
            }
        }

        // protected void paintComponent(final Graphics g) {
        //     try {
        //         super.paintComponent(g);
        //     } catch (Exception e) {
        //         // Catch JoGL Exceptions and hide it ...
        //         // Make another try
        //         System.err.println("[SafeGLJPanel.paintComponent] catching "+e.toString());
        //     }
        // }
    }

    /*
    * Empty class to allow xlick/xgetmouse
    * catch same cannonical name
    */
    private final class SafeGLCanvas extends GLCanvas {
        private static final long serialVersionUID = -3315164314205693678L;
    }

    private static SwingScilabCanvasImpl me = null;

    public static SwingScilabCanvasImpl getInstance() {
        if (me == null) {
            me = new SwingScilabCanvasImpl();
        }

        return me;
    }

    public Component createOpenGLComponent() {
        if (enableGLCanvas) {
            return new SafeGLCanvas();
        } else {
            return new SafeGLJPanel();
        }
    }

    public SwingScilabCanvasImpl() {}
}
