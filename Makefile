CC = gcc
CFLAGS = -Wall -o $(target)

target = alist
src = alist.c myopt.c mynetworks.c myhtmlparser.c
header = alist.h myopt.h mynetworks.h myhtmlparser.h macro.h

.PHONY: all dbg
all:
	$(CC) $(CFLAGS) $(src)

dbg:
	$(CC) $(CFLAGS) -DDBGMODE=1 $(src)

$(target): $(src) $(header)
	$(CC) $(CFLAGS) -o $@ $(src)

.PHONY: clean
clean:
	rm -rf $(target)

.PHONY: run run-keio run-sfc run-w3c run-rg
.PHONY: run-keio-i run-sfc-i run-w3c-i run-rg-i
run:
	./$(target) -i

run-keio:
	./$(target) -u www.keio.ac.jp

run-w3c:
	./$(target) -u https://www.w3.org
