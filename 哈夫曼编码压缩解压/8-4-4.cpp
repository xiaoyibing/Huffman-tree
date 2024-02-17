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


// 函数：根据编码查找对应字符
char* findCharacter(struct CharEncoding *charEncodings, char *code) {
    for (int i = 0; charEncodings[i].data[0] != '\0'; i++) {
        if (strcmp(charEncodings[i].code, code) == 0) {
            return charEncodings[i].data;
        }
    }
    return NULL; // 如果未找到对应字符，返回空指针
}

void decompressText(const char *inputFilename, const char *outputFilename, struct CharEncoding *charEncodings) {
    FILE *inputFile, *outputFile;
    unsigned char buffer;
    char code[50];
    int codeIndex = 0;

    // 打开输入文件
    inputFile = fopen(inputFilename, "rb");//读取二进制文件用rb 
    if (inputFile == NULL) {perror("文件无法打开\n");return;}
    outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {perror("文件无法打开\n");fclose(inputFile);return;}


    // 逐比特读取输入文件内容，并解压到输出文件中
    while (fread(&buffer, sizeof(buffer), 1, inputFile) == 1) {
        for (int bit = 7; bit >= 0; bit--) {
            // 将当前比特添加到编码字符串中
            code[codeIndex] = (buffer >> bit) & 1 ? '1' : '0';//条件运算符,如果 (buffer >> bit) & 1 的结果为真（即最低位为 1），则选择 '1'，否则选择 '0'
            codeIndex++;
            code[codeIndex] = '\0'; // 在编码末尾添加字符串终止符

            // 检查否与编码表中的编码匹配
            char* character = findCharacter(charEncodings, code);
            
            if (character != NULL) {
            	if(strcmp(character, "\\n") == 0){fprintf(outputFile, "\n");}//换行符，额外处理 
                else fprintf(outputFile, "%s", character); // 输出字符
                    
                // 清空编码字符串
                memset(code, '\0', sizeof(code));

                codeIndex = 0; // 重置
            }
        }
    }
    // 关闭文件
    fclose(inputFile);
    fclose(outputFile);
}


int main() {
	printf("正在运行中(请耐心等待30秒左右)...\n");
    struct CharEncoding *charEncodings = NULL;

    charEncodings = readCodesFromFile("codes.txt");
    if (charEncodings == NULL) {printf("出现错误.\n");return 1;}

    decompressText("yasuo.bin", "jieya.txt", charEncodings);


    free(charEncodings);
    printf("解压完成\n");
    return 0;
}


