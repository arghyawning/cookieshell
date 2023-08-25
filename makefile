cookieshell: common.c main.c prompt.c warp.c proclore.c peek.c fg.c bg.c syscom.c
	gcc -g common.c main.c prompt.c warp.c proclore.c peek.c fg.c bg.c syscom.c -o cookieshell -lm