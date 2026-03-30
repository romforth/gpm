all : gpm test/inp1
	./gpm < test/inp1 > test/out1
	cmp test/out1 test/exp1
clean :
	rm -f gpm test/out1
