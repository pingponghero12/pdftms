# pdftms
PDF TUI Managment System

Simple TUI manager for obsidian-like vault of `.pdf` files. Uses `fzf` for blazingly fast opening files inside vault in your favourite pdf reader. All commands get things done thanks to `fzf`.

## Installation
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

You can ofcourse still just `nix-shell` and do the manual cmake build. This will also generate `compile_commands.json` for clangd LSP.
```
mkdir -p build
cd build 
cmake ..
make
```

## Usage

```sh
pdftms <command> [arguments]
```

## Commands

- `create <path> <pdf_reader>`  
  Create a PDF vault at the specified path with the given PDF viewer.

- `mv <file>`  
  Move the specified PDF file to the PDF vault.

- `add <file>`  
  Copy the specified PDF file to the PDF vault.

- `mkdir`  
  Create a directory inside the PDF vault.

- `rename`  
  Rename a file inside the PDF vault.

- `help`  
  Show this help message.

## Examples

### Create a Vault

```sh
pdftms create ~/path/to/vault xreader
```

This command creates a PDF vault at `/path/to/vault` and sets `xreader` as the PDF viewer.

### Add a File

```sh
pdftms add /path/to/file.pdf
```

This command copies `/path/to/file.pdf` to the PDF vault.

## Functional nature
`pdftms` is written with functional programming paradigm in mind. This allows for easier testing and what I believe to be cleaner code. The shortness and flow through nature of the program perfectly supports the use case of this programming style.

## To add
1. Move files inside of vault
2. Vault tree view

