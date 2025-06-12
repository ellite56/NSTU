#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
using namespace std;

void Show_text()  //функция просмотра текстового файла (8)
{
    int lines = 0;//для строк
    string str;
    ifstream file_txt("Text_file.txt");  //открываем текстовый файл для вывода
    cout << "Текст из текстового файла:" << endl << endl;
    if (file_txt.is_open())
    {
        while (getline(file_txt, str))//считываем одну строку и  добавляем  ее в стр
        {
            lines++;
            cout << lines << " | " << str << endl;  //выводим строку
        }
    }

    file_txt.close();
}

long Size_of_string_in_bytes(const string& str)  //размер строки в байтах
{
    return sizeof(str[0]) * str.size();//размер одного символа умножаем на все
};

class List
{

private:
    long nextp;  //указатель на следующий элемент
    int size;  //размер строки
    string data;  //содержимое строки

public:

    List() : nextp(-1), size(0) {}  //конструктор

    ~List() {}  //деструктор

    List(const List& obj)  //объекты списка
    {
        nextp = obj.nextp;  //указатель на следующий элемент
        size = obj.size;  //размер строки
        data = obj.data;  //содержимое строки
    }

    friend ostream& operator<<(ostream& os, const List& node)  //перезагруженный оператор << для записи 
    {
        os.write(reinterpret_cast<const char*>(&node.nextp), sizeof(long));//запись указ на следующ
        os.write(reinterpret_cast<const char*>(&node.size), sizeof(long));//размер
        os.write(node.data.c_str(), node.size);//запись содержимого
        return os;
    }

    friend istream& operator>>(istream& is, List& node)   //перезагруженный оператор >> для вывода

    {
        is.read(reinterpret_cast<char*>(&node.nextp), sizeof(long));//следующий
        is.read(reinterpret_cast<char*>(&node.size), sizeof(long));//размер
        string buffer;
        buffer.resize(node.size);//выделяем размер 
        is.read(&buffer[0], node.size);//читаем
        node.data = buffer;
        return is;
    }

    string get_data()//получаем содержимое строки
    {
        return data;
    }

    void set_data(string obj)
    {
        data = obj;
    }

    long get_size()//размер
    {
        return size;
    }

    void set_size(string obj)
    {
        size = Size_of_string_in_bytes(obj);
    }

    long get_nextp()//получаем  доступ к указателю
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
    fstream file_bin;  //бинарный файл
    long headp;  //указатель на начало списка
    const char* filename;

public:

    BinaryFile(const char* filename) : file_bin(filename, ios::binary | ios::in | ios::out | ios::trunc)  //конструктор
    {
        headp = -1;
        this->filename = filename;

        if (!file_bin.is_open())
        {
            file_bin.open(filename, ios::binary | ios::out);//для записи
            file_bin.close();
            file_bin.open(filename, ios::binary | ios::in | ios::out);//чтение и записи
        }

        file_bin.seekg(0, ios::beg);//устанавливаем указатель на чтение
    }

    ~BinaryFile() //дестркутор
    {
        file_bin.close();
    }

