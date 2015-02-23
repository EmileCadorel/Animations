#include "XML.hh"
using namespace std;

#pragma warning(disable : 4996)

Node::Node(){}

Node::Node(const Node & node) {
	m_name = node.m_name;
	type = node.type;
	for (int i = 0; i < node.m_attr.size(); i++) {
		m_attr.push_back(node.m_attr[i]);
	}
	for (int i = 0; i < node.m_childs.size(); i++) {
		m_childs.push_back(node.m_childs[i]);
	}
}


const Node & Node::getChild(const char * name) {
	int find = -1, i = 0;
	while (i < m_childs.size() && find == -1) {
		if (m_childs[i]->m_name == name) {
			find = i;
		}
		i++;
	}
	if (find != -1) {
		return *m_childs[find];
	}
	return *this;
}


string Node::get_Value(const char * name) const {
	int i = 0, find = -1;
	while (find == -1 && i < m_attr.size()) {
		if (m_attr[i].name == name) {
			find = i;
		}
		i++;
	}
	if (find != -1) {
		return m_attr[find].value;
	}
	return string("");
}

const Node & Node::operator=(const Node & node) {
	m_name = node.m_name;
	type = node.type;
	for (int i = 0; i < node.m_attr.size(); i++) {
		m_attr.push_back(node.m_attr[i]);
	}
	for (int i = 0; i < node.m_childs.size(); i++) {
		m_childs.push_back(node.m_childs[i]);
	}
	return node;
}





void XML::load(const char * name) {
	FILE * file = fopen(name, "r");
	read_file(file);
	fclose(file);
}


void XML::read_file(FILE * file) {
	string line = "a";
	Node * current = NULL;
	while (line != "") {
		line = get_nextNode(file);
		if (line == "") {
			break;
		}
		Node node = analyse_Node(line);
		if (node.type != EMPTY) {
			if (current == NULL) {
				current = new Node();
				*current = node;
				fathers.push_back(current);
				all.push_back(current);
			}
			else {
				if (node.type == CLOSE) {
					if (current != NULL && current->type == FATHER) {
						current = current->father;
					}
					else if (current != NULL && current->father != NULL) {
						current = current->father->father;
					}
					else {
						current = NULL;
					}
				}
				else {
					if (current->type == FATHER) {
						add_Childs(current, new Node(node));
						current = current->m_childs.back();
					}
					else {
						current = current->father;
						Node * aux = new Node(node);
						add_Childs(current, aux);
						current = aux;
					}
				}
			}
		}
	}
}


std::string XML::get_nextNode(FILE * file) {
	if (file != NULL) {
		char line[256];
		line[0] = '\0';
		fgets(line, 255, file);
		string l = line;
		return l.substr(0, l.length() - 1);
	}
	return string("");
}


const Node & XML::getNode(const char * name) {
	int i = 0, find = -1;
	while (i < all.size() && find == -1) {
		if (all[i]->m_name == name) {
			find = i;
		}
		i++;
	}
	if (find != -1) {
		return *all[find];
	}
	return *all[0];
}



Node * XML::getNodePtr(const char * name) {
	int i = 0, find = -1;
	while (i < all.size() && find == -1) {
		if (all[i]->m_name == name) {
			find = i;
		}
		i++;
	}
	if (find != -1) {
		return all[find];
	}
	return all[0];
}


Node XML::analyse_Node(std::string & line) {
	bool end = false;
	Node node;
	node.father = NULL;
	int i = 0;
	while (!end && i < line.length()) {
		if (node.m_name == "" && line[i] == '<') {
			int j = i + 1;
			if (j < line.size() && line[j] == '?') {
				//ligne de commentaire
				break;
			}
			if (j < line.size() && line[j] == '/') {
				node.type = CLOSE;
				j++;
			}

			while (j < line.size() && line[j] != ' ' && line[j] != '/' && line[j] != '>') {
				node.m_name += line[j];
				j++;
			}
			i = j;
		}
		else if (node.m_name != "") {
			read_attributes(line, i, node);
			end = true;
		}
		i++;
	}
	if (node.m_name == "") {
		node.type = EMPTY;
	}
	else if (node.type != CLOSE && node.type != UNI) {
		node.type = FATHER;
	}
	return node;
}


void XML::read_attributes(string & line, int i, Node & node) {
	if (line.size() > i) {
		while (i < line.size() && line[i] == ' ') i++;
		while (i < line.size() && line[i] != '/' && line[i] != '>') {
			while (i < line.size() && line[i] == ' ') i++;
			if (i < line.size() && line[i] != '/' && line[i] != '>') {
				node.m_attr.push_back(Attr());
				while (i < line.size() && line[i] != '=' && line[i] != '/' && line[i] != '>') {
					node.m_attr.back().name += line[i];
					i++;
				}
				i+=2;
				while (i < line.size() && line[i] != ' ' && line[i] != '"' && line[i] != '>') {
					node.m_attr.back().value += line[i];
					i++;
				}
			}
			i++;
		}
		if (line[line.length() - 2] == '/') {
			node.type = UNI;
		}
	}
}

void XML::add_Childs(Node * parent, Node * child) {
	if (parent != NULL && child != NULL) {
		parent->m_childs.push_back(child);
		parent->type = FATHER;
		all.push_back(child);
		child->father = parent;
	}
	else if (parent == NULL && child != NULL) {
		fathers.push_back(child);
		child->father = NULL;
		all.push_back(child);
	}
}


void XML::addNode(Node & node, const char * parent) {
	if (parent != NULL) {
		Node * p = getNodePtr(parent);
		add_Childs(p, new Node(node));
	}
	else {
		add_Childs(NULL, new Node(node));
	}
}


void XML::write(const char * name) {
	ofstream file(name, ios::trunc);
	for (int i = 0; i < fathers.size(); i++) {
		writeNode(file, fathers[i]);
	}
	file.close();
}



void XML::writeNode(ofstream & file, Node * node, string space) {
	file << space << "<" << node->m_name << "";
	for (int i = 0; i < node->m_attr.size(); i++) {
		file << " " << node->m_attr[i].name << "=\"" << node->m_attr[i].value << "\"";
	}
	if (node->type == FATHER) {
		file << ">\n";
		for (int i = 0; i < node->m_childs.size(); i++) {
			writeNode(file, node->m_childs[i], space + "  ");
		}
		file << space << "</" << node->m_name << ">\n";
	}
	else {
		file << "/>\n";
	}
}

void XML::clear() {
	for (int i = 0; i < all.size(); i++) {
		if (all[i] != NULL) {
			delete all[i];
		}
	}
	all.clear();
	fathers.clear();
}


XML::~XML() {
	for (int i = 0; i < all.size(); i++) {
		if (all[i] != NULL) {
			delete all[i];
		}
	}
}
