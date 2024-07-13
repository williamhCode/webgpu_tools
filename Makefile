.PHONY: build

TYPE = release

build:
	cmake --build build/$(TYPE) --target webgpu_tools

build-setup:
	cmake . -B build/release \
		-DCMAKE_BUILD_TYPE=RelWithDebInfo \
		-GNinja \
		-DCMAKE_C_COMPILER=clang \
		-DCMAKE_CXX_COMPILER=clang++ \
		-DCMAKE_C_COMPILER_LAUNCHER=ccache \
		-DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
		-DCMAKE_COLOR_DIAGNOSTICS=ON

clean:
	cmake --build build/$(TYPE) --target clean

install:
	cmake --install build/$(TYPE)

