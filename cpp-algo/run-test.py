#!/usr/bin/env python
import os
import subprocess
import shutil
def run_test_ubuntu():
     shutil.rmtree("build", ignore_errors=True)
     os.mkdir("build")
     os.chdir("build")
     subprocess.run("cmake ../ && make && ./cpp-algo", shell=True, check=True)

def run_test_vs2017():
     shutil.rmtree("build", ignore_errors=True)
     os.mkdir("build")
     os.chdir("build")
     cmake = "C:/Program Files (x86)/Microsoft Visual Studio/2017/Enterprise/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe"
     vs_version = "Visual Studio 15 2017 Win64"
     ms_build = "C:/Program Files (x86)/Microsoft Visual Studio/2017/Enterprise/MSBuild/15.0/Bin/amd64/MSBuild.exe"
     subprocess.run([cmake,  "-G" , vs_version , "..\\"], check=True)
     subprocess.run([ms_build, "/p:Configuration=Release",  "/p:Platform=x64", "cpp-algo.sln" ], check=True)

if __name__ == '__main__':
    if os.name == "posix":
        run_test_ubuntu()
    else:
        run_test_vs2017()
    




