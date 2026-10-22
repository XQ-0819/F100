@echo off
setlocal EnableExtensions

chcp 65001 >nul
cd /d "%~dp0"
if errorlevel 1 (
    echo [错误] 无法进入脚本所在目录：%~dp0
    exit /b 1
)

set "cleanup_failed=0"

echo --------------------------------------------------
echo 正在清理嵌入式工程生成文件...
echo 清理范围：%CD%
echo --------------------------------------------------

rem 只清理明确的编译生成文件和临时文件，不删除 .bak、.orig 或通用 HTML 文档。
for /r %%F in (*.SearchResults *.ddk *.edk *.lst *.lnp *.mpf *.mpj *.obj *.omf *.plg *.rpt *.tmp *.__i *.crf *.o *.d *.tra *.dep *.iex *.build_log.htm *.uvgui.* *.uvoptx.bak JLinkLog.txt) do (
    if exist "%%F" (
        del /f /q /a "%%F" 2>nul
        if exist "%%F" (
            echo [错误] 无法删除：%%F
            set "cleanup_failed=1"
        )
    )
)

rem 复用模拟器现有清理逻辑，删除模拟器构建、单元测试输出和错误日志，不触发 clean.cmd 的暂停。
if exist "simulator\clean.ps1" (
    echo 正在清理模拟器生成文件...
    powershell.exe -NoProfile -ExecutionPolicy Bypass -File "simulator\clean.ps1" -Target Build
    if errorlevel 1 (
        echo [错误] 模拟器生成文件清理失败。
        set "cleanup_failed=1"
    )
) else (
    echo [错误] 未找到 simulator\clean.ps1，无法清理模拟器生成文件。
    set "cleanup_failed=1"
)

rem CHM 为构建产物：先复制到临时文件，成功后再替换根目录产物并删除 HTML。
if exist "html\" (
    if not exist "html\readme.chm" (
        echo [错误] html\readme.chm 不存在，已保留 html 目录。
        set "cleanup_failed=1"
    ) else (
        copy /y "html\readme.chm" "%CD%\readme.chm.tmp" >nul
        if errorlevel 1 (
            echo [错误] readme.chm 临时复制失败，已保留 html 目录和原有 CHM。
            set "cleanup_failed=1"
        ) else (
            move /y "%CD%\readme.chm.tmp" "%CD%\readme.chm" >nul
            if errorlevel 1 (
                echo [错误] readme.chm 替换失败，已保留 html 目录和原有 CHM。
                del /f /q "%CD%\readme.chm.tmp" >nul 2>nul
                set "cleanup_failed=1"
            ) else (
                rmdir /s /q "html"
                if exist "html\" (
                    echo [错误] html 目录删除失败。
                    set "cleanup_failed=1"
                )
            )
        )
    )
)

echo --------------------------------------------------
if "%cleanup_failed%"=="0" (
    echo 清理完成。
    echo --------------------------------------------------
    exit /b 0
)

echo 清理未完全成功，请检查以上错误信息。
echo --------------------------------------------------
exit /b 1
