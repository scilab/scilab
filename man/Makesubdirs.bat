@echo off
@echo Checking French manual
@cd fr
@nmake /C /F check.mak all /A
@cd ..
@echo Checking English manual
@cd eng
@nmake /C /F check.mak all /A
@cd ..
@echo on
