LDFLAGS=-g
all : gpm test/inp1
	./gpm < test/inp1 > test/out1
	cmp test/out1 test/exp1
	./gpm < test/inp2 > test/out2
	cmp test/out2 test/exp2
	./gpm < test/inp3 > test/out3
	cmp test/out3 test/exp3
	./gpm < test/inp4 > test/out4
	cmp test/out4 test/exp4
clean :
	rm -f gpm test/out1 test/out2 test/out3 test/out4
