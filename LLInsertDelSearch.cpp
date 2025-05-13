#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Node untuk binary tree
typedef struct Node {
    int data;
    struct Node *left, *right;
} Node;

// Queue untuk bantu level-order traversal
typedef struct QueueNode {
    Node *treeNode;
    struct QueueNode *next;
} QueueNode;

QueueNode *front = NULL, *rear = NULL;

// Queue operations
void enqueue(Node *treeNode) {
    QueueNode *temp = (QueueNode *)malloc(sizeof(QueueNode));
    temp->treeNode = treeNode;
    temp->next = NULL;
    if (!rear) front = rear = temp;
    else {
        rear->next = temp;
        rear = temp;
    }
}

Node* dequeue() {
    if (!front) return NULL;
    QueueNode *temp = front;
    Node *result = front->treeNode;
    front = front->next;
    if (!front) rear = NULL;
    free(temp);
    return result;
}

bool isQueueEmpty() {
    return front == NULL;
}

// Buat node baru
Node* createNode(int value) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Swap isi data antar dua node
void swap(Node *a, Node *b) {
    int temp = a->data;
    a->data = b->data;
    b->data = temp;
}

// Heapify turun (untuk delete root)
void heapifyDown(Node *root) {
    if (!root) return;
    Node *minNode = root;

    if (root->left && root->left->data < minNode->data)
        minNode = root->left;
    if (root->right && root->right->data < minNode->data)
        minNode = root->right;

    if (minNode != root) {
        swap(root, minNode);
        heapifyDown(minNode);
    }
}

// INSERT: masukkan node ke posisi yang tepat (level order), lalu heapify-up
void insert(Node **root, int value) {
    Node *newNode = createNode(value);
    if (!(*root)) {
        *root = newNode;
        return;
    }

    enqueue(*root);

    while (!isQueueEmpty()) {
        Node *temp = dequeue();

        if (!temp->left) {
            temp->left = newNode;
            if (temp->data > temp->left->data)
                swap(temp, temp->left);
            break;
        } else enqueue(temp->left);

        if (!temp->right) {
            temp->right = newNode;
            if (temp->data > temp->right->data)
                swap(temp, temp->right);
            break;
        } else enqueue(temp->right);
    }
}

// DELETE: hapus root dan ganti dengan node terakhir, lalu heapify-down
void deleteMin(Node **root) {
    if (!(*root)) {
        printf("Heap kosong!\n");
        return;
    }

    Node *temp = NULL, *last = NULL, *parent = NULL;
    enqueue(*root);

    // Cari node terakhir dan parent-nya
    while (!isQueueEmpty()) {
        temp = dequeue();
        if (temp->left) {
            enqueue(temp->left);
            parent = temp;
            last = temp->left;
        }
        if (temp->right) {
            enqueue(temp->right);
            parent = temp;
            last = temp->right;
        }
    }

    // Replace root data dengan data node terakhir
    if (last && *root) {
        (*root)->data = last->data;

        // Hapus node terakhir
        if (parent->right == last) {
            parent->right = NULL;
        } else if (parent->left == last) {
            parent->left = NULL;
        }

        free(last);

        // Perbaiki heap
        heapifyDown(*root);
    } else {
        // Jika hanya satu node (root)
        free(*root);
        *root = NULL;
    }
}

// SEARCH: cari elemen dalam heap (DFS rekursif)
bool search(Node *root, int key) {
    if (!root) return false;
    if (root->data == key) return true;
    return search(root->left, key) || search(root->right, key);
}

// INORDER (traversal untuk cek isi)
void inorder(Node *root) {
    if (!root) return;
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

// MAIN
int main() {
    Node *heap = NULL;
	int a, b, c, d, e, f, g, h, i, n;
	printf("Berapa insert node yang ingin dimasukkan?: ");
	scanf("%d", &n);
	for(int i = 0; i < n; i++){
		printf("Insert node: ");
		scanf("%d", &a);
    	insert(&heap, a);
	}
//	printf("Insert node: ");
//	scanf("%d", &a);
//    insert(&heap, a);
//    printf("Insert node: ");
//	scanf("%d", &b);
//    insert(&heap, b);
//    printf("Insert node: ");
//	scanf("%d", &c);
//    insert(&heap, c);
//    printf("Insert node: ");
//	scanf("%d", &d);
//    insert(&heap, d);
//    printf("Insert node: ");
//	scanf("%d", &e);
//    insert(&heap, e);
    
//    insert(&heap, 10);
//    insert(&heap, 5);
//    insert(&heap, 3);
//    insert(&heap, 12);
//    insert(&heap, 1);

    printf("Isi Heap (inorder): ");
    inorder(heap);
    printf("\n");

    printf("Apakah 12 ada? %s\n", search(heap, 12) ? "Ya" : "Tidak");
    printf("Apakah 99 ada? %s\n", search(heap, 99) ? "Ya" : "Tidak");

    printf("Menghapus elemen terkecil (min)...\n");
    deleteMin(&heap);

    printf("Isi Heap setelah deleteMin: ");
    inorder(heap);
    printf("\n");

    return 0;
}

