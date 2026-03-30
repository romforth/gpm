LDFLAGS=-g
all : gpm test/inp1
	./gpm < test/inp1 > test/out1
	cmp test/out1 test/exp1
	./gpm < test/inp2 > test/out2
	cmp test/out2 test/exp2
clean :
	rm -f gpm test/out1 test/out2
