#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

/*
 * A dynamic library to allow process-intern network requests from within Vim
 *
 * To use it, set LD_LIBRARY_PATH to include the directory with libvimcurl.so,
 * start Vim and make a request with:
 *
 * :let result=libcall("libvimcurl.so", "make_GET_request", "http://example.com")
 *
 * Build the lib with a simple make (requires libcurl-dev).
 *
 * Philipp Frank <phil.frank@torq.de>
 *
 * Almost everything in this file has been taken from
 * http://curl.haxx.se/lxr/source/docs/examples/getinmemory.c
 */

struct MemoryStruct {
  char *memory;
  size_t size;
};

static void *myrealloc(void *ptr, size_t size);

static void *myrealloc(void *ptr, size_t size)
{
  /* There might be a realloc() out there that doesn't like reallocing
     NULL pointers, so we take care of it here */
  if(ptr)
    return realloc(ptr, size);
  else
    return malloc(size);
}

static size_t
WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)data;

  mem->memory = myrealloc(mem->memory, mem->size + realsize + 1);
  if (mem->memory) {
    memcpy(&(mem->memory[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
  }
  return realsize;
}

char* make_GET_request(const char* request)
{
  CURL *curl_handle;

  struct MemoryStruct chunk;

  chunk.memory = NULL; /* we expect realloc(NULL, size) to work */
  chunk.size   = 0;    /* no data at this point */

  curl_global_init(CURL_GLOBAL_ALL);

  if (NULL == curl_handle)
  {
    return strdup("ERROR INITIALIZING REQUEST");
  }

  /* init the curl session */
  curl_handle = curl_easy_init();

  /* specify URL to get */
  curl_easy_setopt(curl_handle, CURLOPT_URL, request);

  /* follow redirects */
  curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);

  /* specify a timeout of 30s */
  curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 30);

  /* send all data to this function  */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "vimcurl/1.0");

  /* get it! */
  curl_easy_perform(curl_handle);

  /* cleanup curl stuff */
  curl_easy_cleanup(curl_handle);

  /* we're done with libcurl, so clean it up */
  curl_global_cleanup();

  return chunk.memory;
}

