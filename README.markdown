Overview
========

Provides a library for Vim's libcall-function to make requests (over HTTP or
otherwise) from within a Vim process (this should be a bit faster than using
curl or wget as external processes).

Platforms
=========

Vim's libcall-functionality is available on Windows, but you need to be able to
build the libvimcurl.dll (my Makefile does not do this).

Installation
==============

    # Make sure you have libcurl-dev installed
    make
  
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd):
    # alternatively, you can add something like this to your ~/.vimrc:
    #   let g:vimcurl_lib = "/full/path/to/libvimcurl.so"
  
    # Install the plugin
    cp plugin/vimcurl.vim ~/.vim/plugin


