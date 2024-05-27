#!/bin/bash

cd bin
# En-têtes pré-compilées
cmake -H../src/Engine/Core/Headers/Headers.hpp .
make -j
./main
cd ..