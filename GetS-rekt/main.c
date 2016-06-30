#include <stdio.h>
#include <stdlib.h>

int main() {
	const char* const orig = "LOLLOLLOL";
	char buf[20];
	gets(buf);
	printf("%s",buf);
	exit(0);
}
