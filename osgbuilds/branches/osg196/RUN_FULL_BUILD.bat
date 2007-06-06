title %0
call SETENV.bat
%VSIDE% /build %BUILDCFG% /project INSTALL %SOLUTION%
@echo DONE
@date /t
@time /t
@pause
