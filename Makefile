all:
	c++ -std=c++1y -fpic -shared -Ilib/juble/include -I/usr/include/ruby-2.1.0 -I/usr/include/ruby-2.1.0/x86_64-linux -o color_coded.so color_coded.cpp

run:
	#gvim -c ":rubyfile ${PWD}/color_coded.rb"
	gvim -c ":source ${PWD}/color_coded.vim"
