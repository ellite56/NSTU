#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
using namespace std;

void Show_text()  //������� ��������� ���������� ����� (8)
{
    int lines = 0;//��� �����
    string str;
    ifstream file_txt("Text_file.txt");  //��������� ��������� ���� ��� ������
    cout << "����� �� ���������� �����:" << endl << endl;
    if (file_txt.is_open())
    {
        while (getline(file_txt, str))//��������� ���� ������ �  ���������  �� � ���
        {
            lines++;
            cout << lines << " | " << str << endl;  //������� ������
        }
    }

    file_txt.close();
}

long Size_of_string_in_bytes(const string& str)  //������ ������ � ������
{
    return sizeof(str[0]) * str.size();//������ ������ ������� �������� �� ���
};

class List
{

private:
    long nextp;  //��������� �� ��������� �������
    int size;  //������ ������
    string data;  //���������� ������

public:

    List() : nextp(-1), size(0) {}  //�����������

    ~List() {}  //����������

    List(const List& obj)  //������� ������
    {
        nextp = obj.nextp;  //��������� �� ��������� �������
        size = obj.size;  //������ ������
        data = obj.data;  //���������� ������
    }

    friend ostream& operator<<(ostream& os, const List& node)  //��������������� �������� << ��� ������ 
    {
        os.write(reinterpret_cast<const char*>(&node.nextp), sizeof(long));//������ ���� �� �������
        os.write(reinterpret_cast<const char*>(&node.size), sizeof(long));//������
        os.write(node.data.c_str(), node.size);//������ �����������
        return os;
    }

    friend istream& operator>>(istream& is, List& node)   //��������������� �������� >> ��� ������

    {
        is.read(reinterpret_cast<char*>(&node.nextp), sizeof(long));//���������
        is.read(reinterpret_cast<char*>(&node.size), sizeof(long));//������
        string buffer;
        buffer.resize(node.size);//�������� ������ 
        is.read(&buffer[0], node.size);//������
        node.data = buffer;
        return is;
    }

    string get_data()//�������� ���������� ������
    {
        return data;
    }

    void set_data(string obj)
    {
        data = obj;
    }

    long get_size()//������
    {
        return size;
    }

    void set_size(string obj)
    {
        size = Size_of_string_in_bytes(obj);
    }

    long get_nextp()//��������  ������ � ���������
    {
        return nextp;
    }

    void set_nextp(long obj)
    {
        nextp = obj;
    }
};

class BinaryFile
{

private:
    fstream file_bin;  //�������� ����
    long headp;  //��������� �� ������ ������
    const char* filename;

public:

    BinaryFile(const char* filename) : file_bin(filename, ios::binary | ios::in | ios::out | ios::trunc)  //�����������
    {
        headp = -1;
        this->filename = filename;

        if (!file_bin.is_open())
        {
            file_bin.open(filename, ios::binary | ios::out);//��� ������
            file_bin.close();
            file_bin.open(filename, ios::binary | ios::in | ios::out);//������ � ������
        }

        file_bin.seekg(0, ios::beg);//������������� ��������� �� ������
    }

    ~BinaryFile() //����������
    {
        file_bin.close();
    }

    void Filling_binary_file() //���������� ��������� �����
    {
        string obj = "";
        ifstream file_txt("Text_file.txt");
        List node;

        if (file_txt.is_open())
        {
            file_bin.clear();//������� cc����
            while (getline(file_txt, obj))
            {
                node.set_data(obj);//��������� ������
                node.set_size(obj);//������

                if (headp != -1)//���� ������ �� ����, �� ���������� � �����
                {
                        file_bin.clear();
                        file_bin.seekg(0, std::ios::beg);//����������� ��������� �� ������ �����
                        long head = -1;//��� �������� ������� �������� 
                        file_bin.read(reinterpret_cast<char*>(&head), sizeof(long));//��������� �� ������ ������ � ����������� � head
                        file_bin.seekg(head, std::ios::beg);//���������� �� ������
                        List currentNode;
                        long prevNextPosition = file_bin.tellg();//������� ������� ��������� �� ������
                    while (!file_bin.eof() && file_bin.tellg() != -1)
                    {
                        prevNextPosition = file_bin.tellp();//��������� ������� ������� ������
                        file_bin >> currentNode;
                        file_bin.seekg(currentNode.get_nextp(), std::ios::beg);//����������
                    }
                    file_bin.clear();
                    file_bin.seekp(0, std::ios::end);//���������� � �����
                    long currentPosition = file_bin.tellp();
                    file_bin << node;//���������� ����� �������
                    file_bin.seekp(prevNextPosition, std::ios::beg);
                    file_bin.write(reinterpret_cast<const char*>(&currentPosition), sizeof(long));//���������� � ��� � �������
                }

                if (headp == -1)
                {
                    headp = 4;
                    file_bin.seekg(0, std::ios::beg);//������������� ��������� �� ������
                    file_bin.write(reinterpret_cast<const char*>(&headp), sizeof(long));//���������� � ��������
                    file_bin << node;
                }
            }
        }
        else
        {
            cout << "�� ������� ������� ��������� ����!" << endl;
            return;
        }

        cout << "����� �������� � �������� ����!" << endl;
        file_txt.close();
    }

