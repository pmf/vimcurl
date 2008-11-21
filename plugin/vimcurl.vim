" vimcurl.vim -- Make network requests from within Vim
" @Author:      Philipp Frank (phil.frank AT torq de)
" @License:     BSD
" @Created:     2008-11-20.
" @Last Change: 2008-11-20.
" @Revision:    0.1
" 

if &cp || exists('loaded_vimcurl')
    finish
endif
let loaded_vimcurl = 1

if !exists('g:vimcurl_lib')
    let g:vimcurl_lib = "libvimcurl.so"
endif

function! Vimcurl(url)
    return libcall(g:vimcurl_lib, "make_GET_request", a:url)
endfunction

