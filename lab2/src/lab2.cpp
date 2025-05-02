#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <map>

/*!
Реализовать поиск заданного элемента в массиве (необходимо найти ВСЕ вхождения)
объектов по ключу в соответствии с вариантом (ключом является  первое НЕ числовое поле объекта) следующими методами:
- линейный поиск
- с помощью бинарного дерева поиска
- с помощью красно-черного дерева
- с помощью хэш таблицы
*/
class Accountant
{
    // Вынес в паблик, чтобы не надо было определять геттеры
public:
    //! ФИО служащего
    std::string full_name;
    //! Должность
    std::string job_title;
    //! Подразделение
    std::string subdivision;
    //! Месячная зарплата
    unsigned long salary;

    Accountant(std::string _full_name = "", std::string _job_title = "", std::string _subdivision = "", unsigned long _salary = 0)
    {
        this->full_name = _full_name;
        this->job_title = _job_title;
        this->subdivision = _subdivision;
        this->salary = _salary;
    }

    bool operator<(const Accountant &account) const
    {
        return (this->subdivision != account.subdivision) ? (this->subdivision < account.subdivision) : ((this->full_name != account.full_name) ? (this->full_name < account.full_name) : ((this->salary != account.salary) ? (this->salary < account.salary) : false));
    }

    bool operator==(const Accountant &account) const
    {
        return (this->subdivision == account.subdivision && this->salary == account.salary && this->job_title == account.job_title && this->full_name == account.full_name);
    }

    bool operator!=(const Accountant &account) const
    {
        return !(*this == account);
    }

    bool operator<=(const Accountant &account) const
    {
        return *this == account || *this < account;
    }

    bool operator>(const Accountant &account) const
    {
        return !(*this <= account);
    }

    bool operator>=(const Accountant &account) const
    {
        return !(*this < account);
    }
};

/*
ЧАСТЬ С ХЭШМАПОМ
*/
const unsigned long int HASH_MAP_SIZE = 1e6+33;

/*!
 * @brief Функция подсчёта хэша
 *
 * Преобразует объект в число
 *
 * @param obj От какого объекта считаем хэш
 *
 * @return Значение хэша
 */
unsigned long int HashRot13(std::string& s)
{
    unsigned long int hash = 0;
    const char* str = s.c_str();
    for (; *str; str++)
    {
        hash += (unsigned char)(*str);
        hash -= (hash << 13) | (hash >> 19);
    }
    return hash % HASH_MAP_SIZE;
}

double hashmap_adapter(Accountant a[], long size, std::string findElem, long result[], long &resultSize)
{
    
    std::vector<std::vector<std::pair<Accountant&, unsigned int>>> hashmap(HASH_MAP_SIZE);

    for (long i = 0; i < size; i++)
    {
        unsigned long int new_key = HashRot13(a[i].full_name);
        hashmap[new_key].push_back(std::make_pair(std::ref(a[i]), i));
    }

    // Поиск элемента
    resultSize = 0;
    clock_t start = clock();

    unsigned long int findKey = HashRot13(findElem);
    std::vector<std::pair<Accountant&, unsigned int>> collision_objects = hashmap[findKey];
    for(int i = 0; i < collision_objects.size(); ++i)
    {
        if(collision_objects[i].first.full_name == findElem)
        {
            result[resultSize] = collision_objects[i].second;
            ++resultSize;
        }
    }

    clock_t end = clock();

    return (double)(end - start) / (double)(CLOCKS_PER_SEC);
}


// БИНАРНОЕ ДЕРЕВО

/*!
 * @brief Структура бинарного дерева, на котором будем строить поиск
 *
 * Функция для записи в файл
 *
 * @param left Файловый поток для записи
 * @param func Функция алгоритма
 * @param fileStream Файловый поток для вывода алгоритма
 *
 */
struct tree
{
    //! Левый узел
    tree *left;
    //! Правый узел
    tree *right;
    //! Ключ текущего узла (ФИО служащего)
    std::string key;
    //! Значение текущего узла
    Accountant obj;
    //! Индекс элемента в исходном массиве
    long index;
};

