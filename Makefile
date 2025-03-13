.PHONY: help len ls luna out

help:
	@echo "mtutils -- targets:"
	@echo "- help - lists all targets"
	@echo "- <tool name> - builds the specified tool"

out:
	mkdir -p out

len: out
	gcc len/main.c -o out/len

ls: out
	gcc ls/main.c -o out/ls

luna: out
	gcc luna/main.c -o out/luna

md: out
	gcc md/main.c -o out/md
	ln -s out/md out/mkdir

rd: out
	gcc rd/main.c -o out/rd
	ln -s out/rd out/rmdir

all: len ls luna md rd
