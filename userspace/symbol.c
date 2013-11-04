/*
 * symbol.c
 *
 * This file is part of ktap by Jovi Zhangwei.
 *
 * Copyright (C) 2013 Azat Khuzhin <a3at.mailgmail.com>.
 *
 * ktap is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * ktap is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "symbol.h"

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <libelf.h>

/**
 * TODO: split into small helpers
 */
vaddr_t find_symbol(const char *exec, const char *symbol)
{
    GElf_Ehdr *elf_header = NULL;
    Elf *elf = NULL;
    Elf_Scn *scn = NULL;
    Elf_Data *elf_data = NULL;
    GElf_Sym sym;
    GElf_Shdr shdr;

    int fd;
    struct stat stat;
    char *buffer;
    vaddr_t vaddr = 0;

    if ((fd = open(exec, O_RDONLY)) == -1) {
        goto err;
    }
    if ((fstat(fd, &stat))) {
        goto err_close;
    }
    if (!(buffer = (char *)malloc(stat.st_size))) {
        goto err_close;
    }
    if ((read(fd, buffer, stat.st_size)) < stat.st_size) {
        goto err_free;
    }

    if (elf_version(EV_CURRENT) == EV_NONE) {
        goto err_free;
    }
    elf_header = (GElf_Ehdr *)buffer;
    elf = elf_begin(fd, ELF_C_READ, NULL);

    while ((scn = elf_nextscn(elf, scn))) {
        int i;
        int symbols;
        char *current_symbol;

        gelf_getshdr(scn, &shdr);

        if (shdr.sh_type != SHT_SYMTAB) {
            continue;
        }

        elf_data = elf_getdata(scn, elf_data);
        symbols = shdr.sh_size / shdr.sh_entsize;

        for (i = 0; i < symbols; i++) {
            gelf_getsym(elf_data, i, &sym);

            if (GELF_ST_TYPE(sym.st_info) != STT_FUNC) {
                continue;
            }

            current_symbol = elf_strptr(elf, shdr.sh_link, sym.st_name);
            if (!strcmp(current_symbol, symbol)) {
                vaddr = sym.st_value;
                goto success;
            }
        }
    }

success:
err_free:
    if (elf) {
        elf_end(elf);
    }
    free(buffer);
err_close:
    close(fd);
err:
    return vaddr;
}
