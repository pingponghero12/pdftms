{ pkgs ? import <nixpkgs> {} }:

pkgs.stdenv.mkDerivation rec {
  pname = "pdftms";
  version = "1.0.0";
  src = ./.;

  buildInputs = [
    pkgs.cmake
    pkgs.gnumake
    pkgs.gcc
    pkgs.yaml-cpp
  ];

  # Use -B -S to avoid error with normal mkdir -p build etc
  configurePhase = ''
    cmake -B build -S .
  '';

  buildPhase = ''
    make -C build
  '';

  installPhase = ''
    mkdir -p $out/bin
    cp build/pdftms $out/bin/
  '';
}
