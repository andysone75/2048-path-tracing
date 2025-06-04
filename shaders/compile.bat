@echo off
setlocal enabledelayedexpansion

:: Checking for availability glslc.exe in the PATH
where glslc.exe >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: glslc.exe not found in the PATH. Make sure that it is installed and added to the environment variables.
    pause
    exit /b 1
)

:: We compile everything .vert and .frag files in the current folder
for %%f in (*.vert *.frag) do (
    set "filename=%%~nf"
    set "ext=%%~xf"
    set "ext=!ext:~1!"  :: Removing the dot from the extension
    set "output=!filename!-!ext!.spv"
    
    echo Compiling: "%%f" -> "!output!"
    glslc.exe "%%f" -o "!output!"
    
    if !errorlevel! neq 0 (
        echo Compile error "%%f"
    ) else (
        echo Success: "!output!"
    )
)

echo All shaders are processed.
pause