    void Delete(int index)//�������� �� �������
    {
        file_bin.clear();  //������� ����� �� ������
        file_bin.seekg(0, ios::beg);//0 ��� ����� � beg ������
        List delete_node;

        if (index == 0)
        {   
            file_bin.clear();  //������� ����� �� ������
            file_bin.seekg(0, ios::beg); //���������� �� ������
            long head = -1;
            file_bin.read(reinterpret_cast<char*>(&head), sizeof(long));//������ �������
            file_bin.seekg(head, ios::beg); // ���������� ��������� �� ��� ������� 
            file_bin >> delete_node;//��������� ������ �� ������
            long delete_nextp = delete_node.get_nextp();
            file_bin.seekg(0, ios::beg);
            file_bin.write(reinterpret_cast<const char*>(&delete_nextp), sizeof(long));//���������� ����� �������, ��������� ����� ����������
            headp = delete_nextp;

            cout << "������ �������!" << endl;
        }

        else
        {
            file_bin.clear();  //������� ����� �� ������
            file_bin.seekg(0, ios::beg); 
            long head = -1;
            file_bin.read(reinterpret_cast<char*>(&head), sizeof(long));
            file_bin.seekg(head, ios::beg); 
            long prev_pos = head; 
            long cur_pos = head; 
            List current_node;

            for (int i = 0; i <= index; i++) // ������� �������, �������������� ������� �������
            {
                if (file_bin.eof() || file_bin.tellg() == -1)//���� ����� 
                {
                    cout << "������������ ������!" << endl;
                    return;
                }

                prev_pos = cur_pos;//������� ������� ���������
                cur_pos = file_bin.tellg();//���������� ������� ������� ��������� �� ������ � ������
                file_bin >> current_node;//��� ������ �������� ��������
                file_bin.seekg(current_node.get_nextp(), ios::beg);//����������� �� ���������
            }
                
            file_bin.clear();
            long new_lastp = current_node.get_nextp();//��������� 
            file_bin.seekp(prev_pos, ios::beg); // ������ �� ������ ����� ���������
            file_bin.write(reinterpret_cast<const char*>(&new_lastp), sizeof(long));

            cout << "������ �������!" << endl;
        }
    }

