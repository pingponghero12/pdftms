{
  stdenv,
  lib,
  fetchFromGitHub,
  cmake,
  gnumake,
  gcc,
  yaml-cpp
}:

stdenv.mkDerivation rec {
  pname = "pdftms";
  version = "1.0.0";

  src = fetchFromGitHub {
    owner = "pingponghero12";
    repo = "pdftms";
    rev = "main";
    sha256 = "sha256-Stb1QHCa3AcVZaGoBBQM8fA6Fm7ZCHvb35rwNsjxFY8=";
  };

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

  meta = with lib; {
    description = "PDF TUI Managment System";
    homepage = "https://github.com/pingponghero12/pdftms";
    license = licenses.mit;
    maintainers = [ maintainers.pingponghero12 ];
    platforms = platforms.linux;
  };
}
