/* The simplest HelloWorld module */
#include <httpd.h>
#include <http_protocol.h>
#include <http_config.h>
#include <util_filter.h>
#include <stdio.h>
#include <stdlib.h>
#include <my_global.h>
#include <mysql.h>

static void log_request(request_rec *r) {
	FILE * f;
    f = fopen ("/licenta/test.txt","w");
    if (f!=NULL)
        {
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

static int test_filter_init(ap_filter_t* f) {
  /*txt_ctxt* ctxt = f->ctx = apr_palloc(f->r->pool, sizeof(txt_ctxt)) ;
  txt_dir_cfg* conf = ap_get_module_config(f->r->per_dir_config, &txt_module) ;

  ctxt->head = txt_file_bucket(f->r, conf->header) ;
  ctxt->foot = txt_file_bucket(f->r, conf->footer) ;*/
  return OK ;
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


static int test_filter(ap_filter_t* f, apr_bucket_brigade* bb_in) {
	apr_bucket* b ;
	apr_bucket_brigade *bb_out; 
	
	request_rec *r = f->r; 
	conn_rec *c = r->connection; 
	apr_pool_t *p = r->pool;
	
	
	
	bb_out = apr_brigade_create(p, c->bucket_alloc);
	//write_brigade(bb_out,r,"XMP"); 
	
	int ok = 0;
	
	char *data = "hey";
	char *data2 = "heyend";
	for ( b = APR_BRIGADE_FIRST(bb_in); b != APR_BRIGADE_SENTINEL(bb_in); b = APR_BUCKET_NEXT(b) ) 
		{
		const char* buf ;
		size_t bytes ;
		if ( APR_BUCKET_IS_EOS(b) ) {
			/*APR_BUCKET_INSERT_BEFORE(b, apr_bucket_pool_create( 
			   data2, 
			   strlen(data2), 
			   f->r->pool, 
			   f->r->connection->bucket_alloc 
			));*/
			}
			else 
			{
			apr_bucket_read(b, &buf, &bytes, APR_BLOCK_READ);
			char * pch;
			pch = strstr(buf,"daniel");
			int len = pch-buf;
			
			char content[1000];
			content[0]=0;
			
			strncat(content,buf,len);
			strcat(content,"DANIEL");
			strcat(content,pch+6);
			
			write_brigade(bb_out,r,content); 
			
			ok=1;
			
			
			/*APR_BUCKET_INSERT_BEFORE(b, apr_bucket_pool_create(
				data,
				strlen(data),
				f->r->pool,
				f->r->connection->bucket_alloc
			));*/
			}
		}
	APR_BRIGADE_INSERT_TAIL(bb_out,apr_bucket_eos_create(c->bucket_alloc)); 
	ap_remove_output_filter(f); 
 
	if (ok==0)
		return ap_pass_brigade(f->next, bb_in) ;
		else
		return ap_pass_brigade(f->next, bb_out) ;
}	

static void helloworld_hooks(apr_pool_t *pool) {
    //ap_hook_handler(helloworld_handler, NULL, NULL, APR_HOOK_LAST);
	ap_register_output_filter("helloworld", test_filter, NULL, AP_FTYPE_RESOURCE) ;
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