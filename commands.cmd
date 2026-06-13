# Inside build/
cmake -G"Ninja Multi-Config" ..

## After building, outside build/
cmake --build build/ --config Debug --target Project_AK
