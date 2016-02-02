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
use Locale::PO;

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
$sci_modules_dir =~ s/\/localization\/src\/perl//g;

# Module list
my %modules = get_module_list();

# Where is located the msg
my %msg_module;

# statistics
my $nb_msgs                   = 0;
my $nb_modules                = 0;
my $nb_po_files               = 0;
my $nb_duplicated_msgs        = 0;
my $nb_different_translation  = 0;

# Gestion de la langue
my $language = 'fr_FR';
if( $ARGV[0] ne '' )
{
	$language = $ARGV[0];
	$language =~ s/^\s+//;
	$language =~ s/\s+$//;
}



# Récupération de la liste de tous messages
# ==============================================================================

foreach my $module (sort keys %modules)
{
	my $po_file = get_po_file($module,$language);
	
	if($po_file eq '')
	{
		next;
	}
	
	$nb_po_files++;
	
	my %po_objects = load_po_file(get_po_file($module,$language));
	
	foreach my $msgid ( keys %po_objects )
	{
		if( exists($msg_module{ $msgid }) )
		{
			$msg_module{ $msgid } .= ','.$module;
		}
		else
		{
			$msg_module{ $msgid } = $module;
		}
	}
}

# Détection des messages
# ==============================================================================

my $count = 0;

foreach my $msgid (sort keys %msg_module)
{
	$count++;
	
	my %msg_modules      = get_msg_modules($msgid);
	my $msg_modules_size = keys %msg_modules;
	
	if($msg_modules_size > 1)
	{
		$nb_duplicated_msgs++;
		
		my %translations      = get_msg_translations($msgid,$language);
		my $translations_size = keys %translations;
		
		if($translations_size > 1)
		{
			$nb_different_translation++;
			
			print "================================================================================"."\n";
			print $msgid."\n";
			foreach my $module (sort keys %msg_modules)
			{
				print ' - '.$module.' ';
				
				for( my $i=length($module) ; $i < 30 ; $i++ )
				{
					print '.';
				}
				
				print ' '.get_msg_translation($msgid,$module,$language);
				print "\n";
			}
		}
	}
}

# Affichage des statistics
# ==============================================================================
$nb_msgs      = keys %msg_module;
$nb_modules   = keys %modules;

print "\n";
print "\n";
print "================================================================================"."\n";
print 'Nombre de messages               : '.$nb_msgs."\n";
print 'Nombre de modules                : '.$nb_modules."\n";
print 'Nombre de fichier PO             : '.$nb_po_files."\n";
print 'Nombre de messages en double     : '.$nb_duplicated_msgs."\n";
print 'Nombre de messages dont'."\n";
print '  plusieurs traductions existent : '.$nb_different_translation."\n";
print "================================================================================"."\n";
print "\n";
print "\n";





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
# $path = get_po_file($module,$language)
# ==============================================================================

sub get_po_file
{
	my $module   = $_[0];
	my $language = $_[1];
	
	my $po_dir   = get_module_path($module).'/locales/'.$language;
	my $po_file  = $po_dir.'/'.$module.'.po';
	
	unless( -f $po_file )
	{
		return '';
	}
	
	return $po_file;
}

# ==============================================================================
# $path = get_module_path($module,$language)
# ==============================================================================

sub get_module_path
{
	return $sci_modules_dir.'/'.$_[0];
}


# ==============================================================================
# %str = load_po_file($po_file)
# ==============================================================================

sub load_po_file
{
	my $po_file  = $_[0];
	
	my $hash_ref = Locale::PO->load_file_ashash($po_file);
	my %hash     = %$hash_ref;
	
	my %hash_out;
	
	foreach my $key (sort keys %hash)
	{
		my $po_object = $hash{$key};
		
		if(length($key) <= 2)
		{
			next;
		}
		
		# remove the " at the start and the end of string
		my $new_key = substr($key,1,length($key)-2);
		
		unless( $po_object->obsolete == 1 )
		{
			$hash_out{$new_key} = $hash{$key};
		}
	}
	
	return %hash_out;
}


# ==============================================================================
# %modules = get_msg_modules($msgid)
# ==============================================================================

sub get_msg_modules
{
	my $msgid       = $_[0];
	my %modules;
	
	my $modules_str   = $msg_module{ $msgid };
	my @modules_array = split(/,/,$modules_str);
	
	
	foreach my $module (sort @modules_array)
	{
		$modules{$module} = 1;
	}
	
	return %modules;
}


# ==============================================================================
# $msgstr = get_msg_translation($msgid,$module,$language)
# ==============================================================================

sub get_msg_translation
{
	my $msgid      = $_[0];
	my $module     = $_[1];
	my $language   = $_[2];
	
	my $po_file    = get_po_file($module,$language);
	
	my $hash_ref   = Locale::PO->load_file_ashash($po_file);
	my %hash       = %$hash_ref;
	
	my $msgstr     = '';
	
	foreach my $key (sort keys %hash)
	{
		if( $key eq '"'.$msgid.'"' )
		{
			my $po_object = $hash{$key};
			$msgstr       = $po_object->msgstr;
			$msgstr       = substr($msgstr,1,length($msgstr)-2);
			last;
		}
	}
	
	return $msgstr;
}


# ==============================================================================
# %msgstr = get_msg_translations($msgid,$language)
# ==============================================================================

sub get_msg_translations
{
	my $msgid      = $_[0];
	my $language   = $_[1];
	
	my %msgstr;
	
	my %modules    = get_msg_modules($msgid);
	
	
	foreach my $module (sort keys %modules)
	{
		my $translation = get_msg_translation($msgid,$module,$language);
		
		unless( exists($msgstr{$translation}) )
		{
			$msgstr{$translation} = $module;
		}
	}
	
	return %msgstr;
}







