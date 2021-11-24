/*
 *
 * Copyright (C) 2021 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include<time.h>
#include<errno.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdbool.h>
#include<signal.h>
#include<string.h>
#include<unistd.h>
#include<limits.h>
#include<sys/ioctl.h>
#include<linux/input.h>

static int fd=-1;
static unsigned times=0;

/* generate random number */
static int random_int(int max,int min){
	return (rand()%(max-min))+min;
}

/* sleep millisecond */
static void msleep(uint32_t ms){
	usleep(ms*1000);
}

/* search pc speaker */
static int find_snd_input(void){
	int fd=-1;
	bool found=false;
	unsigned char mask[EV_MAX/8+1];
	char buf[PATH_MAX],name[256];
	for(int i=0;i<64;i++){
		memset(buf,0,PATH_MAX);
		snprintf(buf,PATH_MAX-1,"/dev/input/event%d",i);
		if((fd=open(buf,O_WRONLY))<0){
			if(errno!=ENOENT)fprintf(stderr,"open device %s failed: %s\n",buf,strerror(errno));
			continue;
		}
		if(ioctl(fd,EVIOCGBIT(0,sizeof(mask)),mask)<0){
			fprintf(stderr,"failed to %s ioctl EVIOCGBIT: %s\n",buf,strerror(errno));
			goto next;
		}
		for(int j=0;j<EV_MAX;j++)if((mask[j/8]&(1<<(j%8)))&&j==EV_SND){
			found=true;
			break;
		}
		if(!found||ioctl(fd,EVIOCGSND(0))<0)goto next;
		memset(name,0,sizeof(name));
	        if(ioctl(fd,EVIOCGNAME(255),name)<0){
	                fprintf(stderr,"failed to %s ioctl EIOCGNAME: %s\n",buf,strerror(errno));
	                strcpy(name,"unknown");
	        }
	        printf("found sound input device %s (%s)\n",buf,name);
	        return fd;
		next:
		close(fd);
	}
	fprintf(stderr,"no sound input device found\n");
	return -1;
}

/* control speaker */
static ssize_t single_beep(int fd,const uint16_t freq){
	static struct input_event e;
	static size_t s=sizeof(e);
	memset(&e,0,s);
	e.type=EV_SND;
	e.code=SND_TONE;
	e.value=freq;
	return write(fd,&e,s);
}

/* beep once */
static void beep(int fd,const uint16_t freq,const uint32_t ms){
	single_beep(fd,freq);
	msleep(ms);
	single_beep(fd,0);
}

/* cleanup */
static void sig_hand(int sig __attribute__((unused))){
	single_beep(fd,0);
	close(fd);
	exit(0);
}

static int usage(int e){
	fprintf(stderr,"Usage: music [TIME]\n");
	return e;
}

/* setup signals handler */
static void setup_signals(void){
	signal(SIGINT,sig_hand);
	signal(SIGHUP,sig_hand);
	signal(SIGILL,sig_hand);
	signal(SIGFPE,sig_hand);
	signal(SIGBUS,sig_hand);
	signal(SIGABRT,sig_hand);
	signal(SIGQUIT,sig_hand);
	signal(SIGTERM,sig_hand);
	signal(SIGSEGV,sig_hand);
	signal(SIGTRAP,sig_hand);
	signal(SIGALRM,sig_hand);
	signal(SIGWINCH,SIG_IGN);
	signal(SIGUSR1,SIG_IGN);
	signal(SIGUSR2,SIG_IGN);
	signal(SIGCHLD,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
	signal(SIGTTOU,SIG_IGN);
}

static bool set_time(char*value){
	char*end;
	times=(unsigned)strtol(value,&end,10);
	return (!*end)&&(value!=end)&&(errno==0);
}

int main(int argc,char**argv){
	/* setup random */
	srand((unsigned)time(NULL));

	/* min none args */
	if(argc<1)return usage(1);

	/* max one args */
	if(argc>2)return usage(1);

	/* set play time */
	if(argc==2&&!set_time(argv[1]))return usage(1);

	/* search pc speaker device */
	if((fd=find_snd_input())<0)return -1;

	/* setup signals handler */
	setup_signals();

	/* setup play time alarm */
	if(times>0)alarm(times);

	puts("start playing music");
	for(;;){
		/* freq 200-4000, play 10ms-300ms, delay 1ms-100ms */
		int freq=random_int(4000,200);
		int play=random_int(300,10);
		int delay=random_int(300,10);
		beep(fd,freq,play);
		msleep(delay);
	}

	/* cleanup */
	close(fd);
	return 0;
}
