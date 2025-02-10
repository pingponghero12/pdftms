# pdftms
PDF TUI Managment System

## Usage
Program is set up to be easily build and installed using nix package manager. 

Build and run for development:
```bash
# Ensure that ./build does not exist `rm -rf build`
nix-build

# Run
./result/bin/pdftms
```

Pernament install with PATH update:
```bash
nix-env -i -f default.nix

# Unistall
nix-env -e pdftms
```

You can ofcourse still just `nix-shell` and do the manual cmake build:
```
mkdir -p build
cd build 
cmake ..
make
```

## Functional nature
`pdftms` is written with functional programming paradigm in mind. This allows for easier testing and what I believe to be cleaner code. The shortness and flow through nature of the program perfectly supports the use case of this programming style.
