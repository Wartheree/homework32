//
// Created by danil on 21.10.2025.
//
//программе обязетельно нужно принять на вход -t, -c и -e параметры
//
//параметр -t отвечает за тип связанного списка
//1 - однонаправленный связанный список
//2 - двунаправленный связный список
//3 - однонаправленный кольцевой связный список
//4 - двунаправленный кольцевой связный список
//
//параметр -c отвечает за изначальное количество элементов в списке
//если список изначально не имеет элементов, параметр -c должен равняться 0
//
//параметр -e принимает изначальные элементы связанного списка
//они должны указываться через запятую без пробелов
//если список изначально не имеет элементов, параметр -e должен присутствовать, но после него ничего не должно быть
//например: "-t 1 -c 0 -e"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdint>
#include <cmath>

struct Node {
    int data;
    std::vector<struct Node*> links;
};

void add_elem(Node*& head, Node*& tail, const int& k, const int& type) {
    Node* new_node = new Node{k,{nullptr}};
    if (type==2 || type==4) new_node->links.push_back(nullptr);

    if (head==nullptr) {
        tail=new_node;
        head=tail;
        if (type==3) tail->links[0]=head;
        if (type==4) {tail->links[0]=head; tail->links[1]=head;};
        return;
    }
    switch (type) {
        case 1: {
            tail->links[0]=new_node;
            tail=tail->links[0];
            break;
        }
        case 2: {
            tail->links[0]=new_node;
            new_node->links[1]=tail;
            tail=tail->links[0];
            break;
        }
        case 3: {
            tail->links[0]=new_node;
            new_node->links[0]=head;
            tail=tail->links[0];
            break;
        }
        case 4: {
            tail->links[0]=new_node;
            new_node->links[0]=head;
            new_node->links[1]=tail;
            head->links[1]=new_node;
            tail=tail->links[0];
            break;
        }
    }

}

void print(Node*& head, const uint8_t& type) {
    Node* current =head;
    while (current!=nullptr) {
        std::cout << current->data;
        current = current->links[0];
        if (current==head) break;
        switch (type) {
            case 1: if (current!=nullptr) std::cout << " > "; break;
            case 2: if (current!=nullptr) std::cout << " <> "; break;
            case 3: if (current!=nullptr) std::cout << " >0 "; break;
            case 4: if (current!=nullptr) std::cout << " <>0 ";
        }
    }
    std::cout << std::endl;
}

bool find (Node*& head, const int& data, const uint8_t& type) {
    Node* current = head;
    int length=0;
    while (current!=nullptr){
        length++;
        current = current->links[0];
        if (current==head) break;
    }
    current = head;
    int pos=0;
    bool found=false;
    while (current!=nullptr) {
        if (current->data == data) {
            if (type<=2) std::cout << pos<< " ";
            else std::cout << pos<< " + " <<length<< "*n, ";
            found=true;
        }
        current = current->links[0];
        pos++;
        if (current==head) break;
    }
    if (!found) return false;
    if (type>=3) std::cout <<"where n is any integer";
    std::cout << std::endl; return true;

}
bool find_with_no_out (Node*& head, const int& data) {
    Node* current = head;
    bool found=false;
    while (current!=nullptr) {
        if (current->data == data) {
            found=true;
        }
        current = current->links[0];
        if (current==head) break;
    }
    if (!found) return false;
    return true;

}
bool delete_element(Node*& head, Node*&tail, int& data, uint8_t& type) {
    bool result=find_with_no_out(head,data);
    if (result==false) return false;
    Node* current=head;
    switch (type) {
        case 1: {
            if (current->data==data) {
                if (head->links[0]==nullptr) {head=nullptr; return true;}
                else {
                    head=head->links[0];
                    delete current;
                    return true;
                }
            }

            while (current->links[0]!=nullptr) {
                if (current->links[0]->data==data) {
                    Node* temp=current->links[0];
                    current->links[0]=temp->links[0];
                    if (temp==tail) {tail=current; tail->links[0]=nullptr;};
                    delete temp;
                    return true;
                }
                current = current->links[0];
            }
            return true;
        }
        case 2: {
            if (current->data==data) {
                if (head->links[0]==nullptr) {head=nullptr; return true;}
                else {
                    head=head->links[0];
                    head->links[1]=nullptr;
                    delete current;
                    return true;
                }
            }
            while (current!=nullptr) {
                if (current->data==data) {
                    if (current==tail) {tail=current->links[1]; tail->links[0]=nullptr;}
                    else {
                        current->links[0]->links[1]=current->links[1];
                        current->links[1]->links[0]=current->links[0];
                    };
                    delete current;
                    return true;
                }
                current = current->links[0];
            }
            return true;
        }

        case 3: {
            if (current->data==data) {
                if (head->links[0]==nullptr || head->links[0]==head) {head=nullptr; return true;}
                else {
                    head=head->links[0];
                    tail->links[0]=head;
                    delete current;
                    return true;
                }
            }
            while (current->links[0]!=head) {
                if (current->links[0]->data==data) {
                    Node* temp=current->links[0];
                    if (temp==tail) {tail=current; tail->links[0]=head;}
                    else current->links[0]=temp->links[0];
                    delete temp;
                    return true;
                }
                current = current->links[0];
            }
            return true;
        }
        case 4: {
            if (current->data==data) {
                if (head->links[0]==nullptr || head->links[0]==head ) {head=nullptr; return true;}
                else {
                    head=head->links[0];
                    head->links[1]=tail;
                    tail->links[0]=head;
                    delete current;
                    return true;
                }
            }
            while (current!=nullptr) {
                if (current->data==data) {
                    if (current==tail) {tail=current->links[1]; tail->links[0]=head;}
                    else {
                        current->links[0]->links[1]=current->links[1];
                        current->links[1]->links[0]=current->links[0];
                    };
                    delete current;
                    return true;
                }
                current = current->links[0];
            }
            return true;
        }
    }
}

