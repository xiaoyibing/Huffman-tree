#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ����ṹ�����ڴ洢�ַ��͹���������
struct CharEncoding {
    char data[10];
    char code[50];//����д��һЩ����Ҫ�͹��˱��볤�� 
};

// ���ļ��ж�ȡ�ַ��ͱ��룬���ؽṹ������
struct CharEncoding* readCodesFromFile(const char *filename) {
    FILE *file;
    char line[50];
    struct CharEncoding *charEncodings = NULL;
    int n = 0;

    // ���ļ�
    file = fopen(filename, "r");
    if (file == NULL) {perror("�ļ��޷���\n");return NULL;}

    // ��ȡ�ļ����ݣ���ͳ������
    while (fgets(line, sizeof(line), file)) {n++;}

    // ���¶�λ�ļ�ָ�뵽�ļ���ͷ
    fseek(file, 0, SEEK_SET);

    // �����ڴ��Դ洢�ṹ������
    charEncodings = (struct CharEncoding*) malloc((n+1) * sizeof(struct CharEncoding));

    // ���ж�ȡ�ļ����ݣ��������ݴ洢���ṹ��������
    for (int i = 0; i < n; i++) {
        fgets(line, sizeof(line), file);
        strcpy(charEncodings[i].data,strtok(line, " "));
        strcpy(charEncodings[i].code, strtok(NULL, " \n"));
    }

    charEncodings[n].data[0] = '\0';//ĩβ��'\0'ʾ����� 
    fclose(file);
    return charEncodings;
}


// ���������ݱ�����Ҷ�Ӧ�ַ�
char* findCharacter(struct CharEncoding *charEncodings, char *code) {
    for (int i = 0; charEncodings[i].data[0] != '\0'; i++) {
        if (strcmp(charEncodings[i].code, code) == 0) {
            return charEncodings[i].data;
        }
    }
    return NULL; // ���δ�ҵ���Ӧ�ַ������ؿ�ָ��
}

void decompressText(const char *inputFilename, const char *outputFilename, struct CharEncoding *charEncodings) {
    FILE *inputFile, *outputFile;
    unsigned char buffer;
    char code[50];
    int codeIndex = 0;

    // �������ļ�
    inputFile = fopen(inputFilename, "rb");//��ȡ�������ļ���rb 
    if (inputFile == NULL) {perror("�ļ��޷���\n");return;}
    outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {perror("�ļ��޷���\n");fclose(inputFile);return;}


    // ����ض�ȡ�����ļ����ݣ�����ѹ������ļ���
    while (fread(&buffer, sizeof(buffer), 1, inputFile) == 1) {
        for (int bit = 7; bit >= 0; bit--) {
            // ����ǰ������ӵ������ַ�����
            code[codeIndex] = (buffer >> bit) & 1 ? '1' : '0';//���������,��� (buffer >> bit) & 1 �Ľ��Ϊ�棨�����λΪ 1������ѡ�� '1'������ѡ�� '0'
            codeIndex++;
            code[codeIndex] = '\0'; // �ڱ���ĩβ����ַ�����ֹ��

            // �����������еı���ƥ��
            char* character = findCharacter(charEncodings, code);
            
            if (character != NULL) {
            	if(strcmp(character, "\\n") == 0){fprintf(outputFile, "\n");}//���з������⴦�� 
                else fprintf(outputFile, "%s", character); // ����ַ�
                    
                // ��ձ����ַ���
                memset(code, '\0', sizeof(code));

                codeIndex = 0; // ����
            }
        }
    }
    // �ر��ļ�
    fclose(inputFile);
    fclose(outputFile);
}


int main() {
	printf("����������(�����ĵȴ�30������)...\n");
    struct CharEncoding *charEncodings = NULL;

    charEncodings = readCodesFromFile("codes.txt");
    if (charEncodings == NULL) {printf("���ִ���.\n");return 1;}

    decompressText("yasuo.bin", "jieya.txt", charEncodings);


    free(charEncodings);
    printf("��ѹ���\n");
    return 0;
}


