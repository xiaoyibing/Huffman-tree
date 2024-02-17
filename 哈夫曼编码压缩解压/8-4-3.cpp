//压缩
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义结构体用于存储字符和哈夫曼编码
struct CharEncoding {
    char data[10];
    char code[50];//把数写大一些，不要低估了编码长度 
};

// 从文件中读取字符和编码，返回结构体数组
struct CharEncoding* readCodesFromFile(const char *filename) {
    FILE *file;
    char line[50];
    struct CharEncoding *charEncodings = NULL;
    int n = 0;

    // 打开文件
    file = fopen(filename, "r");
    if (file == NULL) {perror("文件无法打开\n");return NULL;}

    // 读取文件内容，并统计行数
    while (fgets(line, sizeof(line), file)) {n++;}

    // 重新定位文件指针到文件开头
    fseek(file, 0, SEEK_SET);

    // 分配内存以存储结构体数组
    charEncodings = (struct CharEncoding*) malloc((n+1) * sizeof(struct CharEncoding));

    // 逐行读取文件内容，并将数据存储到结构体数组中
    for (int i = 0; i < n; i++) {
        fgets(line, sizeof(line), file);
        strcpy(charEncodings[i].data,strtok(line, " "));
        strcpy(charEncodings[i].code, strtok(NULL, " \n"));
    }

    charEncodings[n].data[0] = '\0';//末尾加'\0'示意结束 
    fclose(file);
    return charEncodings;
}

// 函数：根据字符查找其编码
char* findEncoding(struct CharEncoding *charEncodings, char* character) {//对于新手来说，这里的*很容易迷 ，要多练 
    for (int i = 0; charEncodings[i].data[0] != '\0'; i++) {//如果不是结构体最后一行个 
        if (strcmp(charEncodings[i].data, character) == 0) {//比较字符串要用strcmp！ 
            return charEncodings[i].code;
        }
    }
    return NULL; // 如果未找到对应编码，返回NULL
}

// 函数：将文本文件压缩成编码文件
void compressText(const char *inputFilename, const char *outputFilename, struct CharEncoding *charEncodings) {
    FILE *inputFile, *outputFile;
    char character[4]; // 修改为 char 类型，不要写[3]!,因为有'\0'占一个 
    char *encoding;
    unsigned char buffer=0;//用来表示无符号的字符型数据 
    int count = 0;
    
    
    // 打开文件
    inputFile = fopen(inputFilename, "r");
    if (inputFile == NULL) {perror("文件无法打开\n");return;}
    outputFile = fopen(outputFilename, "wb");//二进制写入用wb,读取用rb,0b开头是二进制数 
    if (outputFile == NULL) {perror("文件无法打开\n");fclose(inputFile);return;}
    
    // 逐个汉字读取(汉字一般在UTF-8 编码中占3个字节)输入文件内容，并压缩到输出文件中
    while(fgets(character, 3, inputFile)){
    	//更换换行符 
        if(character[0]=='\n'){
        	strcpy(character, "\\n"); 
		}
        encoding = findEncoding(charEncodings, character); // 查找编码 
        if (encoding != NULL) {
            // 将编码按位写入
            for (int i = 0; encoding[i] != '\0'; i++) {
                if (encoding[i] == '1') {
				//将左侧操作数 buffer 的每个位与右侧操作数 1 进行按位或运算，并将结果赋值给左侧操作数 buffer
                    buffer |= 1;
                }
                count++;
                if (count != 8) {
                	buffer <<= 1;//找了半天原来错在这里，一定要有限制条件，没满8位再加，最好放下一个if的前面 
                }
				if(count == 8){
                    fwrite(&buffer, sizeof(buffer), 1, outputFile);//满8位后整体写入 
                    buffer = 0;
                    count = 0;	//归零 
				}    
            }
        }
    }
    // 处理缓冲区中剩余的位数
    if (count > 0) {
        //如果编码长度不是8的整数倍，只需要在最后一个字节中填充到8位即可
        buffer <<= (8 - count);
        fwrite(&buffer, sizeof(buffer), 1, outputFile);
    }
    fclose(inputFile);
    fclose(outputFile);
}


int main(){
	printf("正在运行中..\n"); 
    struct CharEncoding *charEncodings = NULL;

    charEncodings = readCodesFromFile("codes.txt");

    compressText("yuan_wen.txt", "yasuo.bin", charEncodings);
    // 释放结构体数组的内存
    free(charEncodings);
    printf("\n完成");
    return 0;
}
