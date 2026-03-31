#include <stdio.h>	// getchar, putchar, EOF
#include <string.h>	// strcmp, strdup

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

	while (c=*src++) {
		if (c=='$') {
			int n=*src++-'0';
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

void
expand(char *src, char *dst) {
	char *args[100];

	int arg=split(src, 0, args);

	if (strcmp(args[0], "def")) {
		char *body;
		if (body=find(args[0])) {
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
}

#define MACROLEN 10000

int
main() {
	char c;
	char buf[100];
	char dst[MACROLEN];

	while ((c=getchar()) != EOF) {
		if (c=='{') {
			match('{', '}', buf);
			*dst=0;
			expand(buf, dst);
			printf("%s", dst);
		} else {
			putchar(c);
		}
	}
}