    void Filling_binary_file() //заполнение бинарного файла
    {
        string obj = "";
        ifstream file_txt("Text_file.txt");
        List node;

        if (file_txt.is_open())
        {
            file_bin.clear();//очистка ccылок
            while (getline(file_txt, obj))
            {
                node.set_data(obj);//сохраняем строку
                node.set_size(obj);//размер

                if (headp != -1)//если список не пуст, то записываем в конец
                {
                        file_bin.clear();
                        file_bin.seekg(0, std::ios::beg);//перемещаяем указатель на начало файла
                        long head = -1;//для хранения первого элемента 
                        file_bin.read(reinterpret_cast<char*>(&head), sizeof(long));//указатель на память читаем и записываевм в head
                        file_bin.seekg(head, std::ios::beg);//перемещаем на начало
                        List currentNode;
                        long prevNextPosition = file_bin.tellg();//текущая позиция указателя на потоке
                    while (!file_bin.eof() && file_bin.tellg() != -1)
                    {
                        prevNextPosition = file_bin.tellp();//сохраняем текущую позицию записи
                        file_bin >> currentNode;
                        file_bin.seekg(currentNode.get_nextp(), std::ios::beg);//перемещаем
                    }
                    file_bin.clear();
                    file_bin.seekp(0, std::ios::end);//перемещаем в конец
                    long currentPosition = file_bin.tellp();
                    file_bin << node;//записываем новый элемент
                    file_bin.seekp(prevNextPosition, std::ios::beg);
                    file_bin.write(reinterpret_cast<const char*>(&currentPosition), sizeof(long));//записываем в бин ф позицию
                }

                if (headp == -1)
                {
                    headp = 4;
                    file_bin.seekg(0, std::ios::beg);//устанавливаем указатель на начало
                    file_bin.write(reinterpret_cast<const char*>(&headp), sizeof(long));//записываем в бинарник
                    file_bin << node;
                }
            }
        }
        else
        {
            cout << "Не удалось открыть текстовый файл!" << endl;
            return;
        }

        cout << "Текст добавлен в бинарный файл!" << endl;
        file_txt.close();
    }

    void Delete(int index)//удаление по индексу
    {
        file_bin.clear();  //очистка файла от ссылок
        file_bin.seekg(0, ios::beg);//0 это байты а beg начало
        List delete_node;

        if (index == 0)
        {   
            file_bin.clear();  //очистка файла от ссылок
            file_bin.seekg(0, ios::beg); //перемещаем на начало
            long head = -1;
            file_bin.read(reinterpret_cast<char*>(&head), sizeof(long));//узнаем позиции
            file_bin.seekg(head, ios::beg); // Перемещаем указатель на эту позиции 
            file_bin >> delete_node;//извлекаем данные из потока
            long delete_nextp = delete_node.get_nextp();
            file_bin.seekg(0, ios::beg);
            file_bin.write(reinterpret_cast<const char*>(&delete_nextp), sizeof(long));//записываем новый элемент, следующий после удаленного
            headp = delete_nextp;

            cout << "Строка удалена!" << endl;
        }

        else
        {
            file_bin.clear();  //очистка файла от ссылок
            file_bin.seekg(0, ios::beg); 
            long head = -1;
            file_bin.read(reinterpret_cast<char*>(&head), sizeof(long));
            file_bin.seekg(head, ios::beg); 
            long prev_pos = head; 
            long cur_pos = head; 
            List current_node;

            for (int i = 0; i <= index; i++) // Находим элемент, предшествующий индексу вставки
            {
                if (file_bin.eof() || file_bin.tellg() == -1)//если конец 
                {
                    cout << "Некорреткный индекс!" << endl;
                    return;
                }

                prev_pos = cur_pos;//текущая позиция указателя
                cur_pos = file_bin.tellg();//записываем текущую позицию указателя на чтение в потоке
                file_bin >> current_node;//для данных текущего элемента
                file_bin.seekg(current_node.get_nextp(), ios::beg);//перемещение на следующий
            }
                
            file_bin.clear();
            long new_lastp = current_node.get_nextp();//следующий 
            file_bin.seekp(prev_pos, ios::beg); // Ставим на начало перед удаляемым
            file_bin.write(reinterpret_cast<const char*>(&new_lastp), sizeof(long));

            cout << "Строка удалена!" << endl;
        }
    }

