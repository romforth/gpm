/*
 * Copyright (c) 2026 Charles Suresh <romforth@proton.me>
 * SPDX-License-Identifier: AGPL-3.0-only
 * Please see the LICENSE file for the Affero GPL 3.0 license details
 */

#include <stdio.h>	// getchar, putchar, EOF
#include <string.h>	// strcmp, strdup
#include <stdlib.h>	// free

char *
skip(char open, char close, char *s) {
	char c;
	int nest=1;

	while (c=*s) {
		if (c==close && --nest==0) {
			*s=0;
			return s+1;
		}
		if (c==open) nest++;
		s++;
	}
	return 0;
}

void
match(char open, char close, char *s) {
	char c;
	int nest=1;

	while ((c=getchar()) != EOF) {
		if (c==close && --nest==0) {
			*s++=0;
			return;
		}
		if (c==open) nest++;
		*s++=c;
	}
}

char *
subst(char *src, char *dst, int arg, char **args) {
	char c;

	// printf("subst src:%s arg:%d\n", src, arg);
	while (c=*src++) {
		if (c=='$') {
			c=*src++;
			if (c=='*') {
				for (int i=1; i<arg; i++) {
					for (char *p=args[i]; *dst=*p++; dst++);
					if (i!=arg-1) *dst++=' ';
				}
				continue;
			}
			int n=c-'0';
			for (char *p=args[n]; *dst=*p++; dst++);
		} else {
			*dst++=c;
		}
	}
	return dst;
}

int
split(char *buf, int arg, char **args) {
	char c;

	args[arg++]=buf;
	while (c=*buf) {
		if (c=='[') {
			args[arg-1]++;
			buf=skip('[',']',++buf);
			if (buf) {
				args[arg++]=buf;
				continue;
			}
		}
		if (c==' ') {
			*buf++=0;
			args[arg++]=buf;
		} else {
			buf++;
		}
	}
	return arg;
}

#define NMACROS 1000

struct macro {
	char *name;
	char *body;
} macros[NMACROS];

int nmac=0;

char *
find(char *m) {
	for (int i=nmac-1;i>=0;i--) {
		if (strcmp(m, macros[i].name)) continue;
		return macros[i].body;
	}
	return 0;
}

char *process(int top, char *buf);

char *
expand(char *src, char *dst) {
	char *args[100];

	int arg=split(src, 0, args);

	char *name=args[0];

	if (strcmp(name, "def")) {
		char *body;
		if (name[0]=='{') {
			name=process(1, name);
			body=find(name);
			free(name);
		} else {
			body=find(name);
		}
		if (body) {
			dst=subst(body, dst, arg, args);
		} else {
			for (int i=0;i<arg;i++) {
				dst=subst(args[i], dst, arg, args);
				if (i!=arg-1) *dst++=' ';
			}
		}
		*dst=0;
	} else {
		macros[nmac].name=strdup(args[1]);
		macros[nmac].body=strdup(args[2]);
		nmac++;
	}
	return dst;
}

int
macro(char *src, char *dst) {
	char c;
	int flag=0;

	while (c=*src++) {
		if (c=='{') {
			char *next=skip('{', '}', src);
			dst=expand(src, dst);
			src=next;
			flag=1;
		} else {
			*dst++=c;
		}
	}
	return flag;
}

#define MACROLEN 10000

char *
process(int top, char *buf) {
	char dst[MACROLEN], *cur=dst;
	char src[MACROLEN];
	int redo=0;
	char c, *s=buf, *d=dst;

	// printf("process %d, %s\n", top, buf);
	while (c=*s++) {
		if (c!='{') {
			*d++=c;
			continue;
		}
		redo=1;
		char *next=skip('{', '}', s);
		char *ret=process(top+1, s);
		char *r=ret; while (*d++=*r++); // strcpy(dst, ret);
		free(ret);
		d--;
		s=next;
	}
	*d=0;
	if (redo) return process(top, dst);
	// printf("process rest %d, %s\n", top, buf);
	*dst=0;
	expand(buf, dst);
	for(;;) {
		if (macro(dst, src)) {
			if (macro(src, dst)) {
				continue;
			}
			cur=src;
		}
		if (top==0) {
			printf("%s", cur);
			return 0;
		}
		return strdup(cur);
	}
}

int
main() {
	char c;
	char buf[MACROLEN];

	while ((c=getchar()) != EOF) {
		if (c=='{') {
			match('{', '}', buf);
			process(0, buf);
		} else {
			putchar(c);
		}
	}
}
