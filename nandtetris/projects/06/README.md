# Hack Assembler

The Hack Assembler translates Hack Assembly Language (as specified in the [nand2tetris course](https://www.nand2tetris.org/)) into Hack Machine Language (binary code). This program processes `.asm` files and generates corresponding `.hack` files that can be loaded into the Hack computer.

## Features
- Converts `A` and `C` instructions from Hack assembly language into binary.
- Supports predefined symbols (e.g., `@R0`, `@SCREEN`).
- Handles labels and user-defined symbols.

---

## Requirements

- **Compiler**: GCC or Clang
- **OS**: Linux or any UNIX-like system
- **Memory**: Minimal, as the assembler operates on text files.

---

## Installation

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd <repository-directory>

2. Build the assembler using the make file
    make or make all

## Usage

### Command Syntax
./main <input-file.asm>

### Example
./main asm/Max.asm

### Output
The assembler will generate a file named Max.asm.hack, in the asm folder, containing the binary instructions.

### Notes on Input Files
- **Format**: The assembler expected the input file to be in Hack assembly language.

## Common Issues
- Error: "Usage ./main [filename]"
    - **Cause**: Missing or incorrect input file.
    - **Fix**: Provide a valid .asm file as an argument.
- Error: "Cannot open file <filename>"
    - **Cause**: File either does not exist or cannot be read.
    - **Fix**: Verify that the file exists and that you have read permissions.
- Error: "File contains more than the maximum number of X instructions"
    - **Cause**: The .asm input file has more than 30,000 instructions.
    - **Fix** In main.c, adjust MAX_INSTRUCTION_COUNT to a suitable number, then recompile.
- Error: "Line X: S: ..."
    - **Cause**: The .asm input file contains an error that halts assembling.
    - **Fix**: Go to specifed line in the .asm file and correct the errors to comply with the Hack assembly language standard.

## Credit
This program was made by Eric Orr, while attending OSU-Cascades, following the Nand2tetris lesson plan.
Should this git repository become public, none of the code associated with this program should be used for student projects.
# DON'T STEAL YOUR OWN EDUCATION FROM YOURSELF
