#define _INCLUDE_GLOBLE
#define _INIT_GLOBLE
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include "include.h"
/**
 * @Date: 2021-04-17 20:23:36
 * @Author: Ciao
 * new add
 */
extern int onenum;
int main(void) {
	int h2f_lw_axi_master_fd;
	int h2f_axi_master_fd;
	void* h2f_lw_axi_master_vbase = 0; //h2f_lw_axi_master
	void* h2f_axi_master_vbase = 0; //h2f_axi_master
#ifndef INOTIFY_ENABLE
	char color = 0;
#endif
	char filename[50];
	unsigned int fileevent;
	int ret;
	int i, j = 0;
	char dirPath[6] = "./pic";
	char picdir[30][256] = {0};

	//init
	h2f_lw_axi_master_fd = h2f_lw_axi_master_init(&h2f_lw_axi_master_vbase);
	h2f_axi_master_fd = h2f_axi_master_init(&h2f_axi_master_vbase);
	fbdev_init();
#ifdef INOTIFY_ENABLE
	pic_monitor_init();
#endif
	parameter_init();

	//do
	//while (1)
	{
#ifdef INOTIFY_ENABLE
//		//读取inotify的监视事件信息
//		memset(event_buf, 0, INOTIFY_EVENT_COUNT);
//		result = read(inotify_fd, event_buf, INOTIFY_EVENT_COUNT);
//		assert(result >= (int )sizeof(*event));
//		event_pos = 0; //每次读取数据时复位数组脚标
//		while (result >= (int) sizeof(*event)) {
//			event = (struct inotify_event *) (event_buf + event_pos);
//			if (event->len) {
//				//创建一个文件时
//				if (event->mask & IN_CREATE) {
//					FILE_TRACE("file created", event->name);
//					memset(filename, 0, 50);
//					strcpy(filename, event->name);
//				}
//				//文件上传完毕
//				if (event->mask & IN_CLOSE_WRITE) {
//					if (!strcmp(filename, event->name)) {
//						FILE_TRACE("file upload", event->name);
//						ret = read_pic(event->name);
//						if (ret)
//							handle_pic();
//						strcpy(license, "");
//#ifdef HLS_DEBUG
//						strcpy(license_hps, "");
//#endif
//						for (i = 0; i < chrnum; i++) {
//							ret = chrinfer(i);
//						}
//						VALUEs_TRACE("license result", license);
//#ifdef HLS_DEBUG
//						VALUEs_TRACE("license_hps result", license_hps);
//#endif
//						//最终结果打印
//						Results_PRINT("license result", license);
//						/**
//						 * @Date: 2021-04-17 20:23:54
//						 * @Author: Ciao
//						 * new add
//						 */
//						onenum = -1;
//					}
//				}
//			}
//			/* 更新位置信息,以便获得下一个inotify_event对象的首地址*/
//			event_size = sizeof(*event) + event->len;
//			result -= event_size;
//			event_pos += event_size;
//		}
		DIR *dir=opendir(dirPath);
		    if(dir==NULL)
		        {
		            printf("%d\n",strerror(errno));
		            return 1;
		        }
		    chdir(dirPath);         //进入到当前读取目录
		    struct dirent *ent;
		    while ((ent=readdir(dir))!=NULL)
		        {
		            if(strcmp(ent->d_name,".")==0||strcmp(ent->d_name,"..")==0)
		                {
		                    continue;
		                }
		            struct stat st;
		            stat(ent->d_name,&st);
		            if(S_ISDIR(st.st_mode))
		                {

		                }
		            else
		                {
		            		strcpy(picdir[j++], ent->d_name);//picdir[j++] = ent->d_name;
		                }
		        }
		    closedir(dir);
		    chdir("..");    //返回当前目录的上一级目录
		    for(j = 0;j<30;j++){
				//printf("%s\n",ent->d_name);
				ret = read_pic(picdir[j]);
				if (ret)
					handle_pic();
				strcpy(license, "");
				for (i = 0; i < chrnum; i++) {
					ret = chrinfer(i);
				}
				VALUEs_TRACE("license result", license);
				//最终结果打印
				Results_PRINT("license result", license);
				/**
				 * @Date: 2021-04-17 20:23:54
				 * @Author: Ciao
				 * new add
				 */
				onenum = -1;
		    }
#else
//		color++;
//		if (color == 0xff)
//		color = 0;
//		wait_pan_framebuffer();
//		set_pan_framebuffer();
//		memset(fbdev.fb_currunt_vmap, color, fbdev.fb_screensize);
//		pan_framebuffer();
#endif
	}

	return 1;
}

#undef _INCLUDE_GLOBLE
