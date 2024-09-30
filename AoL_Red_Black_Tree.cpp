#include<stdio.h>
#include<stdlib.h>

/*
Anggota:
	2602094080 - Flavia Louis
	2602097076 - Leonardo Dahendra
	2602097492 - Evelyn Angelica
	2602101514 - Valentcia Angelica
	2602101653 - Andrew Alfonso Lie
	2602172220 - Tiara Intan Kusuma
*/

//untuk Red Black Tree jika dimasukkan angka yang sama maka akan dimasukkan kesebelah kanan 

struct Node{
	//structnya ditambah variabel red untuk mengetahui apakah node red atau tidak
	//0 = black, 1 = red
	int val, red;
	//ditambah pointer yang menuju ke parent dari node agar node bisa mengakses parentnya
	struct Node *left, *right, *parent;
}*root;

//constructor untuk mempermudah pembuatan node baru
struct Node *createNode(int val){
	struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->val = val;
	//setiap node baru secara default berwarna merah
	newNode->red = 1;
	newNode->left = newNode->right = newNode->parent = NULL;
	
	return newNode;
}

//mengubah warna dari sebuah node, jika node merah maka akan menjadi hitam, jika node hitam maka akan menjadi merah
void recolor(struct Node *curRoot){
	if (!curRoot) return;
	//menggunakan ternary operator, jika node berwarna merah, maka diubah menjadi hitam dan jika hitam diubah menjadi merah
	curRoot->red = curRoot->red ? 0 : 1;
	//jika node yang dimasukkan adalah root, maka warnanya akan langsung diubah menjadi hitam karena root selalu berwarna hitam
	if (curRoot == root) curRoot->red = 0;
}

//fungsi untuk print nilai dari data secara inorder, sesuai dengan output di soal nilai yang diprint hanyalah value dari node
void inorder(struct Node *curRoot){
	if (curRoot){
		inorder(curRoot->left);
		printf("%d ", curRoot->val);
		inorder(curRoot->right);
	}
}

//fungsi untuk merotate sebuah node kearah kiri
void rotateLeft(struct Node *curRoot){
	//anak kanan dari node, untuk mempermudah node ini akan direfer dalam comment sebagai anak besar
	struct Node *child = curRoot->right;
	//anak kiri dari anak kanan, untuk mempermudah node ini akan direfer dalam comment sebagai anak kecil
	struct Node *leftOfChild = child->left;
	
	//merubah agar node kiri dari anak besar menjadi node yang ingin dirotate
	child->left = curRoot;
	//mengubah agar kanan dari node yang ingin dirotate diubah menjadi anak kecil
	curRoot->right = leftOfChild;
	
	//mengubah parent dari anak kecil menjadi node yang ingin dirotate, terdapat kemungkinan anak kecil tidak ada
	//maka dicek dulu jika tidak maka akan menyebabkan eror
	if (leftOfChild) leftOfChild->parent = curRoot;
	//mengubah parent dari anak besar menjadi parent dari node yang ingin dirotate
	child->parent = curRoot->parent;
	//jika node yang ingin dirotate mempunyai parent(bukan root), maka ubah anak kiri/kanan dari parentnya menjadi anak besar
	if (curRoot->parent){
		curRoot->parent->left == curRoot ? (child->parent->left = child) : (child->parent->right = child);
	}
	//jika tidak ada parent(root), maka ubah anak besar menjadi root yang baru
	else {
		root = child;
		child->parent = NULL;
		root->red = 0;
	}
	
	//wajib disimpan dibawah if curRoot->parent
	curRoot->parent = child;
}

