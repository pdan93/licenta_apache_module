#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

int my_regex(char * b1, char * b2) {
	regex_t regex;
	int reti;
	reti = regcomp(&regex, b1, REG_EXTENDED);
	reti = regexec(&regex, b2, 0, NULL, 0);
	if (!reti)
		return 1;
		else if (reti == REG_NOMATCH) 
		return 0;
		else
		return 0;
}

int main() {
	
	
  
	regex_t regex;
	int reti;
	char ss[1000];
	regmatch_t matches[10];
	scanf("%s",ss);
	printf("%d\n",my_regex("([.]{2}/)",ss));

	return 1;
}