    void Place_line(int index, string obj)  //���������� ������ �� ���������� �������
    {
        file_bin.clear(); // ������� �� ������
        List node;
        node.set_data(obj); // ����� ������
        node.set_size(obj); // ����� ������ ������

        if (index < 0)
        {
            cout << "������������ ������!" << endl;
            return;
        }

        if (index == 0)
        {
            if (headp == -1)//���� ������ �� ��������� � ������
            {
                file_bin.clear();
                headp = 4;
                file_bin.seekg(0, ios::beg);
                file_bin.write(reinterpret_cast<const char*>(&headp), sizeof(long));//���������� ���������� headp
                file_bin << node;//���������� ������ ������� � ��������
            }

            else//� �����
            {
                file_bin.clear();
                file_bin.seekp(0, ios::end);
                long currentPosition = file_bin.tellp();
                node.set_nextp(headp);//������������� ��������� �� ������� �������
                file_bin << node;
                headp = currentPosition;
                file_bin.seekg(0, ios::beg);
                file_bin.write(reinterpret_cast<const char*>(&headp), sizeof(long));
            }
            cout << "������ ���������!" << endl;
        }

        else
        { //���������� � �������� 
            file_bin.clear(); // ������� ����� �� ������
            file_bin.seekg(0, ios::beg); // ��������� �� ������
            long head = -1;
            file_bin.read(reinterpret_cast<char*>(&head), sizeof(long));
            file_bin.seekg(head, ios::beg); // ���������� ��������� � ������ ������
            long prevNextPosition = head; // ������� ����� ���������� ��������
            List currentNode;

            for (int i = 0; i < index; i++) // ������� �������, �������������� ������� �������
            {
                if (file_bin.eof() || file_bin.tellg() == -1)
                {
                    cout << "������������ ������!" << endl;
                    return;
                }

                prevNextPosition = file_bin.tellp();
                file_bin >> currentNode;
                file_bin.seekg(currentNode.get_nextp(), ios::beg);
            }

            file_bin.clear();

            if (currentNode.get_nextp() == -1)
            {
                file_bin.seekp(0, ios::end);
                long currentPosition = file_bin.tellp(); // ���������� ������� �������
                file_bin << node; // ���������� �������� << ��� ������ �������� ������ � ����
                file_bin.seekp(prevNextPosition, ios::beg);//���������� ��������� �� �� ������� 
                file_bin.write(reinterpret_cast<const char*>(&currentPosition), sizeof(long));//���������� �� ������ �������
                return;
            }

            long NextPosition = file_bin.tellp(); // ������ ������ ��������������� ��������
            file_bin.seekp(0, ios::end);
            long currentPosition = file_bin.tellp(); // ���������� ������� ����� �����
            node.set_nextp(NextPosition);
            file_bin << node; // ���������� �������� << ��� ������ �������� ������ � ����
            file_bin.seekp(prevNextPosition, ios::beg);
            file_bin.write(reinterpret_cast<const char*>(&currentPosition), sizeof(long));
            cout << "������ ���������!" << endl;
        }
    }

    void Adding_string_to_the_end(string obj)  //���������� ������ � ����� �����
    {
        file_bin.clear();
        List node;
        node.set_data(obj);
        node.set_size(obj);

        if (headp != -1)
        {
            file_bin.seekg(0, ios::beg); // ���� ������ �������
            long head = -1;
            file_bin.read(reinterpret_cast<char*>(&head), sizeof(long));
            file_bin.seekg(head, ios::beg); // ���������� ��������� � ������ ������
            List currentNode;
            long prevNextPosition = -1;
            // ������� ��������� ������� � ������

            while (!file_bin.eof() && file_bin.tellg() != -1)
            {
                prevNextPosition = file_bin.tellp();
                file_bin >> currentNode;
                file_bin.seekg(currentNode.get_nextp(), ios::beg);
            }

            file_bin.clear();
            file_bin.seekp(0, ios::end);
            long currentPosition = file_bin.tellp(); // ���������� ������� �������
            file_bin << node; // ���������� �������� << ��� ������ �������� ������ � ����
            file_bin.seekp(prevNextPosition, ios::beg);
            file_bin.write(reinterpret_cast<const char*>(&currentPosition), sizeof(long));
            cout << "������ ���������!" << endl;
        }

        if (headp == -1)
        {
            headp = 4;
            file_bin.seekg(0, ios::beg);
            file_bin.write(reinterpret_cast<const char*>(&headp), sizeof(long));
            file_bin << node; // ���������� �������� << ��� ������ �������� ������ � ����
            cout << "������ ���������!" << endl;
        }
    }

