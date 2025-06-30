{
  description = "Full dev shell: CMake, GCC, GTK4, OpenGL, Vulkan, shader toolchain and profiling";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in {
        devShells.default = pkgs.mkShell {
          name = "graphics-dev-shell";

          nativeBuildInputs = with pkgs; [
            cmake
            gnumake
            gcc
            pkg-config
            clang-tools
          ];

          buildInputs = with pkgs; [
            # GUI and core libraries
            gtk4
            glib
            libsysprof-capture

            # OpenGL
            libepoxy
            mesa
            libglvnd.dev
            wayland
            wayland-protocols
            wayland-utils
            xorg.libX11
            xorg.libXext
            xorg.libXrandr
            xorg.libXi

            # Vulkan
            vulkan-loader
            vulkan-headers
            vulkan-tools
            vulkan-validation-layers
            shaderc

            # Shader toolchain
            glslang
            spirv-tools
            spirv-cross

            # Debug & profiling
            gdb
            lldb
            valgrind
            lcov
            sysprof
          ];

          shellHook = ''
            echo "=============================================="
            echo "🚀 Dev shell ready for C/C++ graphics apps"
            echo "   GCC     = $(gcc --version | head -n1)"
            echo "   GTK4    = $(pkg-config --modversion gtk4)"
            echo "   OpenGL  = $(pkg-config --modversion gl)"
            echo "   Vulkan  = $(vulkaninfo --summary 2>/dev/null | head -n1)"
            echo "=============================================="

            export CC=gcc
            export CXX=g++
            export PKG_CONFIG_PATH="${pkgs.gtk4.dev}/lib/pkgconfig:${pkgs.vulkan-loader.dev}/lib/pkgconfig:$PKG_CONFIG_PATH"
          '';
        };
      });
}
