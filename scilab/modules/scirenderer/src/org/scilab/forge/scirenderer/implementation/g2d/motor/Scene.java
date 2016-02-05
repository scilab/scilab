/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte Denizet
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.forge.scirenderer.implementation.g2d.motor;

import java.awt.Graphics2D;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.Stack;
import java.util.TreeSet;

import org.scilab.forge.scirenderer.clipping.ClippingPlane;

final class Scene {

    private static Set<Scene> faces2d = new TreeSet<Scene>(new Comparator<Scene>() {
        public int compare(Scene o1, Scene o2) {
            if (o1.object.vertices[0].getZ() == o2.object.vertices[0].getZ()) {
                return o1.object.getPrecedence() - o2.object.getPrecedence();
            }

            return (int) Math.signum(o2.object.vertices[0].getZ() - o1.object.vertices[0].getZ());
        }

        public boolean equals(Object obj) {
            return this == obj;
        }
    });

    private static List<Scene> faces = new ArrayList<Scene>();
    private static List<Scene> disabledFaces = new ArrayList<Scene>();
    private static List<ClippingPlane> clippingPlanes;
    private static Stack<Scene> stack = new Stack<Scene>();;

    private List<Scene> behind;
    private List<Scene> onfront;
    private ConvexObject object;
    private boolean drawn;
    private boolean is2d;

    private Scene(final ConvexObject object) {
        this.object = object;
    }

    public static final void setClippingPlanes(final List<ClippingPlane> clippingPlanes) {
        Scene.clippingPlanes = clippingPlanes;
    }

    private static final List<ConvexObject> breakOnClippingPlane(ConvexObject o) {
        List<ConvexObject> list = new ArrayList<ConvexObject>(8);
        List<ConvexObject> tmp = new ArrayList<ConvexObject>(8);
        list.add(o);
        if (clippingPlanes != null) {
            for (ClippingPlane clip : clippingPlanes) {
                if (clip.isEnable()) {
                    tmp.clear();
                    for (ConvexObject co : list) {
                        List<ConvexObject> l = co.breakObject(clip.getEquation());
                        if (l != null) {
                            tmp.addAll(l);
                        } else {
                            list.clear();
                            return list;
                        }
                    }
                    list.clear();
                    list.addAll(tmp);
                    if (list.isEmpty()) {
                        break;
                    }
                }
            }
        }
        return list;
    }

    static final void addToRoot(final boolean is2D, final ConvexObject co) {
        try {
            List<ConvexObject> broken = breakOnClippingPlane(co);
            for (ConvexObject object : broken) {
                add(is2D, object);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static final void add(final boolean is2D, final ConvexObject object) {
        synchronized (faces) {
            Scene st = new Scene(object);
            if (is2D) {
                st.is2d = true;
                faces2d.add(st);
            } else {
                Set<Scene> toRemove = new HashSet<Scene>();
                List<ConvexObject> toAdd = null;

                for (Scene face : faces) {
                    if (face.object instanceof Triangle && object instanceof Segment) {
                        if (((Triangle) face.object).addSegment((Segment) object)) {
                            if (st != null) {
                                toRemove.add(st);
                                st = null;
                            }
                        }
                    } else if (object instanceof Triangle && face.object instanceof Segment) {
                        if (((Triangle) object).addSegment((Segment) face.object)) {
                            toRemove.add(face);
                        }
                    }

                    if (st != null) {
                        int r = face.object.isBehind(st.object);

                        if (r == 1) {
                            // object is behind face.object
                            addBehind(face, st);
                            addOnFront(st, face);
                        } else if (r == -1) {
                            // face.object is behind object
                            addBehind(st, face);
                            addOnFront(face, st);
                        } else if (r == 2) {
                            // collision
                            toAdd = face.object.breakObject(st.object);
                            if (toAdd != null && !toAdd.isEmpty()) {
                                toRemove.add(face);
                                toRemove.add(st);
                                st = null;
                                break;
                            }
                        }
                    }
                }

                if (st != null) {
                    faces.add(st);
                }

                for (Scene s : toRemove) {
                    faces.remove(s);
                    if (s.onfront != null) {
                        for (Scene ss : s.onfront) {
                            ss.behind.remove(s);
                        }
                    }
                }
                if (toAdd != null) {
                    for (ConvexObject co : toAdd) {
                        add(false, co);
                    }
                }
            }
        }
    }


    static final void clearDepth() {
        disabledFaces.addAll(faces);
        disabledFaces.addAll(faces2d);
        faces.clear();
        faces2d.clear();
    }

    static final void drawRoot(final Graphics2D g2d) {
        synchronized (faces) {
            for (Scene face : disabledFaces) {
                face.draw(g2d);
            }

            for (Scene face : disabledFaces) {
                face.drawn = false;
            }

            for (Scene face : faces) {
                face.draw(g2d);
            }

            for (Scene face : faces2d) {
                face.draw(g2d);
            }

            // code to help debug
            /*for (Scene face : faces) {
              face.drawn = false;
              }

              for (Scene face : faces) {
              if (face.object.marked) {
              face.object.draw(g2d);
              }
              }*/
        }
    }

    static final void clear() {
        disabledFaces.clear();
        faces.clear();
        faces2d.clear();
        stack.clear();
    }

    static final void clearAll() {
        clear();
        clippingPlanes.clear();
    }

    private final void draw(final Graphics2D g2d) {
        if (stack.contains(this)) {
            this.object.addArea(stack.peek().object);
            return;
        }
        if (!drawn) {
            stack.push(this);
            drawn = true;
            if (behind != null && !behind.isEmpty()) {
                for (Scene face : behind) {
                    face.draw(g2d);
                }
            }
            object.draw(g2d);
            stack.pop();
        }
    }

    private static final void addBehind(Scene face, Scene s) {
        if (face.behind == null) {
            face.behind = new ArrayList<Scene>();
        }
        face.behind.add(s);
    }

    private static final void addOnFront(Scene face, Scene s) {
        if (face.onfront == null) {
            face.onfront = new ArrayList<Scene>();
        }
        face.onfront.add(s);
    }
}
