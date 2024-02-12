#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ������������ڵ�Ľṹ��
typedef struct Node {
    char* data;
    int weight;
    struct Node* left;
    struct Node* right;
} Node;

// ����һ���µĹ��������ڵ�
Node* createNode(char* data, int weight) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = strdup(data); // ʹ�� strdup �����ַ���
    newNode->weight = weight;
    newNode->left = NULL;
    newNode->right = NULL;
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
        newNode->left = nodes[0];
        newNode->right = nodes[1];

        // ɾ���Ѻϲ��������ڵ�
        nodes[0] = newNode;
        for (int i = 1; i < n - 1; i++) {
            nodes[i] = nodes[i + 1];
        }
        n--;
    }
    return nodes[0]; // ���ظ��ڵ�
}

// ��ʾ������������
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
    printf("���������ݸ�����");
    scanf("%d", &n);

    char data[n][50]; 
    int weight[n];

    printf("�������������ƺ����ݵ�Ȩֵ���Զ��ŷָ�(�磺a,1,e,3,r,4,t,5),//ע����Ӣ�Ķ��ţ�\n");
    for (int i = 0; i < n; i++) {
        scanf(" %[^,], %d,", data[i], &weight[i]); // ʹ�� %[^\n] ��ȡ����ֱ������
    }

    NodeType* nodes = (NodeType*)malloc(sizeof(NodeType) * n);
    for (int i = 0; i < n; i++) {
        nodes[i] = createNode(data[i], weight[i]);
    }

    Node* root = buildHuffmanTree(nodes, n);

    printf("��������:\n");
    displayHuffmanTree(root);

    // �ͷ��ڴ�
    for (int i = 0; i < n; i++) {
        free(nodes[i]->data); // �ͷ� strdup ������ڴ�
        free(nodes[i]);
    }
    free(nodes);

    return 0;
}

