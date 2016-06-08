#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include <time.h>

int main() {
	
	
  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  //printf ( "Current local time and date: %s", asctime (timeinfo) );
  printf("%d-%d-%d %d:%d:%d", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	
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
