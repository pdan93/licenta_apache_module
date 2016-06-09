#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "config.h"
#include "ldns/ldns.h"

int main() {
	
	
  ldns_resolver *res;
        ldns_rdf *domain;
        ldns_pkt *p;
        ldns_rr_list *mx;
        ldns_status s;
	domain = ldns_dname_new_frm_str("wjrkudvgxlao.93.122.136.89.dnsbl.httpbl.org");
	 s = ldns_resolver_new_frm_file(&res, NULL);
//wjrkudvgxlao.93.122.136.89.dnsbl.httpbl.org
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