/*!
 * @brief Поиск первого вхождения элемента
 *
 * Функция для поиска первого элемента по заданному ключу в поддереве root
 *
 * @param root Корень дерева или поддерева для поиска
 * @param key По какому ключу искать?
 *
 * @return Узел найденного элемента
 */

tree *search(tree *root, std::string key)
{
    if ((root == NULL) || (root->key == key))
    {
        return root;
    }
    if (key < root->key)
        return search(root->left, key);
    return search(root->right, key);
}

/*!
 * @brief Поиск всех элементов
 *
 * Функция для поиска всех элементов по заданному ключу в поддереве root
 *
 * @param root Корень дерева или поддерева для поиска
 * @param key По какому ключу искать?
 * @param result[] Массив с результатом, в который будет записан результат выполнения алгоритма (индексы объектов с заданным ключом в массиве)
 * @param resultSize Размер массива result
 */
void searchAll(tree *root, std::string key, long result[], long &resultSize)
{
    tree *cur = NULL;
    tree *tmp = root;
    do
    {
        cur = search(tmp, key);
        if (!cur)
            break;
        result[resultSize] = cur->index;
        resultSize++;
        tmp = cur->right;
    } while (tmp);
}

/*!
 * @brief Очистка дерева
 *
 * Рекурсивная функция для освобождения памяти, занятой деревом
 *
 * @param root Корень дерева или поддерева для очистки
 */
void clearTree(tree *root)
{
    if (root == NULL)
        return;

    clearTree(root->left);  // Очищаем левое поддерево
    clearTree(root->right); // Очищаем правое поддерево
    delete root;            // Удаляем текущий узел
}

std::ostream &operator<<(std::ostream &os, const Accountant &account)
{
    return os << account.full_name << "; " << account.subdivision << "; " << account.job_title << "; " << account.salary;
}

std::istream &operator>>(std::istream &is, Accountant &account)
{
    std::string fullName, jobTitle, subdivision;
    unsigned long salary;
    std::getline(is, fullName);
    std::getline(is, jobTitle);
    std::getline(is, subdivision);
    is >> salary;
    is.ignore(); // Чтобы пропустить символ переноса строки из буффера
    account.full_name = fullName;
    account.job_title = jobTitle;
    account.subdivision = subdivision;
    account.salary = salary;
    return is;
}

/*!
 * @brief Чтение из файла
 *
 * Функция, считывающая данные из файла в массив
 *
 * @param myFile Поток файлового ввода
 * @param accountants[] Массив объектов Accountant
 * @param sample_size Размер массива
 *
 * @return Ничего. Массив сортируется внутри
 */
void readFromFile(std::fstream &myFile, Accountant accountants[], unsigned int sample_size)
{
    unsigned int i = 0;
    while (i < sample_size)
    {
        Accountant tmpObj;
        myFile >> tmpObj;
        if (!myFile)
            break;
        accountants[i] = tmpObj;
        ++i;
    }
}

/*!
 * @brief Линейный поиск
 *
 * Поиск проходится по всем элементам массива и сравнивает каждый элемент массива с переданным. Если совпали, то в переданный массив result заносится индекс найденного элемента. Ищутся все вхождения.
 *
 * @param a[] Массив элементов, в котором ищем элемент
 * @param size Размер массива
 * @param findElem Какой элемент ищем
 * @param result[] В него записываем индексы результата
 * @param resultSize Размер массива result
 *
 * @return Возвращает всегда 0. Обновиться массив result
 */
template <class T>
double linearSearch(T a[], long size, std::string findElem, long result[], long &resultSize)
{
    for (long i = 0; i < size; ++i)
    {
        if (a[i].full_name == findElem)
        {
            result[resultSize] = i;
            resultSize++;
        }
    }
    return 0.0;
}

/*!
 * @brief  Поиск в бинарном дереве
 *
 * Функция для создания бинарного дерева и поиска в нём

 * @param a[] Массив элементов, в котором ищем элемент
 * @param size Размер массива
 * @param findElem Какой элемент ищем
 * @param result[] В него записываем индексы результата
 * @param resultSize Размер массива result
 *
 * @return Обновиться массив result и вернётся время поиска элемента
 */
