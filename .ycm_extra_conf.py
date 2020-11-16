import os
import ycm_core

def Settings(**kwargs):
    return { 'flags': [
        '-x', 'c++',
        '-std=c++17',
        '-Wall',
        '-Wextra',
        '-Werror',
        '-I', 'include',
        '-isystem', 'lib/jest/include',
        '-isystem', '/usr/include/c++/9',
        '-isystem', '/usr/lib/llvm-10/include',
        '-isystem', '/usr/include/lua5.2',
    ] }
