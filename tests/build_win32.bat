@echo off

rem Prerequisites
rem   git: https://git-scm.com/
rem   msvc: https://visualstudio.microsoft.com
rem   python3: https://www.python.org/

if not exist "catch2\.git" (
	git submodule update --init
)

if not "%VSCMD_ARG_TGT_ARCH%" == "x64" (
	call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
)

if not exist ".venv" (
	python -m venv ".venv"
	call ".venv\Scripts\activate.bat"
	pip3 install meson ninja
) else (
	call ".venv\Scripts\activate.bat"
)

if not exist "_build" (
	meson setup "_build" -Db_sanitize=none
	rem --backend vs
)

(
	meson ^
		compile ^
		-C "_build" ^
		-j 2
)
