;
; Scilab ( http://mwww.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2012 - DIGITEO - Allan CORNET
; Copyright (C) 2015 - Scilab Enterprises - Vincent COUVERT
;
; Copyright (C) 2012 - 2016 - Scilab Enterprises
;
; This file is hereby licensed under the terms of the GNU GPL v2.0,
; pursuant to article 5.3.4 of the CeCILL v.2.1.
; This file was originally licensed under the terms of the CeCILL v2.1,
; and continues to be available under such terms.
; For more information, see the COPYING file which you should have received
; along with this program.
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
