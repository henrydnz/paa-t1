#include "arquivoManager.hpp"
#include "huffman.hpp"

static void writeNodeHelper(ofstream& outFile, Node* node, int& pos){
    if(!node) return;

    FileNode filenode;
    filenode.right = -1;
    filenode.left = -1;
    
    if(node->right){
        writeNodeHelper(outFile, node->right, pos);
        filenode.right = pos-1;
    }
    if(node->left){
        writeNodeHelper(outFile, node->left, pos);
        filenode.left = pos-1;
    }

    filenode.token = node->token;

    outFile.seekp(sizeof(Header) + sizeof(FileNode) * pos, ios::beg);
    
    pos++;
}

void writeTree(string filename, HuffmanTree tree){
    ofstream outFile(filename, ios::binary);

    if(!outFile){
        cerr << "O arquivo " << filename << " nao pode ser aberto" << endl;
        return;
    }
    
    Header last_node_pos = tree.nodeAmount - 1;
    outFile.write(reinterpret_cast<const char*>(&last_node_pos), sizeof(Header));
    
    int pos = 0;
    writeNodeHelper(outFile, tree.root, pos);

    outFile.close();
}