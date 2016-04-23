/* $Id$
 * $URL$
 *
 * string plugin
 *
 * Copyright (C) 2003, 2004 Michael Reinelt <michael@reinelt.co.at>
 * Copyright (C) 2004 The LCD4Linux Team <lcd4linux-devel@users.sourceforge.net>
 *
 * This file is part of LCD4Linux.
 *
 * LCD4Linux is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * LCD4Linux is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/* 
 * exported functions:
 *
 * int plugin_init_string (void)
 *  adds some handy string functions
 *
 */


#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "debug.h"
#include "plugin.h"


static void my_strlen(RESULT * result, RESULT * arg1)
{
    double value = strlen(R2S(arg1));
    SetResult(&result, R_NUMBER, &value);
}

/* 'upcase' function (shamelessly stolen from plugin_sample.c)*/
/* returns the string in upper case letters */
static void my_strupper(RESULT * result, RESULT * arg1)
{
    char *value, *p;

    value = strdup(R2S(arg1));

    for (p = value; *p != '\0'; p++)
	*p = toupper(*p);

    SetResult(&result, R_STRING, value);
    free(value);
}

static void my_strstr(RESULT * result, RESULT * arg1, RESULT * arg2)
{
    char *p;
    double value;

    char *haystack = R2S(arg1);
    char *needle = R2S(arg2);

    p = strstr(haystack, needle);

    if (p == NULL) {
	value = -1;
    } else {
	value = p - haystack;
    }

    SetResult(&result, R_NUMBER, &value);
}

static void my_substr(RESULT * result, int argc, RESULT * argv[])
{
    char *str, *p1, *p2;
    int pos, len;

    if (argc < 2 || argc > 3) {
	error("substr(): wrong number of parameters");
	SetResult(&result, R_STRING, "");
	return;
    }

    str = strdup(R2S(argv[0]));

    pos = R2N(argv[1]);
    if (pos < 0)
	pos = 0;

    if (argc == 3) {
	len = R2N(argv[2]);
	if (len < 0)
	    len = 0;
    } else {
	len = -1;
    }

    p1 = str;
    while (pos > 0 && *p1 != '\0') {
	p1++;
	pos--;
    }

    if (len >= 0) {
	p2 = p1;
	while (len > 0 && *p2 != '\0') {
	    p2++;
	    len--;
	}
	*p2 = '\0';
    }

    SetResult(&result, R_STRING, p1);
    free(str);
}

static void my_charat(RESULT * result, RESULT * arg1, RESULT * arg2)
{
	char at_result[2];
	char* org_str = strdup(R2S(arg1));
	int pos = R2N(arg2);

	at_result[1] = 0;

	if (pos > strlen(org_str) - 1 || pos < 0) {
		at_result[0] = 0;
		SetResult(&result, R_STRING, at_result);
		free(org_str);
		return;
	}

	at_result[0] = org_str[pos];
	SetResult(&result, R_STRING, at_result);
	free(org_str);
	return;
}

static void my_rcharat(RESULT * result, RESULT * arg1, RESULT * arg2)
{
	char at_result[2];
	char* org_str = strdup(R2S(arg1));
	int len = strlen(org_str);
	int pos = R2N(arg2);

	at_result[1] = 0;

	if (pos > strlen(org_str) - 1 || pos < 0) {
		at_result[0] = 0;
		SetResult(&result, R_STRING, at_result);
		free(org_str);
		return;
	}

	at_result[0] = org_str[len - pos - 1];
	SetResult(&result, R_STRING, at_result);
	free(org_str);
	return;
}

int plugin_init_string(void)
{

    /* register some basic string functions */
    AddFunction("strlen", 1, my_strlen);
    AddFunction("strupper", 1, my_strupper);
    AddFunction("strstr", 2, my_strstr);
    AddFunction("substr", -1, my_substr);
    AddFunction("charat", 2, my_charat);
    AddFunction("rcharat", 2, my_rcharat);
    return 0;
}

void plugin_exit_string(void)
{
    /* empty */
}
