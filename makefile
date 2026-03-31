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
	./gpm < test/inp5 > test/out5
	cmp test/out5 test/exp5
	./gpm < test/inp6 > test/out6
	cmp test/out6 test/exp6
	./gpm < test/inp7 > test/out7
	cmp test/out7 test/exp7
	./gpm < test/inp8 > test/out8
	cmp test/out8 test/exp8
	./gpm < test/inp9 > test/out9
	cmp test/out9 test/exp9
	./gpm < test/html.in > test/html.out
	cmp test/html.out test/html.exp
	./gpm < test/forth.in > test/forth.out
	cmp test/forth.out test/forth.exp

clean :
	rm -f gpm test/out1 test/out2 test/out3 test/out4 test/out5 test/out6 test/out7 test/out8 test/out9 test/html.out test/forth.out
