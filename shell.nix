{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
    buildInputs = [
        pkgs.cmake
        pkgs.gnumake
        pkgs.gcc

        pkgs.fzf
        pkgs.yaml-cpp
    ];
}
