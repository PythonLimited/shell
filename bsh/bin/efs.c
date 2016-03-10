#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>

struct stat st = { 0 };

char source[] = "/dev/block/bootdevice/by-name/efs";
char mountpoint[] = "/sdcard/.efs";
char type[] = "ext4";
char mode[] = "0";
char options[] = "mode=0777";
char destination[] = "/sdcard/.efs";

void backup()
{
	FILE *f;
	FILE *fp;
	char buffer[255];
	char buff[255];
	f = fopen("/sdcard/.PythonLimited/backups/BACKUP_MAC.bbr", "w+");
	fp = fopen("/sdcard/.efs/wifi/.mac.info", "w+");
	fscanf(fp, " %s", buffer);
	printf("Actual MAC address seems to be : %s\n", buffer);
	printf("Changing backup file to that\n");
	fputs(buffer, f);
	fclose(f);
	fclose(fp);
}

void wifi()
{
	printf("Killing wifi connection\n");
	system("svc wifi disable");
	printf("Success\n");
}

void crack()
{
	FILE *f;
	FILE *fp;
	char buffer[255];
	char buff[255];
	f = fopen("/sdcard/.PythonLimited/default/DEFAULT_MAC.bbr", "w+");
	fp = fopen("/sdcard/.efs/wifi/.mac.info", "w+");
	fscanf(fp, " %s", buffer);
	printf("%s\n", buffer);
	fscanf(f, "%s", buff);
	printf("Default MAC address seems to be : %s\n", buff);
	printf("Changing MAC address to that\n");
	fputs(buff, fp);
	fclose(fp);
	fclose(f);
}

void folder()
{
	printf("Creating directorys\n");
	mkdir("/sdcard/.efs", 0700);
	mkdir(destination, 0700);
	mkdir("/sdcard/.PythonLimited", 0700);
	mkdir("/sdcard/.PythonLimited/backups", 0700);
	mkdir("/sdcard/.PythonLimited/default", 0700);
	perror(errno);
}

void unmount()
{
	printf("Unmounting\n");
	umount("/efs");
	umount("/sdcard/.efs");
	
}

void mountp()
{
	printf("Umounting previusly mounted folders\n");
	umount("/efs");
	umount("/sdcard/.efs");
	printf("Mounting '%s'\n", source);
	system("mount -t ext4 /dev/block/bootdevice/by-name/efs /sdcard/.efs");
	perror(errno);
}


void macdefault(char mac[])
{
	printf("Changing default MAC address to '%s'\n", mac);
	FILE *fp;
	fp = fopen("/sdcard/.PythonLimited/default/DEFAULT_MAC.bbr", "w+");
	fputs(mac, fp);
	fclose(fp);
	printf("Done\n");
}

void efscrack()
{
	wifi();
	folder();
	mountp();
	crack();
	unmount();
	printf("All done\n");
	printf("Reboot required\n");
}

void efsbackup()
{
	folder();
	mountp();
	backup();
	printf("All done\n");
}