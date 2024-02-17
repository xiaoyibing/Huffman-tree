//���ɱ���
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// ������������ڵ�Ľṹ��
typedef struct Node {
    char* data;
    int weight;
    struct Node* parent;
    struct Node* left; // ָ�����ӽڵ��ָ��
    struct Node* right; // ָ�����ӽڵ��ָ��
    char code[50];//���ռ��С 
} Node;

// ����һ���µĹ��������ڵ�
Node* createNode(char* data, int weight) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = strdup(data); // ʹ�� strdup �����ַ���
    newNode->weight = weight;
    newNode->parent=newNode->right = newNode->left = NULL;
    newNode->code[0] = '\0';//������д������ֱ����NULL 
    return newNode;
}

// ������������ڵ�ָ������
typedef Node* NodeType;

// ���������ڵ��λ�ã���������
void swap(NodeType* a, NodeType* b) {
    NodeType temp = *a;
    *a = *b;
    *b = temp;
}

// ѡ�����򣬰��ڵ��Ȩ�ش�С��������
void selectionSort(NodeType nodes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_index = i;
        for (int j = i + 1; j < n; j++) {
            if (nodes[j]->weight < nodes[min_index]->weight) {
                min_index = j;
            }
        }
        swap(&nodes[min_index], &nodes[i]);
    }
}

// ������������
Node* buildHuffmanTree(NodeType nodes[], int n) {
    while (n > 1) {
        selectionSort(nodes, n); // ��Ȩ������

        // �����½ڵ㣬�ϲ�Ȩ����С�������ڵ�
        Node* newNode = createNode(NULL, nodes[0]->weight + nodes[1]->weight);
        
        nodes[0]->parent=nodes[1]->parent=newNode;
        
        newNode->left = nodes[0];
        newNode->right = nodes[1];

        // ɾ���Ѻϲ��������ڵ�
        for (int i = 0; i < n - 2; i++) {
            nodes[i] = nodes[i + 2];
        }
        n--;
        nodes[n-1] = newNode;
    }
    return nodes[0]; // ���ظ��ڵ�
}
//�������� 
void assignBinaryCode(Node* root) {
    if (root != NULL) {
        Node* currentNode = root;
        while (currentNode->parent != NULL) {
            if (currentNode->parent->left == currentNode) {
                memmove(root->code + 1, root->code, strlen(root->code) + 1);
                root->code[0] = '0';
            } else {
                memmove(root->code + 1, root->code, strlen(root->code) + 1);
                root->code[0] = '1';
            }
            currentNode = currentNode->parent;
        }
        assignBinaryCode(root->left);
        assignBinaryCode(root->right);
    }
}

// ���
void displayHuffmanTree(Node* root, FILE *file) {

     
    if (root != NULL) {
        if (root->data != NULL){
        	fprintf(file, "%s %s\n", root->data, root->code);//д���ļ� 
            printf("%s %s\n", root->data, root->code);// �ն����
    	}
        displayHuffmanTree(root->left,file);
        displayHuffmanTree(root->right,file);
    }
}




// �ͷſռ�
void freeNodeMemory(Node* node) {
    if (node != NULL) {
        freeNodeMemory(node->left);
        freeNodeMemory(node->right);
        free(node->data);
        free(node);
    }
}

// �����ļ��ж�����
int countLines(FILE* file) {
    int n = 0;
    char chr;
    while ((chr = fgetc(file)) != EOF) {
        if (chr == '\n') {
            n++;
        }
    }
    rewind(file);
    printf("����%d������\n",n);
    return n;//�������� 
}


// ����
NodeType* processFile(const char* filename, int n) {
    FILE* fp;
    char line[100];
    char* token;
    char* data;
    int weight;
    NodeType* nodes;

    fp = fopen(filename, "r");//��ͳ�ƴ������ļ� 
    if (fp == NULL) {printf("�޷���.\n");return NULL;}

    nodes = (NodeType*)malloc(sizeof(NodeType) * n);

    int i = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        token = strtok(line, " ");//��ȡ���� 
        if (token != NULL) {
            data = strdup(token);
            token = strtok(NULL, "\n");
            if (token != NULL) {
                weight = atoi(token + 1);//��ȡ���� 
            }
        }
        nodes[i] = createNode(data, weight);
        i++;
    }

    fclose(fp);
    return nodes;
}



int main() {
	printf("���������У���ȴ�10s����....\n");
    int n;
    NodeType* nodes;

	//ͳ������ 
    FILE* fp = fopen("quan_zhi.txt", "r");
    if (fp == NULL) {printf("�޷����ļ�.\n");return 1;}
    n = countLines(fp);//���� 
    fclose(fp);
      
    nodes = processFile("quan_zhi.txt", n);
    if (nodes == NULL) {printf("���ִ���.\n");return 1;}
 
    Node* root = buildHuffmanTree(nodes, n);//������������ 
    assignBinaryCode(root);//�������� 
    
    
    FILE *file = fopen("codes.txt", "w"); // ���ļ���ʹ��д��ģʽ ("w")
    displayHuffmanTree(root,file);
	fclose(file);
	
	
    freeNodeMemory(root);//�ͷſռ� 
    free(nodes);
    printf("����ɡ�\n"); 

    return 0;
}