    void Redactor(int index, string obj)
    {
        file_bin.clear(); // ������� �� ������
        List node;
        node.set_data(obj); // ����� ������
        node.set_size(obj); // ����� ������ ������

        if (index < 0)
        {
            cout << "������������ ������!" << endl;
            return;
        }
        if (index == 0)
        {
            List currentNode;
            file_bin.clear(); // ������� ����� �� ������
            file_bin.seekg(0, std::ios::beg); // ���� ������ �������
            long head = -1;
            file_bin.read(reinterpret_cast<char*>(&head), sizeof(long));//������ ������� ������ ���
            file_bin.seekg(head, std::ios::beg); // ���������� ��������� � ������ ������
            file_bin >> currentNode;//������ �������
            long prevNextPosition = currentNode.get_nextp();//��������� ��������� �������
            file_bin.seekp(0, std::ios::end);
            long currentPosition = file_bin.tellp(); // ���������� ������� ����� �����
            node.set_nextp(prevNextPosition);//��������� ������ �� ���� �������
            file_bin << node; // ���������� �������� << ��� ������ �������� ������ � ����
            file_bin.seekg(0, std::ios::beg);
            file_bin.write(reinterpret_cast<const char*>(&currentPosition), sizeof(long));
            cout << "������ ��������!" << endl;
        }
        else
        {
            file_bin.clear(); // ������� ����� �� ������
            file_bin.seekg(0, std::ios::beg); // ���� ������ �������
            long head = -1;
            file_bin.read(reinterpret_cast<char*>(&head), sizeof(long));
            file_bin.seekg(head, std::ios::beg); // ���������� ��������� � ������ ������

            long prevNextPosition = head; // ������� ����� ���������� �������� ��� ������������ ������� ����� ��������

            List currentNode;
            for (int i = 0; i < index; i++) // ������� �������, �������������� ������� �������
            {
                if (file_bin.eof() || file_bin.tellg() == -1)
                {
                    cout << "������������ ������!" << endl;
                    return;
                }
                prevNextPosition = file_bin.tellp();//����� �������� �����, ������� ����� ��������
                file_bin >> currentNode;
                file_bin.seekg(currentNode.get_nextp(), std::ios::beg);
            }
            file_bin.clear();
            if (currentNode.get_nextp() == -1)//���������� � �����
            {
                file_bin.seekp(0, std::ios::end);
                long currentPosition = file_bin.tellp(); // ���������� ������� �������
                file_bin << node; // ���������� �������� << ��� ������ �������� ������ � ����
                file_bin.seekp(prevNextPosition, std::ios::beg);
                file_bin.write(reinterpret_cast<const char*>(&currentPosition), sizeof(long));
                return;
            }
            //��������� ������ � ���������� � ��������
            file_bin >> currentNode;
            long NextPosition = currentNode.get_nextp(); // ������ ������ ��������������� ��������
            file_bin.seekp(0, std::ios::end);
            long currentPosition = file_bin.tellp(); // ���������� ������� ����� �����
            node.set_nextp(NextPosition);
            file_bin << node; // ���������� �������� << ��� ������ �������� ������ � ����
            file_bin.seekp(prevNextPosition, std::ios::beg);
            file_bin.write(reinterpret_cast<const char*>(&currentPosition), sizeof(long));
            cout << "������ ��������!" << endl;
        }
    }

    void View_binary_file()  //������� ��������� ��������� �����
    {
        if (headp == -1)
        {
            cout << "� ����� ��� ���������!" << endl;
            return;
        }

        int lines = 0;
        file_bin.clear(); // ������� ����� �� ������
        file_bin.seekg(0, ios::beg); // ���� ������ �������
        long head = -1;
        file_bin.read(reinterpret_cast<char*>(&head), sizeof(long));
        file_bin.seekg(head, ios::beg); // ���������� ��������� � ������ ������
        cout << "����� � �������� �����:" << endl << endl;
        while (!file_bin.eof() && file_bin.tellg() != -1)
        {
            List node;
            file_bin >> node; // ���������� �������� >> ��� ������ �������� ������ �� �����
            file_bin.seekp(node.get_nextp(), ios::beg); // ���������� ��������� �� ��������� �������

            if (!file_bin.eof())
            {
                lines++;
                cout << "Bin " << lines << " | " << node.get_data() << endl;
            }
        }

        cout << endl;
    }

    int String_in_bin()//������� �� �������� ����� � �������� �����
    {
        int lines = 0;
        file_bin.clear();
        file_bin.seekg(0, ios::beg);
        long head = -1;
        file_bin.read(reinterpret_cast<char*>(&head), sizeof(long));
        file_bin.seekg(head, ios::beg);
        while (!file_bin.eof() && file_bin.tellg() != -1)
        {
            List node;
            file_bin >> node;
            file_bin.seekp(node.get_nextp(), ios::beg);

            if (!file_bin.eof())
            {
                lines++;
            }
        }
        return lines;
    }

