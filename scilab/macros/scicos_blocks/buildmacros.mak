all:: Branching Electrical Events Linear Misc NonLinear Sinks Sources Threshold hydraulics

Branching	:
	@cd Branching
	@makemacros.bat
	@cd ..

Electrical	:
	@cd Electrical
	@makemacros.bat
	@cd ..

Events	:
	@cd Events
	@makemacros.bat
	@cd ..

Linear	:
	@cd Linear
	@makemacros.bat
	@cd ..

Misc	:
	@cd Misc
	@makemacros.bat
	@cd ..

NonLinear	:
	@cd NonLinear
	@makemacros.bat
	@cd ..

Sinks	:
	@cd Sinks
	@makemacros.bat
	@cd ..

Sources	:
	@cd Sources
	@makemacros.bat
	@cd ..

Threshold	:
	@cd Threshold
	@makemacros.bat
	@cd ..

hydraulics	:
	@cd hydraulics
	@makemacros.bat
	@cd ..

clean::
	-del *.bin /s
	-del names.* /s
	-del genlib /s
