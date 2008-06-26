#!/usr/bin/perl -w

# buildtoolbox.pl
# Usage: buildtoolbox.pl toolbox-archive [config file [stage]]

use strict;
use Cwd;

my ($TOOLBOXFILE, # Toolbox archive to compile
    $TOOLBOXNAME); # Name of the toolbox

# is_zip:
#    Return true if toolbox file extension is zip
sub is_zip {
	return $TOOLBOXFILE =~ /\.zip$/;
}

# get_tree_from_tgz:
#   Get all files (names) of the compressed (in tar.gz) sources
sub get_tree_from_tgz {
	my %files;
	
	open my $fd, "tar -tzf ${TOOLBOXFILE}|";
	
	while(<$fd>) {
		chomp;
		$files{$_} = 1;
	}
	
	close $fd;
	return %files;
}

# get_tree_from_zip:
#   Get all files (names) of the compressed (in zip) sources
sub get_tree_from_zip {
	my (%files, $line);
	
	# tail & head are here to skip header & footer
	open my $fd, "unzip -l ${TOOLBOXFILE} | tail -n +4 | head -n -2 |";
	
	while(<$fd>) {
		# zip output format: size date time filename
		/\s*\d+\s+\d+-\d+-\d+\s+\d+:\d+\s+(.+)/ or die "Bad output of unzip";
		chomp $1;
		$files{$1} = 1;
	}
	
	close $fd;
	return %files;
}

# get_tree:
#   Get all files (names) of the compressed sources, in a hash
#   (hash values are meaningless, set to 1)
sub get_tree {
	if(is_zip()) {
		return get_tree_from_zip();
	}
	else {
		return get_tree_from_tgz();
	}
}

# get_description_from_tgz:
#    Extract DESCRIPTION file from the archive (in tar.gz format)
sub get_description_from_tgz {
	open my $fd, "tar -xOzf ${TOOLBOXFILE} ${TOOLBOXNAME}/DESCRIPTION |";
	return $fd;
}

# get_description_from_tgz:
#    Extract DESCRIPTION file from the archive (in zip format)
sub get_description_from_zip {
	open my $fd, "unzip -c ${TOOLBOXFILE} ${TOOLBOXNAME}/DESCRIPTION | tail -n +3 | head -n -1 |";
	return $fd;
}

# get_description:
#   Extract DESCRIPTION file from the archive
sub get_description {
	if(is_zip) {
		return get_description_from_zip();
	}
	else {
		return get_description_from_tgz();
	}
}

# read_description:
#   Check if DESCRIPTION file is correct, and parse it (return a hash
#   field => value).
#   First argument is a file descriptor for the DESCRIPTION file (see
#   get_description)
sub read_description {
	my ($fd) = shift;
	my @required = qw(Toolbox Version Title Author Maintainer
	                  Description License Category);
	my @optional = qw(Date Depends URL Entity);
	my (%infos, $key, $val);
	my (%lines, %correct);
	
	# Populate hash
	while(<$fd>) {
		die "\":\" not followed by a space at line $." if(/:(?! )/);
		if(/:/) { # New field
			($key, $val) = split(/: /, $_, 2);
			$infos{$key} = $val;
			$lines{$key} = $.;
			$correct{$key} = 0;
		}
		else { # Continuation of previous field
			$infos{$key} .= $_;
		}
	}
	
	# Check presence of required fields, mark them as correct
	foreach (@required) {
		if(!defined($infos{$_})) {
			die "Mandatory field \"$_\" not defined";
		}
		else {
			$correct{$_} = 1;
		}
	}
	
	# Mark optional fields as correct
	foreach (@optional) {
		if(defined($infos{$_})) {
			$correct{$_} = 1;
		}
	}
	
	# Check that there's no incorrect (= unknown) fields
	foreach (keys(%infos)) {
		if($correct{$_} == 0) {
			die "Unknown field \"$_\" (defined at line $lines{$_})";
		}
	}
	
	chomp %infos;
	return %infos;
}

