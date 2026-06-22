#!/bin/bash
set -e

if [ ! -d "build" ]; then
  echo "No existe la carpeta build/, generando el proyecto con CMake..."
  cmake -S . -B build -G "Ninja Multi-Config"
fi

echo "Compilando Project_AK ..."
cmake --build build --config Debug --target Project_AK -- -j 8
echo "Compilacion terminada! "
echo "Ejecucion en proceso :)"
echo "| ------------------------------------------------------------------------------------------------ |"
build/Debug/Project_AK
echo "| ------------------------------------------------------------------------------------------------ |"
