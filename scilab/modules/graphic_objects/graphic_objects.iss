;
; Scilab ( http://mwww.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2012 - DIGITEO - Allan CORNET
; Copyright (C) 2015 - Scilab Enterprises - Vincent COUVERT
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
;
;--------------------------------------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;--------------------------------------------------------------------------------------------------------------
; graphic_objects module
;--------------------------------------------------------------------------------------------------------------
;
#define GRAPHIC_OBJECTS "graphic_objects"
;
Source: modules\{#GRAPHIC_OBJECTS}\license.txt; DestDir: {app}\modules\{#GRAPHIC_OBJECTS}; Components: {#COMPN_GRAPHICS}
;
Source: bin\sci{#GRAPHIC_OBJECTS}.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
;
Source: bin\nosci{#GRAPHIC_OBJECTS}.dll; DestDir: {app}\bin; DestName: sci{#GRAPHIC_OBJECTS}.dll; Components: not ({#COMPN_GRAPHICS})
;
Source: modules\{#GRAPHIC_OBJECTS}\etc\{#GRAPHIC_OBJECTS}.quit; DestDir: {app}\modules\{#GRAPHIC_OBJECTS}\etc; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHIC_OBJECTS}\etc\{#GRAPHIC_OBJECTS}.start; DestDir: {app}\modules\{#GRAPHIC_OBJECTS}\etc; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHIC_OBJECTS}\tests\*.*; DestDir: {app}\modules\{#GRAPHIC_OBJECTS}\tests; Flags: recursesubdirs; Components: {#COMPN_GRAPHICS} and {#COMPN_TESTS}
;
Source: modules\{#GRAPHIC_OBJECTS}\jar\org.scilab.modules.{#GRAPHIC_OBJECTS}.jar;DestDir: {app}\modules\{#GRAPHIC_OBJECTS}\jar; Components: {#COMPN_GRAPHICS}
;
;--------------------------------------------------------------------------------------------------------------
