/* The simplest HelloWorld module */
#include <httpd.h>
#include <http_protocol.h>
#include <http_config.h>
#include <util_filter.h>
#include <stdio.h>
#include <stdlib.h>
#include <my_global.h>
#include <mysql.h>

FILE * content_file;
FILE * content_file2;
int content_file_ok = 0;
int content_file_ok2 = 0;

static void log_request(request_rec *r) {
	FILE * f;
    f = fopen ("/licenta/request.txt","w");
    if (f!=NULL)
        {
		fprintf(f, "Request protocol: %s\n",r->protocol);
		fprintf(f, "Request hostname: %s\n",r->hostname);
		fprintf(f, "Request status_line: %s\n",r->status_line);
		fprintf(f, "Request method: %s\n",r->method);
		fprintf(f, "Request range: %s\n",r->range);
		fprintf(f, "Request content_type: %s\n",r->content_type);
		fprintf(f, "Request handler: %s\n",r->handler);
		fprintf(f, "Request content_encoding: %s\n",r->content_encoding);
		fprintf(f, "Request user: %s\n",r->user);
		fprintf(f, "Request ap_auth_type: %s\n",r->ap_auth_type);
		fprintf(f, "Request unparsed_uri: %s\n",r->unparsed_uri);
		fprintf(f, "Request uri: %s\n",r->uri);
		fprintf(f, "Request filename: %s\n",r->filename);
		fprintf(f, "Request canonical_filename: %s\n",r->canonical_filename);
		fprintf(f, "Request path_info: %s\n",r->path_info);
		fprintf(f, "Request args: %s\n",r->args);
		fprintf(f, "Request log_id: %s\n",r->log_id);
		fprintf(f, "Request useragent_ip: %s\n",r->useragent_ip);
		fprintf(f, "Request time (time when request started): %d\n",r->request_time);
		fprintf(f, "Request allowed(y/n): %d\n",r->allowed);
		fprintf(f, "Request resource last modified time: %d\n",r->mtime);
		
		
			
			apr_sockaddr_t* useragent_addr= r->useragent_addr;
			fprintf(f, "\tRequest UseragentAddr hostname: %s\n",useragent_addr->hostname);
			fprintf(f, "\tRequest UseragentAddr servname: %s\n",useragent_addr->servname);
			fprintf(f, "\tRequest UseragentAddr port: %d\n",useragent_addr->port);
			fprintf(f, "\tRequest UseragentAddr ipaddr_len: %d\n",useragent_addr->ipaddr_len);
			fprintf(f, "\tRequest UseragentAddr addr_str_len: %d\n",useragent_addr->addr_str_len);
			fprintf(f, "\tRequest UseragentAddr family: %d\n",useragent_addr->family);
			//fprintf(f, "\tRequest UseragentAddr ip: %s\n",useragent_addr->next->hostname);
			
			apr_uri_t parsed_uri = r->parsed_uri;
			fprintf(f, "\tRequest ParsedURI scheme: %s\n",parsed_uri->user);
		
		//fprintf(f, "Requesr useragent_host: %s\n",r->useragent_host);
			server_rec * server= r->server;
			fprintf(f, "\tServer defn_name: %s\n",server->defn_name);
			fprintf(f, "\tServer server_scheme: %s\n",server->server_scheme);
			fprintf(f, "\tServer server_admin: %s\n",server->server_admin);
			fprintf(f, "\tServer server_hostname: %s\n",server->server_hostname);
			fprintf(f, "\tServer path: %s\n",server->path);
			process_rec * process= server->process;
			fprintf(f, "\t\tServer Process short_name: %s\n",process->short_name);
			fprintf(f, "\t\tServer Process argv: %s\n",process->argv);
			//apr_file_t * error_log= server->error_log;
			//fprintf(f, "\t\tServer ErrorLog fname: %s\n",error_log->fname);
			apr_port_t * port= server->port;
			fprintf(f, "\tServer Port: %d\n",port);
			fprintf(f, "\tServer Timeout(ms): %d\n",server->timeout);
			fprintf(f, "\tServer Keep Alive Timeout(ms): %d\n",server->keep_alive_timeout);
			fprintf(f, "\tServer Keep Alive Max: %d\n",server->keep_alive_max);
			fprintf(f, "\tServer Keep Alive(yes/no): %d\n",server->keep_alive);
			fprintf(f, "\tServer limit on size of the HTTP request line: %d\n",server->limit_req_line);
			fprintf(f, "\tServer limit on size of any request header field: %d\n",server->limit_req_fieldsize);
			fprintf(f, "\tServer limit on number of request header fields: %d\n",server->limit_req_fields);
		
			
		/*
        fputs("Protocol: ", f);
        fputs(r->protocol, f);
        fputs("\nThe Request: ", f);
        fputs(r->the_request, f);
        fputs("\nHostname: ", f);
        fputs(r->hostname, f);
        //fputs("\nStatus Line: ", f);
        //fputs(r->status_line, f);
        fputs("\nMethod: ", f);
        fputs(r->method, f);
        //fputs("\nRange: ", f);
        //fputs(r->range, f);
        fputs("\nCOntent Type: ", f);
        fputs(r->content_type, f);
        fputs("\nHandler: ", f);
        fputs(r->handler, f);
        //fputs("\nContent encoding: ", f);
        //fputs(r->content_encoding, f);
        //fputs("\n Vlist Validator: ", f);
        //fputs(r->vlist_validator, f);
        //fputs("\n User: ", f);
        //fputs(r->user, f);
        //fputs("\n Ap Auth Type: ", f);
        //fputs(r->ap_auth_type, f);
        fputs("\n Unparsed Uri: ", f);
        fputs(r->unparsed_uri, f);
        fputs("\n Uri: ", f);
        fputs(r->uri, f);
        fputs("\n Filename: ", f);
        fputs(r->filename, f);
        fputs("\n Canonical Filename: ", f);
        fputs(r->canonical_filename, f);
        fputs("\n Path info: ", f);
        fputs(r->path_info, f);
        //fputs("\n Args: ", f);
        //fputs(r->args, f);
        //fputs("\n Log Id: ", f);
        //fputs(r->log_id, f);
        fputs("\n UserAgent IP: ", f);
        fputs(r->useragent_ip, f);
        */
        
        
        
        fclose(f);
        }
        
}


