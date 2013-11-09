/*
 * symbol.h
 *
 * This file is part of ktap by Jovi Zhangwei.
 *
 * Copyright (C) 2013 Azat Khuzhin <a3at.mail@gmail.com>.
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

/**
 * TODO: add instead listing functions
 * TODO: error messages
 */

#include <gelf.h>

#define SDT_NOTE_TYPE 3
#define SDT_NOTE_SCN ".note.stapsdt"
#define SDT_NOTE_NAME "stapsdt"

#define FIND_SYMBOL 1
#define FIND_STAPSTD_NOTE 2

typedef GElf_Addr vaddr_t;

/**
 * Find symbol in DSO
 *
 * @return 0 on failure, otherwise address of symbol.
 */
vaddr_t find_symbol(const char *exec, const char *symbol, int type);
