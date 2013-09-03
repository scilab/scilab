;##############################################################################################################
; Scilab ( http://mwww.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2012 - DIGITEO - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; graphic_objects module
;--------------------------------------------------------------------------------------------------------------
;
#define GRAPHIC_OBJECTS "graphic_objects"
;
Source: bin\sci{#GRAPHIC_OBJECTS}.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
Source: bin\nosci{#GRAPHIC_OBJECTS}.dll; DestDir: {app}\bin; DestName: sci{#GRAPHIC_OBJECTS}.dll; Components: not ({#COMPN_GRAPHICS})
;
Source: modules\{#GRAPHIC_OBJECTS}\jar\org.scilab.modules.{#GRAPHIC_OBJECTS}.jar;DestDir: {app}\modules\{#GRAPHIC_OBJECTS}\jar; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHIC_OBJECTS}\etc\{#GRAPHIC_OBJECTS}.quit; DestDir: {app}\modules\{#GRAPHIC_OBJECTS}\etc; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHIC_OBJECTS}\etc\{#GRAPHIC_OBJECTS}.start; DestDir: {app}\modules\{#GRAPHIC_OBJECTS}\etc; Components: {#COMPN_GRAPHICS}
;
;--------------------------------------------------------------------------------------------------------------
