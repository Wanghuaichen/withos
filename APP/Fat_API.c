#include "Fat_API.h"

FATFS fs; 
u8 SD_Buffer[BufferSize];
u8 FileList[MaxFileCount][30];//最多支持100个文件
char FilePath[96]={"0:"};
u16 filecount = 0;

void FilePath_process(char * source,char *addstring)
{	
	u8 count = 0;
	while(*source != '\0'){
    source++;
		count++;
  }
	if(*addstring == '.' && *(addstring+1) == '.') {
		if(*(source-1) == ':' && *(source-2) == '0' && count == 2) return;
		while(*source != '/') {
		  *source = '\0';
			source--;
		}
		*source = '\0';
		source--;
	} else{
	  *source = '/';
	  source++;
	  while(*addstring != '\0') {
		  *source = *addstring;
		  source++;
		  addstring++;
	  }
  }
}
void ClearPath(char *path)
{
	path += 2;
	while(*path){
		*path = '\0';
		path++;
	}
}

void FatFs_Open(char *filename)
{
  OS_CPU_SR  cpu_sr;
// 	UINT ss;
	u8 res = 0;
// 	FIL infile;
	DIR root_file;
	FILINFO file_name;
// 	u32 file_size = 0;
	#if _USE_LFN
	  char lfn[30];
		file_name.lfname = lfn;
		file_name.lfsize = 30;
	#endif
	 OS_ENTER_CRITICAL();                  
	
		res = f_opendir(&root_file,filename);//根目录的名字是""
		filecount = 0;
		while((f_readdir(&root_file, &file_name) == FR_OK) && file_name.fname[0]){ 
			if(filecount < MaxFileCount){
				strcpy((char *)FileList[filecount],file_name.fname);//file_name.fname
				filecount ++;
      } else {
				break;
			}
// 			printf((char*)FileList[filecount]);
// 			printf("\r\n");
    }
	OS_EXIT_CRITICAL();
// 		printf("filecount = %d\r\n",filecount);
// printf("\r\n电子电路:\r\n\r\n");
// res = f_opendir(&root_file, "电子电路");        //打开目录	
// while((f_readdir(&root_file, &file_name) == FR_OK) && file_name.fname[0]){  
// 	printf((char *)file_name.fname);
// 	printf("\r\n");
// }
		
// retry = f_open(&infile,"SD.txt",FA_CREATE_NEW);    //创建新文件
// 	res = f_open(&infile, "test.txt", FA_OPEN_EXISTING | FA_READ);
// // f_write(&infile, "abcdefg",8, &ss);
// 	if(res != FR_OK) return;
// 	file_size = infile.fsize;
// 		printf("test.txt size: %ld\r\n",file_size);
// // 		TIME = 0;
// 	    res = f_read(&infile, SD_Buffer, BufferSize-1, &ss);
// // 		printf((char *)SD_Buffer);
// 			if (res != FR_OK) {
// 				printf("File read error!\r\n");
// // // // 				break;
// 			} 
// // 			if(ss<512)break;//读到文件末尾
//   f_close(&infile);    //关闭文件
}
// void FileOpen_Txt(char *buf)
// {
// 	u8 res = 0;
// 	FIL infile;
//   u32 file_size = 0;	
// 	UINT ss;
// 	FilePath_process(FilePath,buf);
// 	res = f_open(&infile, FilePath, FA_OPEN_EXISTING | FA_READ);
// 	if(res != FR_OK) return;
// 	file_size = infile.fsize;
// 	printf("**.txt size: %ld\r\n",file_size);
// 	ClearPath((char *)SD_Buffer);
// 	res = f_read(&infile, SD_Buffer, BufferSize-1, &ss);
// 	if (res != FR_OK) {
// 		 printf("File read error!\r\n");
// 	} 
//   f_close(&infile);    //关闭文件
// 	FilePath_process(FilePath,"..");
// }