template <class T>
double binary_tree_adapter(T a[], long size, std::string findElem, long result[], long &resultSize)
{
    tree *root = new tree();
    root->obj = a[0];
    root->key = a[0].full_name;
    root->left = NULL;
    root->right = NULL;
    root->index = 0;

    for (long i = 1; i < size; i++)
    {
        tree *newNode = new tree();
        newNode->obj = a[i];
        newNode->key = a[i].full_name;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->index = i;

        tree *current = root;
        tree *parent = NULL;

        // ищем место
        while (current != NULL)
        {
            parent = current;
            if (newNode->key < current->key)
                current = current->left;
            else
                current = current->right;
        }

        // вставляем
        if (newNode->key < parent->key)
            parent->left = newNode;
        else
            parent->right = newNode;
    }

    // Поиск элемента
    resultSize = 0;
    clock_t start = clock();
    // searchAll(root, findElem.full_name, result, resultSize);
    searchAll(root, findElem, result, resultSize);
    clock_t end = clock();

    // Очистка дерева
    clearTree(root);
    return (double)(end - start) / (double)(CLOCKS_PER_SEC);
}

/*!
 * @brief Подсчёт времени
 *
 * Функция для вычисления времени работы алгоритма
 *
 * @param func Функция с алгоритмом сортировки
 * @param a[] Массив элементов, который надо отсортировать
 * @param size Размер массива
 * @param findElem Элемент для поиска
 * @param result Массив с результатом
 * @param resultSize Размер массива с результатом
 *
 * @return Время работы алгоритма
 */
template <class T>
double calc_algo_time(double (*func)(T[], long, std::string, long[], long &), T a[], long size, std::string findElem, long result[], long &resultSize)
{
    clock_t start = clock();
    double t = func(a, size, findElem, result, resultSize);
    clock_t end = clock();
    return t ? t : ((double)(end - start) / (double)(CLOCKS_PER_SEC));
}

/*!
 * @brief Функция открытия файла
 *
 * Функция для вычисления времени работы алгоритма
 *
 * @param outFile Поток для открытия файла
 * @param filename Имя файла для открытия
 *
 * @return 500, если ошибка, иначе 0
 */
int openOutFile(std::fstream &outFile, std::string &filename)
{
    outFile.open(filename, std::ios::out);
    if (!outFile)
    {
        std::cerr << "Unable to open output file: " << filename;
        return 500;
    }
    return 0;
}

/*!
 * @brief Функция записи в файл
 *
 * Функция для записи в файл
 *
 * @param outFile Файловый поток для записи
 * @param a Массив, который надо записать
 * @param sample_size Размер массива
 *
 * @return Ничего
 */
template <class T>
void writeToFile(std::fstream &outFile, T *a, long sample_size)
{
    for (int i = 0; i < sample_size; ++i)
        outFile << "Сотрудник " << i + 1 << ": " << a[i] << std::endl;
}

/*!
 * @brief Структура для алгоритмов
 *
 * Функция для записи в файл
 *
 * @param name Файловый поток для записи
 * @param func Функция алгоритма
 * @param fileStream Файловый поток для вывода алгоритма
 *
 */
struct algoInfo
{
    std::string name;
    double (*func)(Accountant[], long, std::string, long[], long &);
    std::fstream &fileStream;
    algoInfo(std::string name, double (*func)(Accountant[], long, std::string, long[], long &), std::fstream &fileStream) : name(name), func(func), fileStream(fileStream) {}
};

/*!
 * @brief Структура красно-черного дерева
 */
struct RBTree
{
    RBTree *left;
    RBTree *right;
    RBTree *parent;
    std::string key;              // ФИО служащего
    std::vector<Accountant> objs; // Данные сотрудника
    std::vector<long> indexes;    // Индекс в исходном массиве
    bool color;                   // true = красный, false = черный

    RBTree() : left(nullptr), right(nullptr), parent(nullptr), color(true) {}
};

/*!
 * @brief Левый поворот в красно-черном дереве
 */
