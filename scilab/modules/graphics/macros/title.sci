function title(varargin)
// Copyright INRIA
// TITLE function
// Add titles on a graphics window 
// F.Belahcene

varargin = list('title',varargin(1:$));
TitleLabel(varargin(:));
endfunction