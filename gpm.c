#include <stdio.h>	// getchar, putchar, EOF

void
match(char close) {
	char c;

	while ((c=getchar()) != EOF) {
		if (c==close) return;
		putchar(c);
	}
}

int
main() {
	char c;

	while ((c=getchar()) != EOF) {
		if (c=='{') {
			match('}');
		} else {
			putchar(c);
		}
	}
}
