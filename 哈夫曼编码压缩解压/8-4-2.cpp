//生成编码
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// 定义哈夫曼树节点的结构体
typedef struct Node {
    char* data;
    int weight;
    struct Node* parent;
    struct Node* left; // 指向左子节点的指针
    struct Node* right; // 指向右子节点的指针
    char code[50];//带空间大小 
} Node;

// 创建一个新的哈夫曼树节点
Node* createNode(char* data, int weight) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = strdup(data); // 使用 strdup 复制字符串
    newNode->weight = weight;
    newNode->parent=newNode->right = newNode->left = NULL;
    newNode->code[0] = '\0';//特殊填写，不能直接用NULL 
    return newNode;
}

// 定义哈夫曼树节点指针类型
typedef Node* NodeType;

// 交换两个节点的位置（用于排序）
void swap(NodeType* a, NodeType* b) {
    NodeType temp = *a;
    *a = *b;
    *b = temp;
}

// 选择排序，按节点的权重从小到大排序
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

// 构建哈夫曼树
Node* buildHuffmanTree(NodeType nodes[], int n) {
    while (n > 1) {
        selectionSort(nodes, n); // 按权重排序

        // 创建新节点，合并权重最小的两个节点
        Node* newNode = createNode(NULL, nodes[0]->weight + nodes[1]->weight);
        
        nodes[0]->parent=nodes[1]->parent=newNode;
        
        newNode->left = nodes[0];
        newNode->right = nodes[1];

        // 删除已合并的两个节点
        for (int i = 0; i < n - 2; i++) {
            nodes[i] = nodes[i + 2];
        }
        n--;
        nodes[n-1] = newNode;
    }
    return nodes[0]; // 返回根节点
}
//构建编码 
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

// 输出
void displayHuffmanTree(Node* root, FILE *file) {

     
    if (root != NULL) {
        if (root->data != NULL){
        	fprintf(file, "%s %s\n", root->data, root->code);//写入文件 
            printf("%s %s\n", root->data, root->code);// 终端输出
    	}
        displayHuffmanTree(root->left,file);
        displayHuffmanTree(root->right,file);
    }
}




// 释放空间
void freeNodeMemory(Node* node) {
    if (node != NULL) {
        freeNodeMemory(node->left);
        freeNodeMemory(node->right);
        free(node->data);
        free(node);
    }
}

// 计算文件有多少行
int countLines(FILE* file) {
    int n = 0;
    char chr;
    while ((chr = fgetc(file)) != EOF) {
        if (chr == '\n') {
            n++;
        }
    }
    rewind(file);
    printf("共有%d个汉字\n",n);
    return n;//返回行数 
}


// 输入
NodeType* processFile(const char* filename, int n) {
    FILE* fp;
    char line[100];
    char* token;
    char* data;
    int weight;
    NodeType* nodes;

    fp = fopen(filename, "r");//打开统计次数的文件 
    if (fp == NULL) {printf("无法打开.\n");return NULL;}

    nodes = (NodeType*)malloc(sizeof(NodeType) * n);

    int i = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        token = strtok(line, " ");//读取汉字 
        if (token != NULL) {
            data = strdup(token);
            token = strtok(NULL, "\n");
            if (token != NULL) {
                weight = atoi(token + 1);//读取次数 
            }
        }
        nodes[i] = createNode(data, weight);
        i++;
    }

    fclose(fp);
    return nodes;
}



int main() {
	printf("正在运行中，请等待10s左右....\n");
    int n;
    NodeType* nodes;

	//统计行数 
    FILE* fp = fopen("quan_zhi.txt", "r");
    if (fp == NULL) {printf("无法打开文件.\n");return 1;}
    n = countLines(fp);//行数 
    fclose(fp);
      
    nodes = processFile("quan_zhi.txt", n);
    if (nodes == NULL) {printf("出现错误.\n");return 1;}
 
    Node* root = buildHuffmanTree(nodes, n);//构建哈夫曼树 
    assignBinaryCode(root);//构建编码 
    
    
    FILE *file = fopen("codes.txt", "w"); // 打开文件，使用写入模式 ("w")
    displayHuffmanTree(root,file);
	fclose(file);
	
	
    freeNodeMemory(root);//释放空间 
    free(nodes);
    printf("已完成。\n"); 

    return 0;
}

