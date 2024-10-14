sim: common.c cache.c sim.c ll.c pt.c tlb.c
	gcc -o sim sim.c cache.c common.c ll.c pt.c tlb.c -I./include -Wall -Werror -lm -g -DDEBUG
clean: 
	rm sim
