source ~/.vimrc

nnoremap <leader>fi :YcmCompleter FitIt<CR>
nnoremap <leader>gt :YcmCompleter GetType<CR>

set path+=include
set path+=lib/jest/include
set path+=/usr/include/c++/9
set path+=/usr/lib/llvm-10/include
