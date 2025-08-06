# Naos
Not another operating system, simplest version (32-bit x86)

## Description
This a little modified version of the [bare bones tutorial](https://wiki.osdev.org/Bare_Bones).

## Using makefile
First run:
```bash
make build
```
If you miss some dependencies install them and then rerun after with
```bash
make clean && make build
```
If the build was successful, you will see an iso image in root directory of project, and then you can run it with qemu:
```bash
make run
```

## LSP in zeditor
```json
"lsp": {
  "cpp": {
    "binary": {
      "path": "/home/egrapa/opt/cross/bin/i686-elf-g++"
    }
  }
}
```
