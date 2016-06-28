
char * printtable_file;

struct post_body {
	char keys[10][100];
	char values[10][100];
	int nr;
};
struct MCookie {
	char * key;
	char * value;
};
static int printitem(void* rec, const char* key, const char* value) { //MODIFIED FROM APACHE MODULES BOOK 131
  /* rec is a userdata pointer.  We'll pass the request_rec in it */
  request_rec* r = rec ;
  sprintf(printtable_file + strlen(printtable_file), "%s: %s\n", ap_escape_html(r->pool, key), ap_escape_html(r->pool, value)) ;
  /* Zero would stop iterating; any other return value continues */
  return 1 ;
}
static void printtable(request_rec* r, apr_table_t* t, char * file) {//MODIFIED FROM APACHE MODULES BOOK 131
  /* Print the data: apr_table_do iterates over entries with our callback */
  printtable_file = file;
  apr_table_do(printitem, r, t, NULL) ;
}


void read_in_buffer(char * filepath, char ** buffer, request_rec * r ) {
	FILE * content_file2;
	content_file2 = fopen (filepath,"r");
	
  fseek (content_file2 , 0 , SEEK_END);
  long lSize = ftell (content_file2);
  rewind (content_file2);

  // allocate memory to contain the whole file:
  *buffer = (char*) apr_pcalloc(r->pool,sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  // copy the file into the buffer:
  size_t result = fread (*buffer,1,lSize,content_file2);
  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}	 
fclose(content_file2);  
}
int strpos(char *haystack, char *needle)
{
   char *p = strstr(haystack, needle);
   if (p)
      return p - haystack;
   return -1;
}

char *str_replace(char *orig, char *rep, char *with, request_rec * r) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
	char *orig_copy;
    int len_rep;  // length of rep
    int len_with; // length of with
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements
	orig_copy = orig;
    if (!orig)
        return NULL;
    if (!rep)
        rep = "";
    len_rep = strlen(rep);
    if (!with)
        with = "";
    len_with = strlen(with);

    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    tmp = result = apr_pcalloc(r->pool,2*(strlen(orig) + (len_with - len_rep) * count + 1));

    if (!result)
        return NULL;

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
	//free(orig_copy);
    return result;
}

char * replace_from_till(char * from, char * till, char * text, char * with, request_rec* r) {
	char *result; // the return string
	int pos_from = strpos(text,from);
	pos_from += strlen(from);
	int pos_to = strpos(text+pos_from,till)+pos_from;
	printf("%d\n",pos_from);
	printf("%d\n",pos_to);
	//char * search = (char*) malloc (sizeof(char)*(pos_to-pos_from));
	//strncat(search,text+pos_from,(pos_to-pos_from));
	result = (char*) apr_pcalloc(r->pool,sizeof(char)*(strlen(text)-(pos_to-pos_from)+strlen(with)));
	strncat(result,text,pos_from);
	strcat(result,with);
	strcat(result,text+pos_to);
	return result;
}


void static run_php_script() {
	char buf[1000];
	FILE *fp;
	char path[1035];

	/* Open the command for reading. */
	fp = popen("php -f /licenta/test.php", "r");
	if (fp == NULL) {
	exit(1);
	}
	buf[0]=0;
	/* Read the output a line at a time - output it. */
	while (fgets(path, sizeof(path)-1, fp) != NULL) {
		strcat(buf,path);
	}

	//log_text(buf);
	/* close */
	pclose(fp);
}


void write_brigade(apr_bucket_brigade *bb_out,request_rec *r,char *data){ 
 APR_BRIGADE_INSERT_TAIL( 
  bb_out, 
  apr_bucket_pool_create( 
   data, 
   strlen(data), 
   r->pool, 
   r->connection->bucket_alloc 
  ) 
 ); 
} 

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

char *urlDecode(const char *str, request_rec* r) {
  int d = 0; /* whether or not the string is decoded */

  char *dStr = apr_pcalloc(r->pool,strlen(str) + 1);
  char eStr[] = "00"; /* for a hex code */

  strcpy(dStr, str);

  while(!d) {
    d = 1;
    int i; /* the counter for the string */

    for(i=0;i<strlen(dStr);++i) {

      if(dStr[i] == '%') {
        if(dStr[i+1] == 0)
          return dStr;

        if(isxdigit(dStr[i+1]) && isxdigit(dStr[i+2])) {

          d = 0;

          /* combine the next to numbers into one */
          eStr[0] = dStr[i+1];
          eStr[1] = dStr[i+2];

          /* convert it to decimal */
          long int x = strtol(eStr, NULL, 16);

          /* remove the hex */
          memmove(&dStr[i+1], &dStr[i+3], strlen(&dStr[i+3])+1);

          dStr[i] = x;
        }
      }
    }
  }
  int j;
  for (j=0; j<strlen(dStr); j++)
	  if (dStr[j]=='+')
		  dStr[j]=' ';

  return dStr;
}


struct post_body break_post_body(char * s) {
	//urlDecode(s);
	struct post_body pb;
	int i,j,lg,nr;
	if (my_regex("^(.+=.+)+(&.+=.*)*",s))//then it's correct type
		{
		i=0;
		nr = 0;
		while (1)
			{		
			lg = 0;
			for (j=i; j<strlen(s); j++)
				{
				if (s[j]!='=')
					{
					pb.keys[nr][lg]=s[j];
					lg++;
					}
					else
					{
					pb.keys[nr][lg]=0;
					break;
					}
				}	
			lg = 0;
			for (j=j+1; j<strlen(s); j++)
				{
				if (s[j]!='&')
					{
					pb.values[nr][lg]=s[j];
					lg++;
					}
					else
					{
					pb.values[nr][lg]=0;
					break;
					}
				}
			if (j==strlen(s))
				pb.values[nr][lg]=0;
			i=j+1;
			nr++;
			if (i>=strlen(s))
				break;
			}
		pb.nr = nr;
		
		}
	return pb;
		
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
}
int categorize_xss_injection(char *s) { //1 - tautologies, 2 - illegal/logically incorect queries, 3- union query, 4- piggy backed query, 5-stored procedures, 6- alternate encodings
	FILE * s_patterns = fopen("/licenta/xss_injection_patterns","r");
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
}


unsigned int rand_interval(unsigned int min, unsigned int max)
{
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    /* Create equal size buckets all in a row, then fire randomly towards
     * the buckets until you land in one of them. All buckets are equally
     * likely. If you land off the end of the line of buckets, try again. */
    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}

struct MCookie parse_cookie(request_rec*r, char * return_val) {
	struct MCookie rcookie;
	rcookie.key = apr_pcalloc(r->pool, sizeof(char)*strlen(return_val));
		rcookie.value = apr_pcalloc(r->pool, sizeof(char)*strlen(return_val));
		strcat(rcookie.key,return_val);
		rcookie.key[strpos(return_val,"=")]=0;
		strcat(rcookie.value,return_val+strpos(return_val,"=")+1);
		return rcookie;
}
