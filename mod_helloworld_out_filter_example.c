#include "httpd.h" 
#include "http_config.h" 
#include "apr_buckets.h" 
#include "util_filter.h" 
#include "apr_strings.h" 


//------- 
static const char module_name[] = "helloworld"; 
module AP_MODULE_DECLARE_DATA myoutfilter_module; 

//------- 
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


//------- 
static apr_status_t out_filter(ap_filter_t *f,apr_bucket_brigade *bb_in){ 
 apr_bucket *bucket_in; 
 apr_bucket_brigade *bb_out; 

 request_rec *r = f->r; 
     conn_rec *c = r->connection; 
  apr_pool_t *p = r->pool; 

 const char *data_in; 
 apr_size_t len_in; 


 int decision = 0; 


 //--- example if we just do nothing 
 //--- do something to set 'decision' to true(1) or false(0) 
 if(decision){ 
  ap_remove_output_filter(f); 
  return ap_pass_brigade(f->next, bb_in); 
 } 

   bb_out = apr_brigade_create(p, c->bucket_alloc); 


 //--- maybe we want write HTML header 
 write_brigade(bb_out,r,"<XMP>"); 
 write_brigade(bb_out,r,"<%START%>\n\n"); 


 while(!APR_BRIGADE_EMPTY(bb_in)){ 

  bucket_in = APR_BRIGADE_FIRST(bb_in); 

  //--- if FLUSH requested ? I don't really know when, just follow other sample. 
  //--- maybe you need it. 
  /* -- 
  if (APR_BUCKET_IS_FLUSH(bucket_in)){ 
   APR_BRIGADE_INSERT_TAIL(bb_out, 
apr_bucket_flush_create(c->bucket_alloc)); 
   if (ap_pass_brigade(f->next, bb_out) != APR_SUCCESS){return APR_SUCCESS;} 
      bb_out = apr_brigade_create(p, c->bucket_alloc); 
   continue; 
      } 
      -- */ 


  if (APR_BUCKET_IS_EOS(bucket_in)){ 
   //--- do we want do something when we know there is no data anymore ? 
         break; 
  } 

  //---- fetch the data 
      apr_bucket_read(bucket_in, &data_in, &len_in, APR_BLOCK_READ); 

       //--- do something with data_in 

   write_brigade(bb_out,r,"<%DATA%>\n"); 
   write_brigade(bb_out,r,apr_pstrdup(p,data_in)); 
   write_brigade(bb_out,r,"\n<%/DATA%>\n "); 

      //--- end fetch 
  apr_bucket_delete(bucket_in); 
 } 

   //-- we don't need bb_in anymore 
   apr_brigade_destroy(bb_in); 


 //--- maybe we want write HTML footer 
 write_brigade(bb_out,r,"\n<%/END%>"); 
 write_brigade(bb_out,r,"</XMP>"); 


 //--- send EOS to bb_out to finish inserting data to bb_out 
 APR_BRIGADE_INSERT_TAIL(bb_out,apr_bucket_eos_create(c->bucket_alloc)); 


 //--- pass bb_out to next filter 
 ap_remove_output_filter(f); 
 return ap_pass_brigade(f->next, bb_out); 
} 

//------- 
static void register_hooks(apr_pool_t *p){ 
    ap_register_output_filter(module_name, out_filter, NULL, 
                              AP_FTYPE_CONTENT_SET); 
} 

//------- 
module AP_MODULE_DECLARE_DATA helloworld_module = { 
    STANDARD20_MODULE_STUFF, 
    NULL,                         /* dir config creater */ 
    NULL,                         /* dir merger --- default is to override */ 
    NULL,                         /* server config */ 
    NULL,                         /* merge server config */ 
    NULL,                         /* command table */ 
    register_hooks                /* register hooks */ 
}; 
