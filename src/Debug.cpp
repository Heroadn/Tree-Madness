// Database_hello.cpp: define o ponto de entrada para o aplicativo.
//
#include <iostream>
#include <string>
#include <string_view>
#include "Tree.hpp"
#include "SymbolTable.hpp"

int main()
{/*
    Tree tree;

    tree.insert(30, "A");
    tree.insert(10, "A");
    tree.insert(50, "A");
    tree.insert(6, "A");
    tree.insert(18, "A");
    tree.insert(45, "A");
    tree.insert(58, "A");
    tree.insert(5, "A");
    tree.insert(8, "A");
    tree.insert(15, "A");
    tree.insert(47, "A");
    tree.insert(65, "A");
    tree.insert(7, "A");
    tree.insert(9, "A");
    tree.remove(10);

    //
    Tree treeA;
    treeA.insert(20, "A");
    treeA.insert(8, "B");
    treeA.insert(22, "C");
    treeA.insert(30, "D");
    treeA.insert(12, "E");
    treeA.insert(4, "F");
    treeA.insert(10, "G");
    treeA.insert(14, "H");
   
    
    treeA.remove(8);
    treeA.remove(10);
    treeA.remove(14);
    treeA.remove(20);*/

    Tree treeB;
    /*
    treeB.insert(40, "A");
    treeB.insert(30, "B");
    //treeB.insert(50, "C");
    treeB.insert(20, "D");
    //treeB.insert(60, "E");
    treeB.insert(10, "F");
    treeB.insert(9, "F");*/

    treeB.insert(40, "A");
    treeB.insert(30, "B");
    treeB.insert(20, "D");
    treeB.insert(10, "F");
    treeB.insert(9, "F");
    treeB.print();

    //left rotation
    //treeB.insert(10, "A");
    //treeB.insert(9, "B");
    //treeB.insert(8, "C");

    //double right
    //treeB.insert(11, "A");
    //treeB.insert(4, "B");
    //treeB.insert(8, "B");

    //
    //tree.print();
    //treeA.print();
    //treeB.print();
    
    /*
    std::cout << "VALUE(45): " << tree.search(45).value_or("NOT FOUND") << std::endl;
    std::cout << "VALUE(15): " << tree.search(15).value_or("NOT FOUND") << std::endl;
    std::cout << "VALUE(79): " << tree.search(79).value_or("NOT FOUND") << std::endl;
    std::cout << "VALUE(90): " << tree.search(90).value_or("NOT FOUND") << std::endl;
    std::cout << "VALUE(10): " << tree.search(10).value_or("NOT FOUND") << std::endl;
    std::cout << "VALUE(55): " << tree.search(55).value_or("NOT FOUND") << std::endl;
    std::cout << "VALUE(12): " << tree.search(12).value_or("NOT FOUND") << std::endl;
    std::cout << "VALUE(20): " << tree.search(20).value_or("NOT FOUND") << std::endl;
    std::cout << "VALUE(50): " << tree.search(50).value_or("NOT FOUND") << std::endl;*/
    /*
    //
    std::cout << "VALUE(2): "  << tree.search(2).value_or("NOT FOUND") << std::endl;
    std::cout << "VALUE(4): "  << tree.search(4).value_or("NOT FOUND") << std::endl;
    std::cout << "VALUE(15): " << tree.search(15).value_or("NOT FOUND") << std::endl;
    std::cout << "VALUE(17): " << tree.search(17).value_or("NOT FOUND") << std::endl;
    std::cout << "VALUE(20): " << tree.search(20).value_or("NOT FOUND") << std::endl;
    std::cout << "VALUE(79): " << tree.search(79).value_or("NOT FOUND") << std::endl;

    //
    SymbolTable symbolTable;
    symbolTable.put("A", "1");
    symbolTable.put("B", "2");
    symbolTable.put("C", "3");
    symbolTable.put("D", "4");

    std::cout << "KEY(A): " << symbolTable.get("A").value_or("NOT FOUND") << std::endl;
    std::cout << "KEY(B): " << symbolTable.get("B").value_or("NOT FOUND") << std::endl;
    std::cout << "KEY(C): " << symbolTable.get("C").value_or("NOT FOUND") << std::endl;
    std::cout << "KEY(D): " << symbolTable.get("D").value_or("NOT FOUND") << std::endl;*/
    return 0;
}