void leftRotate(RBTree *&root, RBTree *k1)
{
    // k2 на той картинке
    RBTree *k2 = k1->right;
    // Меняем родителя в поддереве
    k2->parent = k1->parent;

    // Если k1 был корнем, то k2 теперь корень
    if (k1->parent == nullptr)
        root = k2;
    else if (k1 == k1->parent->left)
        k1->parent->left = k2; // Если k1 был левым ребенком, то k2 теперь левый ребенок
    else
        k1->parent->right = k2; // Если k1 был правым ребенком, то k2 теперь правый ребенок

    // B теперь цепляется к k1
    k1->right = k2->left;

    // Если B не nullptr, то B->parent = k1
    if (k2->left != nullptr)
        k2->left->parent = k1;

    // Теперь k2 - родитель k1
    k1->parent = k2;

    // Теперь k1 - левый потомок k2
    k2->left = k1;
}

/*!
 * @brief Правый поворот в красно-черном дереве
 */
void rightRotate(RBTree *&root, RBTree *k1)
{
    RBTree *k2 = k1->left;
    // Меняем родителя в поддереве
    k2->parent = k1->parent;

    // Если k1 был корнем, то k2 теперь корень
    if (k1->parent == nullptr)
        root = k2;
    else if (k1 == k1->parent->right)
        k1->parent->right = k2; // Если k1 был правым потомок, то k2 теперь правый потомок
    else
        k1->parent->left = k2; // Если k1 был левым потомок, то k2 теперь левый потомок

    // B теперь цепляется к k1
    k1->left = k2->right;

    // Если B не nullptr, то B->parent = k2
    if (k2->right != nullptr)
        k2->right->parent = k1;

    // Теперь k2 - родитель k1
    k1->parent = k2;
    // Теперь k1 - правый потомок k2
    k2->right = k1;
}

/*!
 * @brief Восстановление свойств красно-черного дерева после вставки
 *
 * Функция для восстановления свойств красно-черного дерева после вставки
 *
 * @param root Корень дерева
 * @param k Узел, который вставляем
 *
 * @return Ничего. Действия происходят в функции на деревом root
 */
void fixTree(RBTree *&root, RBTree *insertedNode)
{
    RBTree *u;  // дядя
    RBTree *f;  // отец
    RBTree *gf; // дед
    // Пока Не дойдём до корня и родитель красный (перекраска идёт только жо этого момента)
    while (insertedNode->parent != nullptr && insertedNode->parent->color)
    {
        // TODO: ПЕРЕПИССАТЬ БОЛЕЕ ЛОГИЧНО ПО РАЗНЫМ СЛУЧАЯМ
        f = insertedNode->parent;
        gf = insertedNode->parent->parent;
        u = gf->left == f ? gf->right : gf->left; // uncle - дядя
        bool node_left = insertedNode == f->left;
        bool father_left = f == gf->left;
        if (u != nullptr && u->color)
        { // И дядя красный
            // Первый случай
            u->color = false;  // Перекрашиваем дядю
            f->color = false;  // Перекрашиваем отца
            gf->color = true;  // Перекрашиваем деда
            insertedNode = gf; // Переходим к дедушке и продолжаем восстанавливать свойства относительно него
        }
        // Дядя чёрный или nullptr.
        else
        {
            // Если сложный случай (надо делать поворот сначала)
            if (node_left ^ father_left)
            {
                // Т.к. отец меняется, а перекраску надо выполнять относительно нового листа
                insertedNode = f;
                node_left ? rightRotate(root, insertedNode) : leftRotate(root, insertedNode); // узел левый ? случай г) : случай в)
                f = insertedNode->parent;
                gf = f->parent;
            }
            // Делаем перекраску
            f->color = false;
            gf->color = true;
            // Отец левый ? случай а) : случай б)
            father_left ? rightRotate(root, gf) : leftRotate(root, gf);
        }
    }
    root->color = false; // Корень всегда чёрный
}

/*!
 * @brief Вставка узла в красно-черное дерево
 *
 * Функция для вставки узла в красно-черное дерево
 *
 * @param root Корень дерева
 * @param node Узел для вставки
 *
 * @return Ничего. Действия происходят в функции на дереве root
 */
