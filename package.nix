{
  stdenv,
  lib,
  fetchFromGitHub,
  cmake,
  gnumake,
  gcc,
  yaml-cpp,
}:

stdenv.mkDerivation rec {
  pname = "pdftms";
  version = "0-unstable-2025-02-14";

  src = fetchFromGitHub {
    owner = "pingponghero12";
    repo = "pdftms";
    rev = "c06fb29e9bd8d7cdd441c9d8e4ff05af0c0ed229";
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
    install -Dm755 build/pdftms -t $out/bin
  '';

  meta = with lib; {
    description = "PDF TUI Managment System";
    homepage = "https://github.com/pingponghero12/pdftms";
    license = licenses.mit;
    maintainers = [ maintainers.pingponghero12 ];
    platforms = platforms.linux;
  };
}
