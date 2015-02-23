#pragma once

#include <fstream>
#include <vector>
#include <sstream>

struct Attr {
	std::string name;
	std::string value;
};

enum Type {
	UNI, FATHER, CLOSE, EMPTY
};

struct Node {
	Node();
	Node(const Node & node);

	std::string m_name;
	std::vector<Attr> m_attr;
	std::vector<Node*> m_childs;
	Node * father;
	Type type;

	/*
	\brief Recupere la valeur d'un attribut
	\param const char * le nom de l'attribut
	\return std::string la valeur
	*/
	std::string get_Value(const char * name) const;

	/*
	\brief CHerche le fils ayant le bon nom
	\brief si aucun noeud n'ets trouve on renvoi le oeud courant
	\param const char * le nom du  noeud
	\return Node & le noeud
	*/
	const Node & getChild(const char * name);


	const Node & operator=(const Node & node);
};


class XML {
public:

	/*
	\brief Charge un fichier de type xml
	\brief il cree tout les noeuds
	\param const char * le nom du fichier
	\return void
	*/
	void load(const char * name);

	/*
	\brief Renvoi le premier noeud ayant le nom specifie
	\param const char * name
	\return Node le noeud conrrespondant au critere
	*/
	const Node & getNode(const char * name);


	/*
	\brief Renvoi le premier noeud ayant le nom specifie
	\param const char * name
	\return Node * le noeud conrrespondant au critere
	*/
	Node * getNodePtr(const char * name);


	/*
	\brief Ecris les node dans un fichier
	\param const char * le nom du fichier
	\return void
	*/
	void write(const char * name);


	/*
		@brief Ajoute un noeud 
		@param Node & le noeud a ajouter
		@param const char * le pere
		@return void
	*/
	void addNode(Node & node, const char * parent);


	/*
	\brief Supprime tout les noeuds
	*/
	void clear();

	/*
	\brief supprime tout les noeuds
	*/
	~XML();

private:


	/*
	\brief charge tout le fichier XML dans les noeuds
	\param FILE * le fichier a charger
	\return void
	*/
	void read_file(FILE * file);


	/*
	\brief lis un noeud et le renvoi sous forme de chaine
	\param FILE * le fichier en train d'etre lu
	\return string
	*/
	std::string get_nextNode(FILE * file);

	/*
	\brief Analyse le contenu d'un noeud et le transfere dans un Node
	\param std::string & la chaine a analyser
	\return Node le noeud
	*/
	Node analyse_Node(std::string & line);

	/*
	\brief Lis les attribut dans un string
	\param std::string & si t'es pas trop con tu sais deja
	\param int la tete de lecture
	\param Node & le noeud a remplir
	*/
	void read_attributes(std::string & line, int i, Node & node);

	/*
	\brief ajoute un fils a un noeud
	\param Node * le pere
	\param Node * le fils
	\return void
	*/
	void add_Childs(Node * parent, Node * child);


	/*
		@brief Ecris un noeud dans le fichier avec tout ses fils
	*/
	void writeNode(std::ofstream & file, Node * node, std::string space = "");

private:

	std::vector<Node*> fathers;

	//pour la suppression plus simple et plus rapide
	std::vector<Node*> all;
};

