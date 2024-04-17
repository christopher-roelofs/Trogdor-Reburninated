rm -r build_portmaster
mkdir build_portmaster
cd build_portmaster
cmake ../ -DRG35XX=ON -DLINUX=ON -DPRELOAD_SOUNDS=ON
make