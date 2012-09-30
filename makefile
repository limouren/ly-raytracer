# Compiler
CC = 'g++'

# Boost library directory
BOOST_DIR = '../3rdparty/boost/latest/'


all :

.PHONY : clean
clean :
	rm -rf bin/*