# check_tree:
#   Given a source tree of a toolbox (see get_tree), check if it is correct
#   (required files are present, files are at their right place, and so on...)
sub check_tree {
	my %tree = @_;
	my %newtree;
	
	# Check that all files are under a root which has the same name as the toolbox
	# Delete this root to simplify other tests
	foreach (keys %tree) {
		if(s#^\Q$TOOLBOXNAME\E(/|$)##) {
			$newtree{$_} = 1 if $_;
		}
		else {
			die "Incorrect archive: \"$_\" is not a child of \"$TOOLBOXNAME\"";
		}
	}
	%tree = %newtree;
	
	# Check that basic files are here
	my @required = qw(DESCRIPTION DESCRIPTION-FUNCTIONS readme.txt license.txt
	                  builder.sce loader.sce);
	push(@required, "etc/$TOOLBOXNAME.start");
	push(@required, "etc/$TOOLBOXNAME.end");
	
	foreach (@required) {
		if(!defined($tree{$_})) {
			die "Incorrect archive: required file \"$_\" not present";
		}
	}
	
	# macros/ must contain only .sci and .sce files
	# If it exists and is non-empty, it must contains buildmacros.sce
	my $macros_empty = 1;
	my $macros_has_builder = 0;
	foreach (grep { $_ =~ m#^macros/# } keys %tree) {
		if(/\.sc[ie]$/) {
			$macros_empty = 0;
			$macros_has_builder = 1 if(m#/buildmacros\.sce$#);
		}
		elsif(!/\/$/) { # Don't be /too/ nazi: allow sub-directories :)
			die "Incorrect archive: macros/ must contain only .sci and .sce files".
			    " (\"$_\" found)";
		}
	}
	
	if(!$macros_empty && !$macros_has_builder) {
		die "Incorrect archive: macros/ not empty and no buildmacros.sce script found";
	}
	
	# All fortran files must be in src/fortran
	foreach (grep { $_ =~ /\.f$/} keys %tree) {
		if(!m#^src/fortran/#) {
			die "Incorrect archive: \"$_\" is a fortran source and hence has to be in ".
			    "src/fortran";
		}
	}

	# All c files must be in src/c or sci_gateway/{c,fortran}
	foreach (grep { $_ =~ /\.[ch]$/} keys %tree) {
		if(!m#^(src/c|sci_gateway/(c|fortran))/#) {
			die "Incorrect archive: \"$_\" is a C source and hence has to be in ".
			    "src/c, sci_gateway/c or sci_gateway/fortran";
		}
	}
	
	# if src/c contains at least a .c file, src/c/buildsrc_c.sce must exists
	my $has_c_source = grep { $_ =~ m#^src/c/.+\.[ch]$# } keys %tree;
	my $has_c_src_builder = defined($tree{"src/c/buildsrc_c.sce"});
	if($has_c_source && !$has_c_src_builder) {
		die "Incorrect archives: C source found in src/c/ but no buildsrc_c.sce ".
		    "script found";
	}
	
	# if src/fortran contains at least a .f file, src/fortran/buildsrc_fortran.sce must exists
	my $has_f_source = grep { $_ =~ m#^src/fortran/.+\.f$# } keys %tree;
	my $has_f_src_builder = defined($tree{"src/fortran/buildsrc_fortran.sce"});
	if($has_f_source && !$has_f_src_builder) {
		die "Incorrect archives: Fortran source found in src/fortran/ ".
		    "but no buildsrc_fortran.sce script found";
	}
	
	# if src/*/buildsrc_*.sce exists, src/buildsrc.sce must exists 
	my $has_src_builder = defined($tree{"src/buildsrc.sce"});
	if(($has_f_source || $has_c_source) && !$has_src_builder) {
		die "Incorrect archive: sources file found but no buildsrc.sce script found";
	}
	
	# if sci_gateway/fortran contains at least a .c file,
	# sci_gateway/fortran/buildgateway_fortran.sce must exists.
	my $has_f_gateway = grep { m#^sci_gateway/fortran/.+\.[ch]$# } keys %tree;
	my $has_f_gateway_builder = defined($tree{"sci_gateway/fortran/buildgateway_fortran.sce"});
	if($has_f_gateway && !$has_f_gateway_builder) {
		die "Incorrect archive: Fortran gateway found but can't find any builder for it";
	}
	
	# if sci_gateway/c contains at least a .c file, sci_gateway/c/buildgateway_c.sce must exists
	my $has_c_gateway = grep { m#^sci_gateway/c/.+\.[ch]$# } keys %tree;
	my $has_c_gateway_builder = defined($tree{"sci_gateway/c/buildgateway_c.sce"});
	if($has_c_gateway && !$has_c_gateway_builder) {
		die "Incorrect archive: C gateway found but can't find any builder for it";
	}
	
	# if sci_gateway/*/buildgateway_*.sce exists, sci_gateway/buildgateway.sce must exists
	my $has_gateway_builder = defined($tree{"sci_gateway/buildgateway.sce"});
	if(($has_c_gateway || $has_f_gateway) && !$has_gateway_builder) {
		die "Incorrect archive: gateway found no gateway builder (buildgateway.sce) found";
	}
	
	# if help/ contains .xml files, it must contains a buildhelp.sce file
	my $has_help = grep { m#^help/.+\.xml$# } keys %tree;
	my $has_help_builder = defined($tree{"help/buildhelp.sce"});
	if($has_help && !$has_help_builder) {
		die "Incorrect archive: help files found but no help builder (buildhelp.sce) found";
	}
}

# Init global vars, check arguments
$TOOLBOXFILE = shift;
if(!defined($TOOLBOXFILE)) {
	die "Toolbox source file required";
}

if(! -r $TOOLBOXFILE) {
	die "$TOOLBOXFILE doesn't exists or can't be read";
}

$TOOLBOXNAME = $1 if ($TOOLBOXFILE =~ /^([^.]+)/);


check_tree(get_tree());

