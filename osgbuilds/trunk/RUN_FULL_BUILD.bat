title %0
call SETENV.bat

@echo START
@date /t
@time /t

%VSIDE% /build %BUILDCFG% /project INSTALL %SOLUTION%

@echo DONE
@date /t
@time /t

@pause
