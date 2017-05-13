
rm:=del
main.so:astparser.o automaton.o nfagraph.o regexresult.o regexsearch.o stringutilities.o TinyRegex.o
	g++ -shared astparser.o automaton.o nfagraph.o regexresult.o regexsearch.o stringutilities.o TinyRegex.o -o main.so

astparser.o:astparser.cpp astparser.h base.h astnode.h regexerror.h nfagraph.h automaton.h \
regexresult.h attribute.h regexerror.h stringutilities.h
	gcc -c -fpic -std=c++11 astparser.cpp

automaton.o:automaton.cpp automaton.h
	gcc -c -fpic -std=c++11 automaton.cpp

nfagraph.o:nfagraph.cpp nfagraph.h automaton.h regexresult.h attribute.h regexerror.h stringutilities.h
	gcc -c -fpic -std=c++11 nfagraph.cpp

regexresult.o:regexresult.cpp regexresult.h base.h
	gcc -c -fpic -std=c++11 regexresult.cpp

regexsearch.o:regexsearch.cpp regexsearch.h astparser.h base.h astnode.h regexerror.h nfagraph.h automaton.h \
regexresult.h attribute.h regexerror.h stringutilities.h
	gcc -c -fpic -std=c++11 regexsearch.cpp

stringutilities.o:stringutilities.cpp stringutilities.h base.h
	gcc -c -fpic -std=c++11 stringutilities.cpp

TinyRegex.o:TinyRegex.cpp TinyRegex.h regexsearch.cpp regexsearch.h astparser.h base.h astnode.h regexerror.h nfagraph.h automaton.h \
regexresult.h attribute.h regexerror.h stringutilities.h
	gcc -c -fpic -std=c++11 TinyRegex.cpp

clean:
	$(rm) *.o;
	$(rm) *.so
