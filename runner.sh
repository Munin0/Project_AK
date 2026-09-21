#!/bin/bash
set -e

LOGFILE="logs/run_$(date +%Y%m%d_%H%M%S).log"
mkdir -p logs
exec > >(tee -a "$LOGFILE") 2>&1
echo "Log guardado en: $LOGFILE"

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
export ASAN_OPTIONS="malloc_context_size=50:fast_unwind_on_malloc=0"
export LSAN_OPTIONS="suppressions=$(pwd)/lsan_suppressions.txt"

EXE="build/Debug/Project_AK"
if [ -f "build/Debug/Project_AK.exe" ]; then
  EXE="build/Debug/Project_AK.exe"
fi

echo "Ejecucion en proceso :)"
echo "| ------------------------------------------------------------------------------------------------ |"
"$EXE" || echo "El programa terminó con error (código $?)"
echo "| ------------------------------------------------------------------------------------------------ |"