static void mysql_test(request_rec *r) {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *server = "localhost";
	char *user = "test";
	char *password = "test"; /* set me first */
	char *database = "test";
	conn = mysql_init(NULL);
	/* Connect to database */
	if (!mysql_real_connect(conn, server,
		 user, password, database, 0, NULL, 0)) {
	  ap_rprintf(r, "%s\n", mysql_error(conn));
	  exit(1);
	}
	/* send SQL query */
	if (mysql_query(conn, "show tables")) {
	  ap_rprintf(r, "%s\n", mysql_error(conn));
	  exit(1);
	}
	res = mysql_use_result(conn);
	/* output table name */
	ap_rprintf(r, "MySQL Tables in mysql database:\n");
	while ((row = mysql_fetch_row(res)) != NULL)
	  ap_rprintf(r, "%s \n", row[0]);
	/* close connection */
	mysql_free_result(res);
	mysql_close(conn);
}

static int helloworld_handler(request_rec *r) {
    /*if (!r->handler || strcmp(r->handler, "helloworld")) {
        return DECLINED;
    }
    if (r->method_number != M_GET) {
        return HTTP_METHOD_NOT_ALLOWED;
    }
    ap_set_content_type(r, "text/html;charset=ascii");
    ap_rputs("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\">\n", r);
    ap_rputs("<html><heade><title> Apache HelloWorld Module</title></head>", r);
    ap_rputs(mysql_get_client_info(), r);
    mysql_test(r);
    
    ap_rputs("</body></html>", r);
    
	log_request(r);*/
	ap_rputs("hey",r);
	
    return OK;
}


static void log_bucket(apr_bucket *b) {
	FILE * f;
    f = fopen ("/licenta/bucket.txt","w");
    if (f!=NULL)
        {
			
        const char* buf ;
		size_t bytes ;
        apr_bucket_read(b, &buf, &bytes, APR_BLOCK_READ);
        
        fputs(buf, f);
        
        
        fclose(f);
        }
        
}

static void log_text(char *s) {
	FILE * f;
    f = fopen ("/licenta/text.txt","w");
    if (f!=NULL)
        {
        fputs(s, f);
        fclose(f);
        }
        
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

	log_text(buf);
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

void read_in_buffer(char * filepath, char ** buffer ) {
	FILE * content_file2;
	content_file2 = fopen (filepath,"r");
	
  fseek (content_file2 , 0 , SEEK_END);
  long lSize = ftell (content_file2);
  rewind (content_file2);

  // allocate memory to contain the whole file:
  *buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  // copy the file into the buffer:
  size_t result = fread (*buffer,1,lSize,content_file2);
  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}	  
}

char *str_replace(char *orig, char *rep, char *with) {
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
    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

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
	free(orig_copy);
    return result;
}

static int output_filter_init(ap_filter_t* f) {
  
	content_file = fopen ("/licenta/output_filter.txt","w");
	fprintf(content_file,"Start3\n");

	return OK ;
}

