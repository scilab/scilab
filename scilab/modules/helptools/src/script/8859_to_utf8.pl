#!/usr/bin/perl

# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
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
# ==============================================================================

my $directory           = getcwd();

if( dirname($0) ne '.' )
{
	$directory .= '/'.dirname($0);
}

if( dirname($0) =~ m/^\// )
{
	$directory = dirname($0);
}

# modules dir path
# ==============================================================================
my $sci_modules_dir = $directory;
$sci_modules_dir =~ s/\/helptools\/src\/perl//g;


# Module list
# ==============================================================================

my %modules;

if( $ARGV[0] ne '' )
{
	$modules{$ARGV[0]} = 1;
}
else
{
	%modules = get_module_list();
}

my %xmllist;

my $count = 0;

my %languages;
$languages{'en_US'} = 1;
$languages{'fr_FR'} = 1;
$languages{'pt_BR'} = 1;

foreach my $module (sort keys %modules)
{
	foreach my $language (sort keys %languages)
	{
		my $this_directory = $sci_modules_dir.'/'.$module.'/help/'.$language;
		
		if( -e $this_directory )
		{
			get_xml_list($sci_modules_dir.'/'.$module.'/help/'.$language);
		}
	}
}

foreach my $xmlfile (sort keys %xmllist)
{
	$count++;
	printf('%03d - %s ',$count,$xmlfile);
	
	for( my $i=length($xmlfile);$i<140;$i++)
	{
		print '.';
	}
	
	if( is_8859($xmlfile) == 1 )
	{
		convert_8859_to_utf8($xmlfile);
		replace_8859_to_utf8($xmlfile);
		print 'PROCESSED';
	}
	else
	{
		print 'UTF-8';
	}
	
	print "\n";
}

# ==============================================================================
# get_module_list
# ==============================================================================

sub get_module_list
{
	my %list;
	
	unless( chdir($sci_modules_dir) )
	{
		print 'ERROR: The directory '.$sci_modules_dir.' doesn\'t exist or read access denied'."\n";
		del_tmp_file();
		exit(0);
	}
	
	my @candidates = <*>;
	
	foreach my $candidate (@candidates)
	{
		if( -e $sci_modules_dir.'/'.$candidate.'/help' )
		{
			$list{$candidate} = 1;
		}
	}
	
	return %list;
}

# ==============================================================================
# get_xml_list
# ==============================================================================

sub get_xml_list
{
	my $dir = $_[0];
	my @list_dir;
	
	my $current_directory;
	
	# On enregistre le répertoire dans lequel on se situe à l'entrée de la fonction
	my $previous_directory = getcwd();
	
	chdir($dir);
	
	@list_dir = <*>;
	
	foreach my $list_dir (@list_dir)
	{
		$current_directory = getcwd();
		
		if(-d $list_dir)
		{
			get_xml_list($current_directory.'/'.$list_dir);
		}
		
		if( (-f $list_dir) && ($list_dir =~ m/\.xml$/) )
		{
			$xmllist{$current_directory.'/'.$list_dir} = 1;
		}
	}
	
	chdir($previous_directory);
}

# ==============================================================================
# is_8859
# ==============================================================================

sub is_8859
{
	my $xmlfile = $_[0];
	
	unless( open(XMLFILE,$xmlfile) )
	{
		print 'ERROR: Le fichier '.$xmlfile.' n\'a pu être ouvert en lecture'."\n";
		exit(0);
	}
	
	my $is_8859 = 0;
	
	while(<XMLFILE>)
	{
		if( ($_ =~ m/encoding/) && ( $_ =~ m/8859/ ) && (index(lc($_),'version="1.0" encoding="iso-8859-1"')!= -1) )
		{
			$is_8859 = 1;
			last;
		}
		
		last;
	}
	
	close(XMLFILE);
	
	return $is_8859;
}

# ==============================================================================
# convert_8859_to_utf8
# ==============================================================================

sub convert_8859_to_utf8
{
	my $xmlfile_in  = $_[0];
	my $xmlfile_out = $xmlfile_in.'.utf8';
	
	my $cmd         = 'iconv -f ISO_8859-1 -t UTF-8 "'.$xmlfile_in.'" -o "'.$xmlfile_out.'"';
	
	my $code = system($cmd);
	
	if( $code != 0 )
	{
		print 'ERROR: La commande "'.$cmd.'" a échoué'."\n";
		exit(0);
	}

	unless(unlink($xmlfile_in))
	{
		print 'ERROR: Le fichier '.$xmlfile_in.' n\'a pu être supprimé'."\n";
		exit(0);
	}

	unless(rename($xmlfile_out,$xmlfile_in))
	{
		print 'ERROR: Le fichier '.$xmlfile_out.' n\'a pu être renommé en '.$xmlfile_in."\n";
		exit(0);
	}
}

# ==============================================================================
# convert_8859_to_utf8
# ==============================================================================

sub replace_8859_to_utf8
{
	my $xmlfile_in  = $_[0];
	my $xmlfile_out = $xmlfile_in.'.utf8';
	
	unless( open(XMLREAD,$xmlfile_in) )
	{
		print 'ERROR: Le fichier '.$xmlfile_in.' n\'a pu être ouvert en lecture'."\n";
		exit(0);
	}
	
	unless( open(XMLWRITE,'> '.$xmlfile_out) )
	{
		print 'ERROR: Le fichier '.$xmlfile_out.' n\'a pu être ouvert en écriture'."\n";
		exit(0);
	}
	
	while(<XMLREAD>)
	{
		if( ($_ =~ m/encoding/) && ( $_ =~ m/iso-8859-1/i ) && (index(lc($_),'version="1.0" encoding="iso-8859-1"')!= -1) )
		{
			$_ =~ s/iso-8859-1/UTF-8/g;
			$_ =~ s/ISO-8859-1/UTF-8/g;
		}
		
		print XMLWRITE $_;
	}
	
	close(XMLREAD);
	close(XMLWRITE);
	
	unless(unlink($xmlfile_in))
	{
		print 'ERROR: Le fichier '.$xmlfile_in.' n\'a pu être supprimé'."\n";
		exit(0);
	}

	unless(rename($xmlfile_out,$xmlfile_in))
	{
		print 'ERROR: Le fichier '.$xmlfile_out.' n\'a pu être renommé en '.$xmlfile_in."\n";
		exit(0);
	}
}