void rotateRight(struct Node *curRoot){
	//untuk right rotate hampir sama persis dengan left rotate, namun node dirotasi kesebelah kanan
	struct Node *child = curRoot->left;
	struct Node *rightOfChild = child->right;
	
	child->right = curRoot;
	curRoot->left = rightOfChild;
	
	if (rightOfChild) rightOfChild->parent = curRoot;
	child->parent = curRoot->parent;
	if (curRoot->parent){
		curRoot->parent->left == curRoot ? (child->parent->left = child) : (child->parent->right = child);
	}
	else {
		root = child;
		child->parent = NULL;
		root->red = 0;
	}	
	
	curRoot->parent = child;
}

//fungsi untuk rebalance treenya jika terdapat violation
void rebalance(struct Node *newNode){
	//jika node yang dimasukkan tidak ada, maka langsung keluar dari fungsi
	if (!newNode) return;
	//violation, violation terjadi ketika node merah mempunyai parent yang merah
	if (newNode->red && newNode->parent && newNode->parent->red){
		//menyimpan parent, grandparent, dan uncle dalam sebuah struct untuk mempermudah penulisan
		struct Node *parent = newNode->parent;
		struct Node *grandparent = parent->parent;
		struct Node *uncle = grandparent->right;
		
		//secara default uncle diasumsi berada disebelah kanan, namun dicek kembali dan diubah menjadi sebelah kiri jika ditemukan memang disebelah kiri
		if (grandparent->right == parent) uncle = grandparent->left;
		
		//jika uncle juga merah, maka hanya mengubah warna dari parent uncle dan grandparent
		if (uncle && uncle->red){
			recolor(parent);
			recolor(uncle);
			recolor(grandparent);
			//terdapat kemungkinan setelah warna diubah, terdapat violation lain pada grandparent, maka dicek kembali
			rebalance(grandparent);		
		}
		//jika uncle hitam atau tidak terdapat uncle
		else{
			//RR dan RL
			if (grandparent->right == parent){
				//jika RL, maka kondisi ini ke run, jika RR, maka hanya kode dibwh kondisi yang dirun
				if (parent->left == newNode) rotateRight(parent);
				rotateLeft(grandparent);
			}
			//LL dan LR
			else if (grandparent->left == parent){
				//jika LR, ini di run, jika LL maka hanya kode dibwh kondisi
				if (parent->right == newNode) rotateLeft(parent);
				rotateRight(grandparent);
			}
			//mengubah warna grandparent dan parent dari grandparent, setelah rotasi, grandparent pasti akan turun dan akan mempunyai parent
			recolor(grandparent);
			recolor(grandparent->parent);
			
			//setelah rotasi dan mengubah warna, terdapat kemungkinan tree masih unbalanced maka perlu dicek kembali
			rebalance(grandparent->parent);
		}
	}
}

//fungsi insert seperti pada umumnya
struct Node *insert(struct Node *curRoot, struct Node *newNode){
	if (!curRoot) return newNode;
	//pada fungsi insert ini terdapat 1 tambahan, yaitu set parent dari newnode menjadi curRoot
	newNode->parent = curRoot;
	if (newNode->val < curRoot->val) curRoot->left = insert(curRoot->left, newNode);
	//jika terdapat nilai yang sama, dianggap ke kanan, mengikuti visualisasi red black tree yang terdapat di internet
	else curRoot->right = insert(curRoot->right, newNode);
	
	return curRoot;
}

//digunakan untuk mempermudah insert data karena user tidak perlu melakukan root = ...
//dan juga digunakan untuk mereset warna root dan rebalance treenya sesuai aturan rb tree
void insertWrapper(int val){
	struct Node *newNode = createNode(val);
	root = insert(root, newNode);
	//memastikan root berwarna hitam
	root->red = 0;
	//melakukan rebalance pada node baru jika ditemukan violation
	rebalance(newNode);
}

int main(){
	int nums[] = {41, 22, 5, 51, 48, 29, 18, 21, 45, 3};
	for (int i = 0; i < sizeof(nums) / sizeof(nums[0]); i++) insertWrapper(nums[i]);
	printf("Inoder Traversal of Created Tree\n");
	inorder(root);
	printf("\n");
	return 0;
}
