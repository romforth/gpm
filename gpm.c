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

char *defs[26];

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

void
expand(char *buf) {
	char *args[100];

	int arg=split(buf, 0, args);

	if (strcmp(args[0], "def")) {
		char *body;
		if (body=defs[*args[0]-'A']) {
			subst(body, arg, args);
		} else {
			for (int i=0;i<arg;i++) {
				subst(args[i], arg, args);
				if (i!=arg-1) putchar(' ');
			}
		}
	} else {
		defs[*args[1]-'A']=strdup(args[2]);
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
