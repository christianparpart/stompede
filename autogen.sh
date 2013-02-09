#! /bin/bash

if [ -z "$CXX" ]; then
	if which clang &>/dev/null; then
		export CC=$(which clang)
	fi
	if which clang++ &>/dev/null; then
		export CXX=$(which clang++)
	fi
fi

if [ "$1" == "clean" ]; then
	find . \( -name 'CMakeCache.txt' -o -name 'CMakeFiles' \
			-o -name 'Makefile' -o -name cmake_install.cmake \) \
		-exec rm -rf {} \; 2>/dev/null
else
	cmake "$(dirname $0)" \
		-DCMAKE_CXX_FLAGS_DEBUG="-O0 -g3" \
		-DCMAKE_BUILD_TYPE="debug" \
		-DCMAKE_INSTALL_PREFIX="${HOME}/local"
fi
