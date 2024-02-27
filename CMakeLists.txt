# CMakeList.txt: projeto de CMake para Database_hello, incluir origem e definir
# lógica específica a um projeto aqui.
#
cmake_minimum_required (VERSION 3.8)
project (
	"Database_hello"
	LANGUAGES CXX C
    VERSION 0.0.0.0)

#
set(CMAKE_CXX_STANDARD 20)

# Adicione a origem ao executável deste projeto.
add_executable (BinarySearchTree
	"src/Debug.cpp" "src/Tree.hpp" "src/BinarySearchTree.cpp"   "src/SymbolTable.hpp" "src/SymbolTable.cpp")

add_executable (BalancedBinaryTree
	"src/Debug.cpp" "src/Tree.hpp" "src/BalancedBinaryTree.cpp" "src/SymbolTable.hpp" "src/SymbolTable.cpp")

# TODO: Adicione testes e instale destinos, se necessário.
