#! /bin/sh
##  -*- sscript -*-
##
## run-test.sh
## Made by  Bruno JOFRET <bruno.jofret@inria.fr>
##
## Started on  Fri Nov 24 08:46:35 2006 jofret
## Last update Wed Mar  7 18:37:21 2007 jofret
##
## Copyright INRIA 2006
##


# set -x

end="[01;37m"
red="[01;31m"
green="[01;32m"
yellow="[01;33m"
blue="[01;34m"
pink="[01;35m"
cyan="[01;36m"


progress=0
scilab="../YetAnotherScilabParser"

good()
{
    echo "${yellow}[ Lets Go for A set of good tests ]${end}"
    for i in `ls good/*.sci good/*.sce`; do
	echo "${cyan}[ ----==== Running Test $i ====---- ]${end}"
	echo
	all_good=$(($all_good + 1))
	$scilab $i
	if [ $? -eq 0 ]; then
	    echo "[01;32mTEST $i OK[0m"
	else
	    echo "[01;31mTEST $i FAILED[0m"
	    er_good=$(($er_good + 1))
	    if [ $progress -eq 1 ]; then
		$scilab --parse-trace $i
		exit 1
	    fi
	fi
	echo
    done
}

cira()
{
    echo "${yellow}[ Lets Go for A set of CIRA tests ]${end}"
    for i in `ls CIRA/avant/*.sci CIRA/avant/*.sce CIRA/apres/*.sci CIRA/apres/*.sce`; do
	echo "${cyan}[ ----==== Running Test $i ====---- ]${end}"
	echo
	all_cira=$(($all_cira + 1))
	$scilab "$i"
	if [ $? -eq 0 ]; then
	    echo "[01;32mTEST $i OK[0m"
	else
	    echo "[01;31mTEST $i FAILED[0m"
	    er_cira=$(($er_cira + 1))
	    if [ $progress -eq 1 ]; then
		$scilab --parse-trace $i
		exit 1
	    fi
	fi
	echo
    done
}

progress()
{
    echo "${yellow}[ Lets Go for A set of progress tests ]${end}"
    progress=1
    good
    if [ $? -eq 0 ]; then
	cira
    fi
}

syntax()
{
    echo "${yellow}[ Lets Go for A set of syntax tests ]${end}"
    for i in `ls syntax/*.sci syntax/*.sce`; do
	echo "${cyan}[ ----==== Running Test $i ====---- ]${end}"
	echo
	all_syntax=$(($all_syntax + 1))
	$scilab $i
	a=$?
	if [ $a -eq 2 -o $a -eq 3 ]; then
	    echo "${green}TEST $i OK${end}"
	else
	    echo "${red}TEST $i FAILED${end}"
	    er_syntax=$(($er_syntax + 1))
	fi
	echo
    done
}

all_good=0
er_good=0
all_cira=0
er_cira=0
all_syntax=0
er_syntax=0
exit_val=0

case $1 in
    good)
	good
	;;
    syntax)
	syntax
	;;
    cira)
	cira
	;;
    progress)
	progress
	;;
    *)
	good
	syntax
	cira
	;;
esac

echo
echo
echo " ---*****************---"
echo " -*-                 -*-"
echo " -*- RESULTS SUMMARY -*-"
echo " -*-                 -*-"
echo " ---*****************---"
echo
if [ $all_good -ne 0 ]; then
    echo "${blue}--> Good tests${end}"
    echo "${cyan}$all_good${end} test"
    pass_good=$(($all_good - $er_good))
    echo "${green}$pass_good${end} test passed"
    echo "${red}$er_good${end} tests error"
    good_passed=$((100 - $er_good * 100 / $all_good))
    echo "${yellow}${good_passed}%${end} passed..."
    echo
fi
if [ $all_cira -ne 0 ]; then
    echo "${blue}--> Cira tests${end}"
    echo "${cyan}$all_cira${end} test"
    pass_cira=$(($all_cira - $er_cira))
    echo "${green}$pass_cira${end} test passed"
    echo "${red}$er_cira${end} tests error"
    cira_passed=$((100 - $er_cira * 100 / $all_cira))
    echo "${yellow}${cira_passed}%${end} passed..."
    echo
fi
if [ $all_syntax -ne 0 ]; then
    echo "${blue}--> Syntax tests${end}"
    echo "${cyan}$all_syntax${end} test"
    pass_syntax=$(($all_syntax - $er_syntax))
    echo "${green}$pass_syntax${end} test passed"
    echo "${red}$er_syntax${end} error"
    syntax_passed=$((100 - $er_syntax * 100 / $all_syntax))
    echo "${yellow}${syntax_passed}%${end} passed..."
    echo
fi

exit 0