    void Place_line(int index, string obj)  //добавление строки на конкретную позицию
    {
        file_bin.clear(); // Очищаем от ссылок
        List node;
        node.set_data(obj); // Задаём данные
        node.set_size(obj); // Задаём размер данных

        if (index < 0)
        {
            cout << "Некорреткный индекс!" << endl;
            return;
        }

        if (index == 0)
        {
            if (headp == -1)//если пустой то добавляем в начало
            {
                file_bin.clear();
                headp = 4;
                file_bin.seekg(0, ios::beg);
                file_bin.write(reinterpret_cast<const char*>(&headp), sizeof(long));//записываем содержимое headp
                file_bin << node;//записываем данные объекта в бинарник
            }

            else//в конец
            {
                file_bin.clear();
                file_bin.seekp(0, ios::end);
                long currentPosition = file_bin.tellp();
                node.set_nextp(headp);//устанавливаем указатель на текущую позицию
                file_bin << node;
                headp = currentPosition;
                file_bin.seekg(0, ios::beg);
                file_bin.write(reinterpret_cast<const char*>(&headp), sizeof(long));
            }
            cout << "Строка добавлена!" << endl;
        }

        else
        { //добавление в середину 
            file_bin.clear(); // Очистка файла от ссылок
            file_bin.seekg(0, ios::beg); // ссылаемся на начало
            long head = -1;
            file_bin.read(reinterpret_cast<char*>(&head), sizeof(long));
            file_bin.seekg(head, ios::beg); // Перемещаем указатель в начало списка
            long prevNextPosition = head; // Позиция конца удаляемого элемента
            List currentNode;

            for (int i = 0; i < index; i++) // Находим элемент, предшествующий индексу вставки
            {
                if (file_bin.eof() || file_bin.tellg() == -1)
                {
                    cout << "Некорреткный индекс!" << endl;
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
                long currentPosition = file_bin.tellp(); // Запоминаем текущую позицию
                file_bin << node; // Используем оператор << для записи элемента списка в файл
                file_bin.seekp(prevNextPosition, ios::beg);//перемещаем указатель на на позицию 
                file_bin.write(reinterpret_cast<const char*>(&currentPosition), sizeof(long));//записываем на данную позицию
                return;
            }

            long NextPosition = file_bin.tellp(); // Старая ссылка предшествующего элемента
            file_bin.seekp(0, ios::end);
            long currentPosition = file_bin.tellp(); // Запоминаем позицию конца файла
            node.set_nextp(NextPosition);
            file_bin << node; // Используем оператор << для записи элемента списка в файл
            file_bin.seekp(prevNextPosition, ios::beg);
            file_bin.write(reinterpret_cast<const char*>(&currentPosition), sizeof(long));
            cout << "Строка добавлена!" << endl;
        }
    }

    void Adding_string_to_the_end(string obj)  //добавление строки в конец файла
    {
        file_bin.clear();
        List node;
        node.set_data(obj);
        node.set_size(obj);

        if (headp != -1)
        {
            file_bin.seekg(0, ios::beg); // Ищем первый элемент
            long head = -1;
            file_bin.read(reinterpret_cast<char*>(&head), sizeof(long));
            file_bin.seekg(head, ios::beg); // Перемещаем указатель в начало списка
            List currentNode;
            long prevNextPosition = -1;
            // Находим последний элемент в списке

            while (!file_bin.eof() && file_bin.tellg() != -1)
            {
                prevNextPosition = file_bin.tellp();
                file_bin >> currentNode;
                file_bin.seekg(currentNode.get_nextp(), ios::beg);
            }

            file_bin.clear();
            file_bin.seekp(0, ios::end);
            long currentPosition = file_bin.tellp(); // Запоминаем текущую позицию
            file_bin << node; // Используем оператор << для записи элемента списка в файл
            file_bin.seekp(prevNextPosition, ios::beg);
            file_bin.write(reinterpret_cast<const char*>(&currentPosition), sizeof(long));
            cout << "Строка добавлена!" << endl;
        }

        if (headp == -1)
        {
            headp = 4;
            file_bin.seekg(0, ios::beg);
            file_bin.write(reinterpret_cast<const char*>(&headp), sizeof(long));
            file_bin << node; // Используем оператор << для записи элемента списка в файл
            cout << "Строка добавлена!" << endl;
        }
    }

    void Redactor(int index, string obj)
    {
        file_bin.clear(); // Очищаем от ссылок
        List node;
        node.set_data(obj); // Задаём данные
        node.set_size(obj); // Задаём размер данных

        if (index < 0)
        {
            cout << "Некорреткный индекс!" << endl;
            return;
        }
        if (index == 0)
        {
            List currentNode;
            file_bin.clear(); // Очистка файла от ссылок
            file_bin.seekg(0, std::ios::beg); // Ищем первый элемент
            long head = -1;
            file_bin.read(reinterpret_cast<char*>(&head), sizeof(long));//чтение позиции первоо элм
            file_bin.seekg(head, std::ios::beg); // Перемещаем указатель в начало списка
            file_bin >> currentNode;//первый элемент
            long prevNextPosition = currentNode.get_nextp();//получение следующей позиции
            file_bin.seekp(0, std::ios::end);
            long currentPosition = file_bin.tellp(); // Запоминаем позицию конца файла
            node.set_nextp(prevNextPosition);//обновляем ссылку на след элемент
            file_bin << node; // Используем оператор << для записи элемента списка в файл
            file_bin.seekg(0, std::ios::beg);
            file_bin.write(reinterpret_cast<const char*>(&currentPosition), sizeof(long));
            cout << "Строка изменена!" << endl;
        }
        else
        {
            file_bin.clear(); // Очистка файла от ссылок
            file_bin.seekg(0, std::ios::beg); // Ищем первый элемент
            long head = -1;
            file_bin.read(reinterpret_cast<char*>(&head), sizeof(long));
            file_bin.seekg(head, std::ios::beg); // Перемещаем указатель в начало списка

            long prevNextPosition = head; // Позиция конца удаляемого элемента для отслеживания позиции перед вставкой

            List currentNode;
            for (int i = 0; i < index; i++) // Находим элемент, предшествующий индексу вставки
            {
                if (file_bin.eof() || file_bin.tellg() == -1)
                {
                    cout << "Некорреткный индекс!" << endl;
                    return;
                }
                prevNextPosition = file_bin.tellp();//позия элемента перед, которым нужно встваить
                file_bin >> currentNode;
                file_bin.seekg(currentNode.get_nextp(), std::ios::beg);
            }
            file_bin.clear();
            if (currentNode.get_nextp() == -1)//записываем в конец
            {
                file_bin.seekp(0, std::ios::end);
                long currentPosition = file_bin.tellp(); // Запоминаем текущую позицию
                file_bin << node; // Используем оператор << для записи элемента списка в файл
                file_bin.seekp(prevNextPosition, std::ios::beg);
                file_bin.write(reinterpret_cast<const char*>(&currentPosition), sizeof(long));
                return;
            }
            //обновляем ссылку и записываем в бинарник
            file_bin >> currentNode;
            long NextPosition = currentNode.get_nextp(); // Старая ссылка предшествующего элемента
            file_bin.seekp(0, std::ios::end);
            long currentPosition = file_bin.tellp(); // Запоминаем позицию конца файла
            node.set_nextp(NextPosition);
            file_bin << node; // Используем оператор << для записи элемента списка в файл
            file_bin.seekp(prevNextPosition, std::ios::beg);
            file_bin.write(reinterpret_cast<const char*>(&currentPosition), sizeof(long));
            cout << "Строка изменена!" << endl;
        }
    }

    void View_binary_file()  //функция просмотра бинарного файла
    {
        if (headp == -1)
        {
            cout << "В файле нет элементов!" << endl;
            return;
        }

        int lines = 0;
        file_bin.clear(); // Очистка файла от ссылок
        file_bin.seekg(0, ios::beg); // Ищем первый элемент
        long head = -1;
        file_bin.read(reinterpret_cast<char*>(&head), sizeof(long));
        file_bin.seekg(head, ios::beg); // Перемещаем указатель в начало списка
        cout << "Текст в бинарном файле:" << endl << endl;
        while (!file_bin.eof() && file_bin.tellg() != -1)
        {
            List node;
            file_bin >> node; // Используем оператор >> для чтения элемента списка из файла
            file_bin.seekp(node.get_nextp(), ios::beg); // Перемещяем указатель на следующий элемент

            if (!file_bin.eof())
            {
                lines++;
                cout << "Bin " << lines << " | " << node.get_data() << endl;
            }
        }

        cout << endl;
    }

    int String_in_bin()//функция по подсчету строк в бинарном файле
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

    void Filling_txt_file() //заполнение текстового файла
    {
        ofstream file_txt("Text_file.txt", ios::trunc);  //открываем текстовый файл для записи, открыв файл очищаем его
        if (file_txt.is_open())
        {
            if (headp == -1)
            {
                cout << "В файле нет текста!" << endl;
                return;
            }

            file_bin.clear(); //очистка файла от ссылок
            file_bin.seekg(0, ios::beg); //ставим указатель в начало бинарного файла
            long head = -1;
            file_bin.read(reinterpret_cast<char*>(&head), sizeof(long));  //считываем первый элемент списка
            file_bin.seekg(head, ios::beg); //перемещаем указатель в начало списка

            while (!file_bin.eof() && file_bin.tellg() != -1)
            {
                List node;
                file_bin >> node; //используем оператор >> для чтения элемента списка из файла
                file_bin.seekp(node.get_nextp(), std::ios::beg); // перемещяем указатель на следующий элемент

                if (!file_bin.eof())
                {
                    string obj = node.get_data();
                    file_txt << node.get_data() << "\n";
                }
            }
        }
        else
        {
            cout << "Не удалось открыть текстовый файл!" << endl;
            return;
        }

        cout << "Текст добавлен в текстовый файл!" << endl;
        file_txt.close();
    }

};


void menu()
{
    cout << "Нажмите [0] для завершения программы;" << endl;
    cout << "Нажмите [1] для загрузки текста в двоичный файл;" << endl;
    cout << "Нажмите [2] для удаления строки;" << endl;
    cout << "Нажмите [3] для редактированя строки;" << endl;
    cout << "Нажмите [4] для вставки строки на конкретную позицию;" << endl;
    cout << "Нажмите [5] для добавления строки в конец;" << endl;
    cout << "Нажмите [6] для сохранения изменений в текстовый файл;" << endl;
    cout << "Нажмите [7] для просмотра бинарного файла." << endl;
    cout << "Нажмите [8] для просмотра текста." << endl;
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
        cout << endl << "Введите цифру: ";
        cin >> input;
        cout << endl;

        if (input == 0) //Выход из программы
        {
            break;
        }

        switch (input)
        {
        case 1:  //загрузка текстового файла
            BinFile.Filling_binary_file();
            break;

        case 2:  //удаление строки
            cout << "Какую строку нужно удалить? Выберите строку от 1 до " << BinFile.String_in_bin() << ": ";
            cin >> input_line;
            BinFile.Delete(input_line - 1);
            break;

        case 3:  //редактирование строки
            cout << "Какую строку нужно переписать? Выберите строку от 1 до " << BinFile.String_in_bin() << ": ";
            cin >> input_line;
            cout << "Введите вашу строку: ";
            getline(cin, new_string);
            getline(cin, new_string);//для "/n"
            BinFile.Redactor(input_line - 1, new_string);
            cout << endl;
            break;

        case 4:  //вставка строки на конкретное место
            cout << "На какое место вставить новую строку? Выберите строку от 1 до " << BinFile.String_in_bin() << ": ";
            cin >> input_line;
            cout << "Введите вашу строку: ";
            getline(cin, new_string);
            getline(cin, new_string);
            cout << endl;
            BinFile.Place_line(input_line - 1, new_string);
            break;

        case 5:  //добавление строки в конец
            cout << "Введите вашу строку: ";
            cin >> new_string;
            BinFile.Adding_string_to_the_end(new_string);
            break;

        case 6:  //сохранение изменений в текстовый файл
            BinFile.Filling_txt_file();
            break;

        case 7:  //показ текста бинарника
            BinFile.View_binary_file();
            break;

        case 8:  //показ текста
            Show_text();
            break;

        default:
            break;
        }
    }
    return 0;
}