void insert(RBTree *&root, RBTree *node)
{
    RBTree *expected_parent = nullptr;
    RBTree *expected_place = root;

    // Находим место для вставки
    while (expected_place != nullptr)
    {
        if (node->key == expected_place->key)
        {
            // Вообще говоря, нам не надо хранить объекты в узле, но если понадобится, то они тут
            // конкатенация векторов
            expected_place->objs.insert(expected_place->objs.end(), node->objs.begin(), node->objs.end());
            expected_place->indexes.insert(expected_place->indexes.end(), node->indexes.begin(), node->indexes.end());
            return;
        }
        expected_parent = expected_place;
        (node->key < expected_place->key) ? expected_place = expected_place->left : expected_place = expected_place->right;
    }

    // Вставляем узел
    if (expected_parent == nullptr)
        root = node;
    else if (node->key < expected_parent->key)
        expected_parent->left = node;
    else
        expected_parent->right = node;
    node->parent = expected_parent;

    // Восстанавливаем свойства красно-черного дерева
    fixTree(root, node);
}

/*!
 * @brief Поиск в красно-черном дереве
 *
 * Функция для поиска в красно-черном дереве
 *
 * @param root Корень дерева
 * @param key Ключ для поиска
 *
 * @return Указатель на узел, если он найден, иначе nullptr
 */
RBTree *search(RBTree *root, const std::string key)
{
    if (root == nullptr || root->key == key)
    {
        return root;
    }
    if (key < root->key)
    {
        return search(root->left, key);
    }
    return search(root->right, key);
}

/*!
 * @brief Поиск всех вхождений в красно-черном дереве
 *
 * Функция для поиска всех вхождений в красно-черном дереве
 *
 * @param root Корень дерева
 * @param key Ключ для поиска
 * @param result Массив для хранения результата
 * @param resultSize Размер массива для хранения результата
 *
 * @return Ничего. Действия происходят в функции и результат пишется в массив result
 */
void searchAll(RBTree *root, const std::string key, long result[], long &resultSize)
{
    RBTree *current = root;
    while (current != nullptr)
    {
        if (key < current->key)
        {
            current = current->left;
        }
        else if (key > current->key)
        {
            current = current->right;
        }
        else
        {
            // Нашли совпадение
            for (size_t i = 0; i < current->indexes.size(); i++)
            {
                result[resultSize++] = current->indexes[i];
            }
            // Всё найдено, выходим
            break;
        }
    }
}

/*!
 * @brief Очистка красно-черного дерева
 */
void clearRBTree(RBTree *root)
{
    if (root == nullptr)
        return;
    clearRBTree(root->left);
    clearRBTree(root->right);
    delete root;
}

// template <class T>
/*!
 * @brief Адаптер для поиска в красно-черном дереве
 *
 * Адаптер для функции поиска в красно-черном дереве
 *
 * @param a[] Массив элементов
 * @param size Размер массива
 * @param findElem Ключ для поиска
 * @param result Массив для хранения результата
 * @param resultSize Размер массива для хранения результата
 *
 * @return Время
 */
