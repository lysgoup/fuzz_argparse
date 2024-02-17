all: 
	clang -fsanitize=fuzzer,address,undefined -g fuzz-main.c ../argparse.c -o fuzz-main

clean:
	rm fuzz-main