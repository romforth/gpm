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

int
main() {
	char c;
	char buf[100];

	while ((c=getchar()) != EOF) {
		if (c=='{') {
			match('}', buf);
			if (strncmp(buf, "def ", 4)) {
				char *body;
				if (body=defs[buf[0]-'A']) {
					printf("%s",body);
				} else {
					printf("%s",buf);
				}
			} else {
				defs[buf[4]-'A']=strdup(buf+6);
			}
		} else {
			putchar(c);
		}
	}
}
