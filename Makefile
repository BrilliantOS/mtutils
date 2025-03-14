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

md: out
	gcc md/main.c -o out/md
	ln -s out/md mkdir

rd: out
	gcc rd/main.c -o out/rd
	ln -s out/rd rmdir

flsplit: out
	gcc flsplit/main.c -o out/flsplit

flunsplit: out
	gcc flunsplit/main.c -o out/flunsplit

all: len ls md rd flsplit flunsplit
