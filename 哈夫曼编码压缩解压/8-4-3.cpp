//ѹ��
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

// �����������ַ����������
char* findEncoding(struct CharEncoding *charEncodings, char* character) {//����������˵�������*�������� ��Ҫ���� 
    for (int i = 0; charEncodings[i].data[0] != '\0'; i++) {//������ǽṹ�����һ�и� 
        if (strcmp(charEncodings[i].data, character) == 0) {//�Ƚ��ַ���Ҫ��strcmp�� 
            return charEncodings[i].code;
        }
    }
    return NULL; // ���δ�ҵ���Ӧ���룬����NULL
}

// ���������ı��ļ�ѹ���ɱ����ļ�
void compressText(const char *inputFilename, const char *outputFilename, struct CharEncoding *charEncodings) {
    FILE *inputFile, *outputFile;
    char character[4]; // �޸�Ϊ char ���ͣ���Ҫд[3]!,��Ϊ��'\0'ռһ�� 
    char *encoding;
    unsigned char buffer=0;//������ʾ�޷��ŵ��ַ������� 
    int count = 0;
    
    
    // ���ļ�
    inputFile = fopen(inputFilename, "r");
    if (inputFile == NULL) {perror("�ļ��޷���\n");return;}
    outputFile = fopen(outputFilename, "wb");//������д����wb,��ȡ��rb,0b��ͷ�Ƕ������� 
    if (outputFile == NULL) {perror("�ļ��޷���\n");fclose(inputFile);return;}
    
    // ������ֶ�ȡ(����һ����UTF-8 ������ռ3���ֽ�)�����ļ����ݣ���ѹ��������ļ���
    while(fgets(character, 3, inputFile)){
    	//�������з� 
        if(character[0]=='\n'){
        	strcpy(character, "\\n"); 
		}
        encoding = findEncoding(charEncodings, character); // ���ұ��� 
        if (encoding != NULL) {
            // �����밴λд��
            for (int i = 0; encoding[i] != '\0'; i++) {
                if (encoding[i] == '1') {
				//���������� buffer ��ÿ��λ���Ҳ������ 1 ���а�λ�����㣬���������ֵ���������� buffer
                    buffer |= 1;
                }
                count++;
                if (count != 8) {
                	buffer <<= 1;//���˰���ԭ���������һ��Ҫ������������û��8λ�ټӣ���÷���һ��if��ǰ�� 
                }
				if(count == 8){
                    fwrite(&buffer, sizeof(buffer), 1, outputFile);//��8λ������д�� 
                    buffer = 0;
                    count = 0;	//���� 
				}    
            }
        }
    }
    // ����������ʣ���λ��
    if (count > 0) {
        //������볤�Ȳ���8����������ֻ��Ҫ�����һ���ֽ�����䵽8λ����
        buffer <<= (8 - count);
        fwrite(&buffer, sizeof(buffer), 1, outputFile);
    }
    fclose(inputFile);
    fclose(outputFile);
}


int main(){
	printf("����������..\n"); 
    struct CharEncoding *charEncodings = NULL;

    charEncodings = readCodesFromFile("codes.txt");

    compressText("yuan_wen.txt", "yasuo.bin", charEncodings);
    // �ͷŽṹ��������ڴ�
    free(charEncodings);
    printf("\n���");
    return 0;
}
