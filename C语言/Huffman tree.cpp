#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义哈夫曼树节点的结构体
typedef struct Node {
    char* data;
    int weight;
    struct Node* left;
    struct Node* right;
} Node;

// 创建一个新的哈夫曼树节点
Node* createNode(char* data, int weight) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = strdup(data); // 使用 strdup 复制字符串
    newNode->weight = weight;
    newNode->left = NULL;
    newNode->right = NULL;
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
        newNode->left = nodes[0];
        newNode->right = nodes[1];

        // 删除已合并的两个节点
        nodes[0] = newNode;
        for (int i = 1; i < n - 1; i++) {
            nodes[i] = nodes[i + 1];
        }
        n--;
    }
    return nodes[0]; // 返回根节点
}

// 显示哈夫曼树数组
void displayHuffmanTree(Node* root) {
    if (root != NULL) {
    	if(root->data == NULL)        
			printf("(,%d)\n",root->weight);
		else
        	printf("(%s,%d)\n", root->data, root->weight);
        displayHuffmanTree(root->left);
        displayHuffmanTree(root->right);
    }
}

int main() {
    int n;
    printf("请输入数据个数：");
    scanf("%d", &n);

    char data[n][50]; 
    int weight[n];

    printf("请输入数据名称和数据的权值，以逗号分隔(如：a,1,e,3,r,4,t,5),//注意是英文逗号：\n");
    for (int i = 0; i < n; i++) {
        scanf(" %[^,], %d,", data[i], &weight[i]); // 使用 %[^\n] 读取整行直到逗号
    }

    NodeType* nodes = (NodeType*)malloc(sizeof(NodeType) * n);
    for (int i = 0; i < n; i++) {
        nodes[i] = createNode(data[i], weight[i]);
    }

    Node* root = buildHuffmanTree(nodes, n);

    printf("哈夫曼树:\n");
    displayHuffmanTree(root);

    // 释放内存
    for (int i = 0; i < n; i++) {
        free(nodes[i]->data); // 释放 strdup 分配的内存
        free(nodes[i]);
    }
    free(nodes);

    return 0;
}

