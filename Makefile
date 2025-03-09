.PHONY: help len ls luna out

help:
	@echo "mtutils -- targets:"
	@echo "- help - lists all targets"
	@echo "- <tool name> - builds the specified tool"

out:
	mkdir -p out

len: len/main.c | out
	gcc len/main.c -o out/len

ls: ls/main.c | out
	gcc ls/main.c -o out/ls

luna: luna/main.c | out
	gcc luna/main.c -o out/luna

all: len ls luna
