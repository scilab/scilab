#!/usr/bin/perl

# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.

use strict;
use Cwd;
use File::Basename;

# Current directory
my $directory       = getcwd();

if( dirname($0) ne '.' )
{
	$directory .= '/'.dirname($0);
}

if( dirname($0) =~ m/^\// )
{
	$directory = dirname($0);
}

# modules dir path
my $sci_modules_dir = $directory;
$sci_modules_dir =~ s/\/development_tools\/src\/perl//g;

# Module list
my %modules = get_module_list();

foreach my $module (sort keys %modules)
{
	print $module;
	
	my $nb_pb                  = 0;
	
	my %list_makefile_headers  = get_makefileam_list($module,'HEADERS');
	my %list_real_headers      = get_real_list($module,'HEADERS');
	
	my $nb_makefile_headers    = 0;
	my $nb_real_headers        = 0;
	
	my $error_list             = '';
	
	
	foreach my $file (sort keys %list_makefile_headers)
	{
		$nb_makefile_headers++;
		
		if( ! -e $sci_modules_dir.'/'.$module.'/'.$file )
		{
			$nb_pb++;
			$error_list .= sprintf("\t".'% 3d - %s doesn\'t exist'."\n",$nb_pb,$file);
		}
	}
	
	foreach my $file (sort keys %list_real_headers)
	{
		$nb_real_headers++;
		
		if( ! exists( $list_makefile_headers{$file} ) )
		{
			$nb_pb++;
			$error_list .= sprintf("\t".'% 3d - %s is not listed in the Makefile.am file'."\n",$nb_pb,$file);
		}
	}
	
	if( $nb_pb == 0 )
	{
		for( my $i = length($module) ; $i <= 30 ; $i++ )
		{
			print ' ';
		}
		print 'OK';
		printf(' [ % 4d include file(s) ]'."\n",$nb_makefile_headers);
	}
	else
	{
		print "\n".$error_list;
	}
}

# ==============================================================================
# get_makefileam_list
# ==============================================================================

sub get_makefileam_list()
{
	my $module            = $_[0];
	
	my $list_start        = 0;
	my @file_list_string;
	my %list;
	
	my $nb_blocs = 0;
	
	unless( chdir($sci_modules_dir.'/'.$module) )
	{
		print $module.' : The directory '.$sci_modules_dir.'/'.$module.' doesn\'t exist or read access denied'."\n";
		exit;
	}
	
	# Ouverture du fichier Makefile.am pour en extraire le bloc d'include
	# ==========================================================================
	
	unless( open(MAKEFILEAM,'Makefile.am') )
	{
		print $module.' : Makefile.am file doesn\'t exist or read access denied'."\n";
		exit;
	}
	
	while(<MAKEFILEAM>)
	{
		# suppression du \n
		chomp($_);
		
		# Suppression des espaces en début et fin de chaîne
		$_ =~ s/^\s+//;
		$_ =~ s/\s+$//;
		
		# Début de la list
		
		if($_ =~ m/lib(.*)include_HEADERS/ )
		{
			$nb_blocs++;
			$file_list_string[$nb_blocs] = '';
			$list_start = 1;
		}
		
		# Traitement de la liste
		if( $list_start == 1 )
		{
			$file_list_string[$nb_blocs] .= $_;
			
			if( index($_,' \\') == -1 )
			{
				$list_start = 0;
			}
		}
	}
	
	close(MAKEFILEAM);
	
	# parcours des blocs
	# ==========================================================================
	
	for( my $i=1 ; $i <= $nb_blocs ; $i++ )
	{
		# traitement du bloc d'include
		# ======================================================================
		
		# suppression des parties inutiles
		# ================================
		
		$file_list_string[$i] =~ s/\\/ /g;
		
		# Suppression des espaces en début et fin de chaîne
		my $rep = index( $file_list_string[$i] , '=' );
		substr($file_list_string[$i],0,$rep+1) = '';
		
		# Suppression des espaces en début et fin de chaîne
		$file_list_string[$i] =~ s/^\s+//;
		$file_list_string[$i] =~ s/\s+$//;
		
		# Séparation des fichiers
		my @files = split(/[[:blank:]]+/,$file_list_string[$i]);
		
		foreach my $file (@files)
		{
			$list{$file} = 1;
		}
	}
	
	return %list;
}

# ==============================================================================
# get_real_list
# ==============================================================================

sub get_real_list()
{
	my $module = $_[0];
	
	my $list_start        = 0;
	my $file_list_string  = '';
	
	my %list;
	
	unless( chdir($sci_modules_dir.'/'.$module) )
	{
		print $module.' : The directory '.$sci_modules_dir.'/'.$module.' doesn\'t exist or read access denied'."\n";
		exit;
	}
	
	my @files = <includes/*>;
	
	foreach my $file (@files)
	{
		$list{$file} = 1;
	}
	
	return %list;
}

# ==============================================================================
# get_module_list
# ==============================================================================

sub get_module_list
{
	my %list;
	
	unless( chdir($sci_modules_dir) )
	{
		print 'The directory '.$sci_modules_dir.' doesn\'t exist or read access denied'."\n";
		exit;
	}
	
	my @candidates = <*>;
	
	foreach my $candidate (@candidates)
	{
		if( -e $sci_modules_dir.'/'.$candidate.'/Makefile.am' )
		{
			$list{$candidate} = 1;
		}
	}
	
	return %list;
}

# ==============================================================================
# search
# ==============================================================================

sub search
{
	my $dir      = $_[0];
	my $module   = $_[1];
	
	my $dirToPrint;
	my @list_dir;
	
	my $current_directory;
	my $previous_directory = getcwd();
	
	chdir($dir);
	
	@list_dir = <*>;
	
	foreach my $list_dir (@list_dir)
	{
		$current_directory = getcwd();
		
		if( (-d $list_dir) && ($list_dir ne '.svn') )
		{
			search($current_directory.'/'.$list_dir,$module);
		}
		
		if( -f $list_dir )
		{
			my $file = $current_directory.'/'.$list_dir;
			substr($file,0,length($sci_modules_dir.'/'.$module.'/')) = '';
			
			print LIST $file.' ';
		}
	}
	
	chdir($previous_directory);
}
