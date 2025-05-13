#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Node untuk binary tree
typedef struct Node {
    int data;
    struct Node *left, *right, *parent;
} Node;

// Queue node (untuk level-order traversal)
typedef struct QueueNode {
    Node *treeNode;
    struct QueueNode *next;
} QueueNode;

QueueNode *front = NULL, *rear = NULL;

// Queue Operations
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
    newNode->left = newNode->right = newNode->parent = NULL;
    return newNode;
}

// Swap data antara dua node
void swap(Node *a, Node *b) {
    int temp = a->data;
    a->data = b->data;
    b->data = temp;
}

// Heapify naik untuk insert (heapify-up)
void heapifyUp(Node *node) {
    while (node->parent && node->data < node->parent->data) {
        swap(node, node->parent);
        node = node->parent;
    }
}

// Heapify turun untuk delete root (heapify-down)
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

// INSERT: masukkan node baru dan jaga aturan min-heap
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
            newNode->parent = temp;
            heapifyUp(newNode);
            break;
        } else enqueue(temp->left);

        if (!temp->right) {
            temp->right = newNode;
            newNode->parent = temp;
            heapifyUp(newNode);
            break;
        } else enqueue(temp->right);
    }
}

// DELETE: hapus elemen terkecil (root) dan perbaiki struktur heap
void deleteMin(Node **root) {
    if (!(*root)) {
        printf("Heap kosong!\n");
        return;
    }

    Node *temp = NULL, *last = NULL;

    enqueue(*root);

    while (!isQueueEmpty()) {
        temp = dequeue();
        if (temp->left) {
            enqueue(temp->left);
            last = temp->left;
        }
        if (temp->right) {
            enqueue(temp->right);
            last = temp->right;
        }
    }

    if (last == *root) {
        free(*root);
        *root = NULL;
        return;
    }

    // Replace root dengan data node terakhir
    (*root)->data = last->data;

    // Hapus node terakhir
    Node *parent = last->parent;
    if (parent->right == last) parent->right = NULL;
    else if (parent->left == last) parent->left = NULL;
    free(last);

    // Heapify turun dari root
    heapifyDown(*root);
}

// SEARCH: cari nilai dalam tree secara rekursif
bool search(Node *root, int key) {
    if (!root) return false;
    if (root->data == key) return true;
    return search(root->left, key) || search(root->right, key);
}

// INORDER: untuk cek isi tree
void inorder(Node *root) {
    if (!root) return;
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

// Free semua node
void freeTree(Node *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// MAIN
int main() {
    Node *heap = NULL;
    int n, val;

    printf("Berapa banyak nilai yang ingin dimasukkan ke Min Heap? ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("Masukkan nilai ke-%d: ", i + 1);
        scanf("%d", &val);
        insert(&heap, val);
    }

    printf("Isi Heap (inorder): ");
    inorder(heap);
    printf("\n");

    printf("Cari nilai 12? %s\n", search(heap, 12) ? "Ditemukan" : "Tidak ditemukan");
    printf("Cari nilai 99? %s\n", search(heap, 99) ? "Ditemukan" : "Tidak ditemukan");

    printf("Menghapus elemen terkecil...\n");
    deleteMin(&heap);

    printf("Isi Heap setelah deleteMin: ");
    inorder(heap);
    printf("\n");

    // Bersihkan memory
    freeTree(heap);
    return 0;
}