bool delete_node(Node*&head, Node*&tail) {
    if (head==nullptr) return false;
    Node* current=head;
    while (current!=nullptr) {
        Node* temp=current->links[0];
        delete current;
        current=temp;
        if (current==head) break;
    }
    head=nullptr;
    tail=nullptr;
    return true;
}

void node_to_file(Node*& head, const uint8_t& type, const std::string& filename) {
    if (head==nullptr) {std::cout<<"node not found"<<std::endl; return;};
    std::ofstream fout(filename);
    Node* current = head;
    while (current!=nullptr) {
        fout << current->data;
        current = current->links[0];
        if (current==head) break;
        switch (type) {
            case 1: if (current!=nullptr) fout << " > "; break;
            case 2: if (current!=nullptr) fout << " <> "; break;
            case 3: if (current!=nullptr) fout << " >0 "; break;
            case 4: if (current!=nullptr) fout << " <>0 ";
        }
    }
    std::cout<<"node successfully transferred to file"<<std::endl;
    fout.close();
}
bool file_to_node(Node*& head, Node*&tail, const std::string &filename, uint8_t& type) {
    std::ifstream fin(filename);
    std::string line;
    std::getline(fin,line);
    std::string data;
    std::string save_data;
    if (line.length()<5) return false;
    for (int i=0;i<line.length();i++) {
        if (line[i]=='0' || line[i]=='1' || line[i]=='2' || line[i]=='3' || line[i]=='4' || line[i]=='5' || line[i]=='6' || line[i]=='7' || line[i]=='8' || line[i]=='9') {
            data+=line[i];
        }
        else if (line[i]=='<') {
            if (line[i+1]=='>' && line[i+2]=='0') {
                if (type==0) {type=4; add_elem(head,tail, std::stoi(save_data), type);}
                else if (type!=4) {if (head != nullptr) delete_node(head,tail); return false;}
                i+=2;
            }
            else if (line[i+1]=='>' && line[i+2]==' ') {
                if (type==0) {type=2; add_elem(head,tail, std::stoi(save_data), type);}
                else if (type!=2) {if (head != nullptr) delete_node(head,tail); return false;}
                i+=1;
            }
        }
        else if (line[i]=='>') {
            if (line[i+1]=='0') {
                if (type==0) {type=3; add_elem(head,tail, std::stoi(save_data), type);}
                else if (type!=3) {if (head != nullptr) delete_node(head,tail); return false;}
                i+=1;
            }
            else if (line[i+1]==' ') {
                if (type==0) {type=1; add_elem(head,tail, std::stoi(save_data), type);}
                else if (type!=1) {if (head != nullptr) delete_node(head,tail); return false;}
            }
        }
        else if (line[i]==' ') {
            if (!data.empty()) {
                if (type==0) {
                    save_data=data;
                    data="";
                }
                else {
                    add_elem(head,tail, std::stoi(data), type);
                    data="";
                }
            }
        }
        else {if (head != nullptr) delete_node(head,tail); return false;}
        if (i==line.length()-1) {
            if (!data.empty() && type!=0) {
                add_elem(head,tail, std::stoi(data), type);
                data="";
            }
            else {if (head != nullptr) delete_node(head,tail); return false;}
        }
    }
    return true;
}

