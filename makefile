cookieshell: common.c main.c prompt.c warp.c proclore.c peek.c fg.c bg.c syscom.c pastevents.c seek.c activities.c iman.c fgbg.c signals.c
	gcc -g common.c main.c prompt.c warp.c proclore.c peek.c fg.c bg.c syscom.c pastevents.c seek.c activities.c iman.c fgbg.c signals.c -o cookieshell -lm