// =============================================================================
// Copyright (c) INRIA - Pierre MARECHAL - 2008
// =============================================================================

if ~isdef("genlib") then
	exec(SCI+'/modules/functions/scripts/buildmacros/loadgenlib.sce');
end

genlib("metaneteditorlib",'SCI/modules/metanet/macros/editor',%f,%t);
