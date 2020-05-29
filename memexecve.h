#pragma once

#include <stdlib.h>

void exec_elf(char* raw_elf, size_t elf_size, char** argv, char** envp);