    void Filling_txt_file() //���������� ���������� �����
    {
        ofstream file_txt("Text_file.txt", ios::trunc);  //��������� ��������� ���� ��� ������, ������ ���� ������� ���
        if (file_txt.is_open())
        {
            if (headp == -1)
            {
                cout << "� ����� ��� ������!" << endl;
                return;
            }

            file_bin.clear(); //������� ����� �� ������
            file_bin.seekg(0, ios::beg); //������ ��������� � ������ ��������� �����
            long head = -1;
            file_bin.read(reinterpret_cast<char*>(&head), sizeof(long));  //��������� ������ ������� ������
            file_bin.seekg(head, ios::beg); //���������� ��������� � ������ ������

            while (!file_bin.eof() && file_bin.tellg() != -1)
            {
                List node;
                file_bin >> node; //���������� �������� >> ��� ������ �������� ������ �� �����
                file_bin.seekp(node.get_nextp(), std::ios::beg); // ���������� ��������� �� ��������� �������

                if (!file_bin.eof())
                {
                    string obj = node.get_data();
                    file_txt << node.get_data() << "\n";
                }
            }
        }
        else
        {
            cout << "�� ������� ������� ��������� ����!" << endl;
            return;
        }

        cout << "����� �������� � ��������� ����!" << endl;
        file_txt.close();
    }

};


void menu()
{
    cout << "������� [0] ��� ���������� ���������;" << endl;
    cout << "������� [1] ��� �������� ������ � �������� ����;" << endl;
    cout << "������� [2] ��� �������� ������;" << endl;
    cout << "������� [3] ��� ������������� ������;" << endl;
    cout << "������� [4] ��� ������� ������ �� ���������� �������;" << endl;
    cout << "������� [5] ��� ���������� ������ � �����;" << endl;
    cout << "������� [6] ��� ���������� ��������� � ��������� ����;" << endl;
    cout << "������� [7] ��� ��������� ��������� �����." << endl;
    cout << "������� [8] ��� ��������� ������." << endl;
}

int main()
{
    BinaryFile BinFile("Binary_file.bin");
    setlocale(LC_ALL, "");
    int input = 0;
    int input_line = 0;
    string new_string;
    menu();
    while (1)
    {
        cout << endl << "������� �����: ";
        cin >> input;
        cout << endl;

        if (input == 0) //����� �� ���������
        {
            break;
        }

        switch (input)
        {
        case 1:  //�������� ���������� �����
            BinFile.Filling_binary_file();
            break;

        case 2:  //�������� ������
            cout << "����� ������ ����� �������? �������� ������ �� 1 �� " << BinFile.String_in_bin() << ": ";
            cin >> input_line;
            BinFile.Delete(input_line - 1);
            break;

        case 3:  //�������������� ������
            cout << "����� ������ ����� ����������? �������� ������ �� 1 �� " << BinFile.String_in_bin() << ": ";
            cin >> input_line;
            cout << "������� ���� ������: ";
            getline(cin, new_string);
            getline(cin, new_string);//��� "/n"
            BinFile.Redactor(input_line - 1, new_string);
            cout << endl;
            break;

        case 4:  //������� ������ �� ���������� �����
            cout << "�� ����� ����� �������� ����� ������? �������� ������ �� 1 �� " << BinFile.String_in_bin() << ": ";
            cin >> input_line;
            cout << "������� ���� ������: ";
            getline(cin, new_string);
            getline(cin, new_string);
            cout << endl;
            BinFile.Place_line(input_line - 1, new_string);
            break;

        case 5:  //���������� ������ � �����
            cout << "������� ���� ������: ";
            cin >> new_string;
            BinFile.Adding_string_to_the_end(new_string);
            break;

        case 6:  //���������� ��������� � ��������� ����
            BinFile.Filling_txt_file();
            break;

        case 7:  //����� ������ ���������
            BinFile.View_binary_file();
            break;

        case 8:  //����� ������
            Show_text();
            break;

        default:
            break;
        }
    }
    return 0;
}