double red_black_tree_adapter(Accountant a[], long size, std::string findElem, long result[], long &resultSize)
{
    RBTree *root = nullptr;

    // Создаем красно-черное дерево
    for (long i = 0; i < size; i++)
    {
        RBTree *node = new RBTree();
        node->objs = std::vector({a[i]});
        node->key = a[i].full_name;
        node->indexes = std::vector({i});
        insert(root, node);
    }

    // Поиск элемента
    resultSize = 0;
    clock_t start = clock();
    searchAll(root, findElem, result, resultSize);
    clock_t end = clock();

    // Очистка дерева
    clearRBTree(root);
    return (double)(end - start) / (double)(CLOCKS_PER_SEC);
}
double multimap_adapter(Accountant a[], long size, std::string findElem, long result[], long &resultSize)
{
    std::multimap<std::string, long> tree;
    for (long i = 0; i < size; i++)
        tree.insert(std::make_pair(a[i].full_name, i));

    // Поиск элемента
    resultSize = 0;
    clock_t start = clock();
    std::pair<std::multimap<std::string, long>::iterator, std::multimap<std::string, long>::iterator> range = tree.equal_range(findElem); // Начало и конец диапазона
    for (std::multimap<std::string, long>::iterator it = range.first; it != range.second; ++it)
        result[resultSize++] = it->second;
    clock_t end = clock();

    return (double)(end - start) / (double)(CLOCKS_PER_SEC);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage ./lab2 <count of files in tests directory>";
        return 400;
    }
    setlocale(LC_ALL, "Russian");

    std::fstream analyticsFile;
    std::string analyticsFilename = "analytics.txt";
    analyticsFile.open(analyticsFilename, std::ios::out);
    for (int test_id = 0; test_id < std::stoi(argv[1]); ++test_id)
    {
        std::fstream inFile;
        std::string inFilename = "tests/test_" + std::to_string(test_id) + ".txt";
        inFile.open(inFilename, std::ios::in);
        if (!inFile)
        {
            std::cerr << "Unable to open file: " << inFilename;
            return 500;
        }

        std::fstream outFileA, outFileB, outFileC, outFileD, outFileReal;
        std::string outFilenameA = "results/result_a_" + std::to_string(test_id) + ".txt";
        std::string outFilenameB = "results/result_b_" + std::to_string(test_id) + ".txt";
        std::string outFilenameC = "results/result_c_" + std::to_string(test_id) + ".txt";
        std::string outFilenameD = "results/result_d_" + std::to_string(test_id) + ".txt";
        std::string outFilenameReal = "results/result_real_" + std::to_string(test_id) + ".txt";
        if (openOutFile(outFileA, outFilenameA) == 500 || openOutFile(outFileB, outFilenameB) == 500 || openOutFile(outFileC, outFilenameC) == 500 || openOutFile(outFileD, outFilenameD) == 500 || openOutFile(outFileReal, outFilenameReal) == 500)
        {
            std::cerr << "UNABLE TO OPEN FILE" << std::endl;
            return 500;
        }

        unsigned int sample_size = 0;
        inFile >> sample_size;
        inFile.ignore(); // Чтобы пропустить символ переноса строки из буффера

        Accountant *accountants = new Accountant[sample_size];
        Accountant *accountantsBackup = new Accountant[sample_size];
        long result[1000000] = {0}; // Больше этого точно не будет
        long resultSize = 0;

        readFromFile(inFile, accountants, sample_size);

        srand(time(0));
        long real_idx = rand() % sample_size;
        // Accountant findElem = accountants[real_idx];
        std::string findElem = accountants[real_idx].full_name;

        // Создаём бэкап для восстановления массива
        std::copy(accountants, accountants + sample_size, accountantsBackup);
        std::vector<algoInfo> algos_arr = {algoInfo("Линейный поиск", linearSearch, outFileA), algoInfo("Бинарное дерево", binary_tree_adapter, outFileB), algoInfo("Красно-черное дерево", red_black_tree_adapter, outFileC), algoInfo("Hashmap",hashmap_adapter, outFileD), algoInfo("Multimap", multimap_adapter, outFileReal)};
        analyticsFile << sample_size;
        for (int i = 0; i < algos_arr.size(); ++i)
        {
            // Считаем время поиска
            double algo_time = calc_algo_time(algos_arr[i].func, accountants, sample_size, findElem, result, resultSize);
            std::cout << algos_arr[i].name << " под номером " << test_id << " " << sample_size << " элементов " << " заняла: " << algo_time << " секунд" << std::endl;
            writeToFile(algos_arr[i].fileStream, result, resultSize);
            analyticsFile << " " << algo_time;

            // Возвращаем исходный массив
            std::copy(accountantsBackup, accountantsBackup + sample_size, accountants);
        }
        // std::cout << "РЕАЛЬНЫЙ ИНДЕКС: " << real_idx << std::endl;
        // std::cout << "РЕАЛЬНЫЙ ФИО: " << findElem << std::endl;
        analyticsFile << std::endl;
        delete[] accountantsBackup;
        delete[] accountants;
    }
    return 0;
}