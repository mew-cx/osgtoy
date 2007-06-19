REM Set environment variables for OpenSceneGraph

REM We assume these two envars are already defined by package installation:
REM set OSG_ROOT=C:\Program Files\OpenSceneGraph
REM set OSG_FILE_PATH=%OSG_ROOT%\data;%OSG_ROOT%\data\Images;%OSG_ROOT%\data\fonts

set PATH=%OSG_ROOT%\bin;%PATH%

set PRODUCER_CAMERA_CONFIG_FILE=%OSG_ROOT%\data\Producer\1win.cfg

REM Choose a default notify level if desired:
REM set OSG_NOTIFY_LEVEL=ALWAYS
REM set OSG_NOTIFY_LEVEL=FATAL
REM set OSG_NOTIFY_LEVEL=WARN
REM set OSG_NOTIFY_LEVEL=NOTICE
REM set OSG_NOTIFY_LEVEL=DEBUG
REM set OSG_NOTIFY_LEVEL=DEBUG_FP
REM set OSG_NOTIFY_LEVEL=INFO

