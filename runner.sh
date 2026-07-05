#!/bin/bash
set -e

if [ ! -d "build" ]; then
  echo "No existe la carpeta build/, generando el proyecto con CMake..."
  mkdir -p build
  cd build
  cmake -G "Ninja Multi-Config" ..
  cd ..
fi

echo "Compilando Project_AK ..."
cmake --build build --config Debug --target Project_AK -- -j 8
echo "Compilacion terminada! "

# En Windows (MSVC/Ninja) el binario sale con extension .exe; en Linux/Mac, sin ella.
EXE="build/Debug/Project_AK"
if [ -f "build/Debug/Project_AK.exe" ]; then
  EXE="build/Debug/Project_AK.exe"
fi

echo "Ejecucion en proceso :)"
echo "| ------------------------------------------------------------------------------------------------ |"
"$EXE"
echo "| ------------------------------------------------------------------------------------------------ |"
