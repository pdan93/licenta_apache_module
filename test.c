#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>


int main() {
	
	
	
	regex_t regex;
	int reti;
	char ss[1000];
	regmatch_t matches[10];
	scanf("%s",ss);
	reti = regcomp(&regex, ss, REG_EXTENDED);
	reti = regexec(&regex, "email=x' OR 1=1&password=dssds", 10, matches, 0);
	if (!reti)
		printf("ok\n");
		else if (reti == REG_NOMATCH) 
		printf("not ok\n");	
		else
		printf("regex failed\n");
	printf("%d\n",matches[0].rm_so);
	printf("%d\n",matches[0].rm_eo);
	printf("%d\n",matches[1].rm_so);

	return 1;
}
