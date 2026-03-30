#include <stdio.h>	// getchar, putchar, EOF
#include <string.h>	// strcmp, strdup

void
match(char close, char *s) {
	char c;

	while ((c=getchar()) != EOF) {
		if (c==close) {
			*s++=0;
			return;
		}
		*s++=c;
	}
}

void
subst(char *p, int arg, char **args) {
	char c;

	while (c=*p++) {
		if (c=='$') {
			int n=*p++-'0';
			subst(args[n], arg, args);
		} else {
			putchar(c);
		}
	}
}

int
split(char *buf, int arg, char **args) {
	char c;

	args[arg++]=buf;
	while (c=*buf) {
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

void
expand(char *buf) {
	char *args[100];

	int arg=split(buf, 0, args);

	if (strcmp(args[0], "def")) {
		char *body;
		if (body=find(args[0])) {
			subst(body, arg, args);
		} else {
			for (int i=0;i<arg;i++) {
				subst(args[i], arg, args);
				if (i!=arg-1) putchar(' ');
			}
		}
	} else {
		macros[nmac].name=strdup(args[1]);
		macros[nmac].body=strdup(args[2]);
		nmac++;
	}
}

int
main() {
	char c;
	char buf[100];

	while ((c=getchar()) != EOF) {
		if (c=='{') {
			match('}', buf);
			expand(buf);
		} else {
			putchar(c);
		}
	}
}
