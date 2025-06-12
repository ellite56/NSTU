#include <iostream>
#include <set>
#include <windows.h>
#include <string>

using namespace std;

// Предварительное объявление классов
class Process;
class OSObject;

// Элемент ожидания, связывающий процесс и объект
class WaitItem {
public:
    Process *process;
    OSObject *object;
    WaitItem *next_in_process;
    WaitItem *prev_in_process;
    WaitItem *next_in_object;
    WaitItem *prev_in_object;

    static int global_id_counter;
    int id;

    WaitItem(Process *p, OSObject *obj)
            : process(p), object(obj),
              next_in_process(nullptr), prev_in_process(nullptr),
              next_in_object(nullptr), prev_in_object(nullptr),
              id(++global_id_counter) {}

    void unlink();
};

int WaitItem::global_id_counter = 0;

// Класс процесса/потока
class Process {
public:
    static int global_id_counter;
    int id;

    WaitItem *wait_list_head; // Заголовок списка ожиданий
    int wait_count;           // Количество активных ожиданий

    Process() : wait_list_head(nullptr), wait_count(0) {
        id = ++global_id_counter;
    }

    // Начать ожидание события от объекта
    void wait_for(OSObject *obj);

    // Уменьшить счётчик ожиданий; разблокировать при обнулении
    void remove_one_wait();

    // Разблокировать процесс (заглушка)
    void wake_up() {
        cout << "[Процесс P" << id << "] Разблокирован!" << endl;
    }
};

int Process::global_id_counter = 0;

// Базовый класс объектов ОС
class OSObject {
public:
    static int global_id_counter;
    string name;

    WaitItem *wait_list_head; // Заголовок списка ожиданий

    OSObject() : wait_list_head(nullptr) {
        name = "OBJ" + to_string(++global_id_counter);
    }

    // Обработка наступления события
    void signal();
};

int OSObject::global_id_counter = 0;

// Реализация методов WaitItem
void WaitItem::unlink() {
    cout << "[WaitItem " << id << "] Удаление из списков:"
         << " процесс " << process->id
         << ", объект " << object->name << endl;

    // Удаление из списка процесса
    if (prev_in_process) {
        prev_in_process->next_in_process = next_in_process;
    } else if (process) {
        process->wait_list_head = next_in_process;
    }
    if (next_in_process) {
        next_in_process->prev_in_process = prev_in_process;
    }

    // Удаление из списка объекта
    if (prev_in_object) {
        prev_in_object->next_in_object = next_in_object;
    } else if (object) {
        object->wait_list_head = next_in_object;
    }
    if (next_in_object) {
        next_in_object->prev_in_object = prev_in_object;
    }

    // Обнуление указателей
    next_in_process = prev_in_process = nullptr;
    next_in_object = prev_in_object = nullptr;
}

// Реализация методов Process
void Process::wait_for(OSObject *obj) {
    cout << "[Процесс P" << id << "] Начинает ожидание " << obj->name << endl;
    wait_count++;
    WaitItem *item = new WaitItem(this, obj);

    // Добавление в список процесса (в начало)
    item->next_in_process = wait_list_head;
    if (wait_list_head) {
        wait_list_head->prev_in_process = item;
    }
    wait_list_head = item;

    // Добавление в список объекта (в начало)
    item->next_in_object = obj->wait_list_head;
    if (obj->wait_list_head) {
        obj->wait_list_head->prev_in_object = item;
    }
    obj->wait_list_head = item;
}

void Process::remove_one_wait() {
    wait_count--; // Уменьшаем счётчик
    cout << "[Процесс P" << id << "] Осталось ожиданий: " << wait_count << endl;
    if (wait_count == 0) {
        wake_up(); // Разблокируем процесс, если ожиданий не осталось
    }
}

// Реализация метода OSObject::signal()
void OSObject::signal() {
    cout << "\n[СИГНАЛ] " << name << " отправляет уведомления" << endl;

    WaitItem *curr = wait_list_head;
    while (curr) {
        WaitItem *next = curr->next_in_object; // Сохраняем следующий элемент
        Process *p = curr->process;

        // Удаляем текущий элемент ожидания
        curr->unlink();
        delete curr;

        // Обновляем счётчик и при необходимости разблокируем процесс
        p->remove_one_wait();

        curr = next; // Переходим к следующему элементу
    }
}

// Пример использования
int main() {
    system("chcp 65001");

    OSObject obj1, obj2, obj3, obj4, obj5;
    Process p1, p2, p3, p4, p5, p6, p7, p8;

    cout << "\n НАЧАЛО НАСТРОЙКИ ОЖИДАНИЙ \n";

    p1.wait_for(&obj1);
    p1.wait_for(&obj2);

    p2.wait_for(&obj1);
    p2.wait_for(&obj3);
    p2.wait_for(&obj4);

    p3.wait_for(&obj2);
    p3.wait_for(&obj3);
    p4.wait_for(&obj3);
    p4.wait_for(&obj4);

    p5.wait_for(&obj4);
    p5.wait_for(&obj3);

    p6.wait_for(&obj1);
    p6.wait_for(&obj2);
    p6.wait_for(&obj3);

    /*p7.wait_for(&obj4);

    p8.wait_for(&obj1);
    p8.wait_for(&obj2);
    p8.wait_for(&obj3);
    p8.wait_for(&obj4);
    p8.wait_for(&obj5);*/

    cout << "\nНАЧАЛО СИГНАЛОВ \n";

    obj1.signal();
    obj2.signal();
    obj3.signal();
    obj4.signal();
    //obj5.signal();

    cout << "\n ЗАВЕРШЕНИЕ РАБОТЫ \n";

    return 0;
}