@echo off

rem Prerequisites
rem   python3: https://www.python.org/
rem   meson: http://mesonbuild.com/
rem   ninja: https://ninja-build.org/
rem   git: https://git-scm.com/

if not exist "..\catch\.git" (
	git submodule update --init --remote --depth 1 --recursive
)

if not defined CL_EXIST (
	call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64
	set CL_EXIST=1
)

if not exist "_venv" (
	virtualenv "_venv"
	call "_venv\Scripts\activate.bat"
	pip3 install meson
) else (
	call "_venv\Scripts\activate.bat"
)

if not exist "_build" (
	python "_venv\Scripts\meson.py" "_build"
	rem --backend vs
)

cd "_build"
ninja -j2