static int output_filter(ap_filter_t* f, apr_bucket_brigade* bb_in) {
	apr_bucket* b ;
	apr_bucket_brigade *bb_out; 
	
	request_rec *r = f->r; 
	conn_rec *c = r->connection; 
	apr_pool_t *p = r->pool;
	
	
	
	bb_out = apr_brigade_create(p, c->bucket_alloc);
	write_brigade(bb_out,r,""); 
	
	int ok = 0;
	
	char *data = "hey";
	char *data2 = "heyend";
	
    
	/*fprintf(fortest,"what we get: %d\n",strstr(r->the_request,".php"));
	int length;
	apr_brigade_length(bb_in,1,&length);
	fprintf(fortest,"Brigade length %d\n",length);*/
	
	
	for ( b = APR_BRIGADE_FIRST(bb_in); b != APR_BRIGADE_SENTINEL(bb_in); b = APR_BUCKET_NEXT(b) ) 
		{
		char* buf ;
		size_t bytes ;
		if ( APR_BUCKET_IS_EOS(b) ) {
			
			fprintf(content_file,"END\n");
			fclose(content_file);
			content_file_ok = 1;
			
			//ap_add_output_filter("last_filter",)
			
			
			read_in_buffer("/licenta/output_filter.txt",&buf);
			
			//buf = str_replace(buf,"99","yy");
			
			write_brigade(bb_out,r,buf); 
			
			write_brigade(bb_out,r,"y"); 
			APR_BRIGADE_INSERT_TAIL(bb_out,apr_bucket_eos_create(c->bucket_alloc)); 
			}
			else 
			{
			apr_bucket_read(b, &buf, &bytes, APR_BLOCK_READ);
			buf[bytes]=0;
			fprintf(content_file,"%s\n",buf);
			//log_bucket(b);
			/*char * pch;
			pch = strstr(buf,"daniel");
			int len = pch-buf;
			
			char content[10000];
			content[0]=0;
			
			strncat(content,buf,len);
			strcat(content,"DANIEL");
			strcat(content,pch+6);
			
			
			
			write_brigade(bb_out,r,content); 
			
			
			write_brigade(bb_out,r,buf);*/
			
			//ok=0;
			
			
			}
		}
	//APR_BRIGADE_INSERT_TAIL(bb_in,apr_bucket_eos_create(c->bucket_alloc)); 
	//ap_remove_output_filter(f); 
	
 
	//if (ok==0)
	//	return ap_pass_brigade(f->next, bb_in) ;
	//	else
		return ap_pass_brigade(f->next, bb_out) ;
}	


static int input_filter_init(ap_filter_t* f) {
	log_request(f->r);
	
	content_file2 = fopen ("/licenta/input_filter.txt","a+");
	fprintf(content_file2,"Start3\n");
	content_file_ok2 = 0;

	return OK ;
}

int input_filter(ap_filter_t* f, apr_bucket_brigade *bb, ap_input_mode_t mode, apr_read_type_e block, apr_off_t readbytes) {
	int rv;
	apr_bucket* b ;
	rv = ap_get_brigade(f->next, bb, mode, block, readbytes);
	
	for ( b = APR_BRIGADE_FIRST(bb); b != APR_BRIGADE_SENTINEL(bb); b = APR_BUCKET_NEXT(b) ) 
		{
		
		char* buf ;
		size_t bytes ;
		if ( APR_BUCKET_IS_EOS(b) ) {
			if (content_file_ok2==0)
				{
				fprintf(content_file2,"IN3\n");
				fclose(content_file2);
				content_file_ok2=1;
				}
			}
			else 
			{
			apr_bucket_read(b, &buf, &bytes, APR_BLOCK_READ);
			buf[bytes]=0;
			if (content_file_ok2==1)
				{
				content_file2 = fopen ("/licenta/input_filter.txt","a+");
				content_file_ok2 = 0;
				}
				
			fprintf(content_file2,"%s\n",buf);
			}
		}
	return rv;
	
}

static void helloworld_hooks(apr_pool_t *pool) {
    //ap_hook_handler(helloworld_handler, NULL, NULL, APR_HOOK_LAST);
	//ap_register_output_filter("helloworld", last_filter, NULL, AP_FTYPE_RESOURCE) ;
	ap_register_output_filter("helloworld", output_filter, output_filter_init, AP_FTYPE_RESOURCE);
	ap_register_input_filter("elixir_output_filter", input_filter, input_filter_init, AP_FTYPE_CONTENT_SET);//AP_FTYPE_CONNECTION);
}


module AP_MODULE_DECLARE_DATA helloworld_module = {
  STANDARD20_MODULE_STUFF,
  NULL,  
  NULL,
  NULL,
  NULL,
  NULL,
  helloworld_hooks
};