int main(int argc, char * argv[]) {
    if (argc>7) {std::cout<<"Wrong arguments are given";return 1;}
    if ( argv[1][0] != '-' || argv[1][1] != 't' || argv[1][2] != '\0' || argv[2][1]!='\0') {std::cout<<"Wrong arguments are given";return 1;}
    if (argv[2][0]!='1' && argv[2][0]!='2' && argv[2][0]!='3' && argv[2][0]!='4') {std::cout<<"Wrong arguments are given";return 1;}
    uint8_t node_type = *argv[2] - '0';
    if ( argv[3][0] != '-' || argv[3][1] != 'c' || argv[3][2] != '\0') {std::cout<<"Wrong arguments are given";return 1;}
    int* length = new int;
    int* length_length= new int;
    *length=0;
    *length_length=0;
    for (int i = 0; argv[4][i]!='\0'; i++) (*length_length)++;
    for (int i = 0; argv[4][i]!='\0'; i++) *length += (argv[4][i] - '0') * pow(10, *length_length - 1 - i);
    delete length_length;
    if ( argv[5][0] != '-' || argv[5][1] != 'e' || argv[5][2] != '\0') {std::cout<<"Wrong arguments are given"; delete length; return 1;}
    Node* head = nullptr;
    Node* tail = nullptr;
    if (argc==6 && *length!=0) {std::cout<<"The -c argument doesn't match -e argument "; delete length; return 1;}
    else if (argc==7) {
        char* el = argv[6];
        std::vector<int>* elements = new std::vector<int>;
        for (int i=0;true;i++) {
            if (el[i]!=',') {
                (*elements).push_back(el[i]-'0');
                if (el[i+1]!=',') break;
            }
        }
        delete el;
        if ((*elements).size()!=*length) {std::cout<<"The -c argument doesn't match -e argument "; delete length; delete elements; return 1;}
        for (int i=0;i<*length;i++) {
            add_elem(head,tail,(*elements)[i], node_type);
        }
        delete elements;
    }
    delete length;
    int n=0;
    std::string filename;
    std::cout<<"type a number of desired command"<<std::endl;
    std::cout<<"1 - add element to the node"<<std::endl;
    std::cout<<"2 - print the node"<<std::endl;
    std::cout<<"3 - search for element in the node"<<std::endl;
    std::cout<<"4 - delete the element"<<std::endl;
    std::cout<<"5 - delete the node"<<std::endl;
    std::cout<<"6 - write the node to a file"<<std::endl;
    std::cout<<"7 - read the node from a file. File should be as follows: <data><space><type_of_node><space><data>... (e.g '3 <> 5 <> 1')"<<std::endl;
    std::cout<<"0 - exit the program"<<std::endl;
    do {
        std::cout<<"command number: ";
        std::cin >> n;
        switch (n) {
            case 1: {
                int k = 0;
                std::cout<<"input data: ";
                std::cin>>k;
                add_elem(head,tail,k,node_type);
                break;
            }
            case 2: {
                print(head,node_type);
                break;
            }
            case 3: {
                int f = 0;
                std::cout<<"sought data: ";
                std::cin>>f;
                if (find(head,f,node_type)) std::cout<<"found"<<std::endl;
                else std::cout<<"not found"<<std::endl;
                break;
            }
            case 4: {
                int d = 0;
                std::cout<<"data to delete: ";
                std::cin>>d;
                if (!find_with_no_out(head,d)) std::cout<<"data not found"<<std::endl;
                else {
                    short deletion_type=0;
                    std::cout<<"1-deletes the first found element, 2-deletes every element that contains data"<<std::endl;
                    std::cout<<"pick a type of deletion: "; std::cin>>deletion_type;
                    if (deletion_type==1) {
                        delete_element(head,tail,d,node_type);
                    }
                    else if (deletion_type==2) {
                        while (find_with_no_out(head,d)) delete_element(head,tail,d,node_type);
                    }
                    else {std::cout<<"wrong command number"<<std::endl; break;};
                    std::cout<<"data successfully deleted"<<std::endl;
                }
                break;
            }
            case 5: {
                if (delete_node(head,tail)) std::cout<<"node successfully deleted"<<std::endl;
                else std::cout<<"node not found"<<std::endl;
                break;
            }
            case 6: {
                std::getline(std::cin, filename);
                std::cout<<"input filename: "; std::getline(std::cin, filename);
                node_to_file(head,node_type,filename);
                break;
            }
            case 7: {
                if (head!=nullptr) std::cout<<"node already exists, can not read another one"<<std::endl;
                else {
                    std::getline(std::cin, filename);
                    std::cout<<"input filename: "; std::getline(std::cin, filename);
                    node_type=0;
                    if (file_to_node(head,tail,filename,node_type)) std::cout<<"node successfully read"<<std::endl;
                    else std::cout<<"an error occurred"<<std::endl;
                }
                break;
            }
            case 0: {
                std::cout<<"thank you, have a nice day!"; break;
            }
            default: {
                std::cout<<"wrong command number"<<std::endl;
            }
        }
    }while (n!=0);
}
