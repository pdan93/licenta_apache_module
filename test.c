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

int categorize_sql_injection(char *s) { //1 - tautologies, 2 - illegal/logically incorect queries, 3- union query, 4- piggy backed query, 5-stored procedures, 6- alternate encodings
	FILE * s_patterns = fopen("/licenta/sql_injection_patterns","r");
	char c;
	char pattern[1000];
	while ((c = fgetc(s_patterns)) != EOF) {
		fgetc(s_patterns);
		fgets(pattern, 1000, s_patterns);
		pattern[strlen(pattern)-1]=0;
		if (my_regex(pattern,s))
			{
			fclose(s_patterns);
			return c-'0';
			}
	}
	fclose(s_patterns);
	return -1;
	/*if (my_regex("^[a-z0-9]+$",s))
		return 0;
	if (my_regex("^[a-z0-9]*[';\"]$",s))//just probing (x' x" ' " ;)
		return 1;
	if (my_regex("^[a-z0-9]*['\"] (OR)|(or) .*=[^;]*$",s))//completion (*' OR *=*)
		return 2;
	if (my_regex("^.*; [A-Za-z]+ .*$",s))//instruction
		return 3;
	return -1;*/
}

int main() {
	
	char * test;
	test = (char *) malloc(2*1000);
	scanf("%80[^\n]",test);
	printf("%d\n",categorize_sql_injection(test));
	
	//system("mysqldump -h localhost -u root -p'Dan230793' test | mysql -h localhost -u test3user -p'test3pass' elixir_fashion3");
	/*
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
*/
	return 1;
}
