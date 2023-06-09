# cppTemplate
a simple template for other projects while messing around in vs code.

## To use
terminal > run tasks, pick a compiler.  Then `.\rooster.exe`

## tasks.json
- terminal > configure task for compilers   
- enables: terminal > run tasks
```
{
	"version": "2.0.0",
	"tasks": [
		{
			"type": "cppbuild",
			"label": "Build with G++",
			"command": "C:\\msys64\\mingw64\\bin\\g++.exe",
			"args": [
				"-fdiagnostics-color=always",
				"-g",
				"-std=c++20",
				"${workspaceFolder}\\*.cpp",
				"-o",
				"${fileDirname}\\rooster.exe"
			],
			"options": {
				"cwd": "${fileDirname}"
			},
			"problemMatcher": [
				"$gcc"
			],
			"group": "build",
			"detail": "compiler: C:\\msys64\\mingw64\\bin\\g++.exe"
		},
		{
			"type": "cppbuild",
			"label": "Build with Clang",
			"command": "C:\\msys64\\mingw64\\bin\\clang++.exe",
			"args": [
				"-fdiagnostics-color=always",
				"-g",
				"-std=c++20",
				"${workspaceFolder}\\*.cpp",
				"-o",
				"${fileDirname}\\rooster.exe"
			],
			"options": {
				"cwd": "${fileDirname}"
			},
			"problemMatcher": [
				"$gcc"
			],
			"group": "build",
			"detail": "compiler: C:\\msys64\\mingw64\\bin\\clang++.exe"
		},
		{
			"type": "cppbuild",
			"label": "Build with MSVC",
			"command": "cl.exe",
			"args": [
				"/Zi",
				"/std:c++latest",
				"/EHsc",
				"/Fe:",
				"${fileDirname}\\rooster.exe",
				"${workspaceFolder}\\*.cpp"
			],
			"options": {
				"cwd": "${fileDirname}"
			},
			"problemMatcher": [
				"$msCompile"
			],
			"group": "build",
			"detail": "compiler: cl.exe"
		}
	]
}
```

## c_cpp_properties.json
- open command pallette  
- C/C++:Edit Configurations (UI)  
- change C++ standard to 20  
```
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "windowsSdkVersion": "10.0.19041.0",
            "compilerPath": "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.33.31629/bin/Hostx64/x64/cl.exe",
            "cStandard": "c17",
            "cppStandard": "c++20",
            "intelliSenseMode": "windows-msvc-x64"
        }
    ],
    "version": 4
}
```