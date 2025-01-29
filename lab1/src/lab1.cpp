#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
/*!
* Вариант - 3
*  3, в, г, ж 
* 
* Массив данных бухгалтерии: ФИО служащего,  должность, подразделение, 
месячная зарплата 
(сравнение по полям – подразделение, ФИО служащего, зарплата)
*
* в - Вставками
* г - Шейкер
* ж - Слиянием
*/

//! Класс Бухгалтера
/*!
* Не очень понял почему он так называется, т.к. должность у них у всех должна быть бухгалтер, 
поэтому для интереса у объектов этого класса может быть ещё и другая должность, например "Токарь".
Объекты этого класса я буду сортировать
*/
class Accountant {
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

        Accountant(std::string _full_name="", std::string _job_title="", std::string _subdivision="", unsigned long _salary=0)
        {
            this->full_name = _full_name;
            this->job_title = _job_title;
            this->subdivision = _subdivision;
            this->salary = _salary;
        }

        bool operator < (const Accountant& account) const
        {
            return (this->subdivision != account.subdivision) ? (this->subdivision < account.subdivision) : ((this->full_name != account.full_name) ? (this->full_name < account.full_name) : ((this->salary != account.salary) ? (this->salary < account.salary) : false));
        }

        bool operator == (const Accountant& account) const
        {
            return (this->subdivision == account.subdivision && this->salary == account.salary && this->job_title == account.job_title && this->full_name == account.full_name);
        }

        bool operator != (const Accountant& account) const
        {
            return !(*this == account);
        }

        bool operator <= (const Accountant& account) const
        {
            return *this == account || *this < account;
        }

        bool operator > (const Accountant& account) const
        {
            return !(*this <= account);
        }

        bool operator >= (const Accountant& account) const
        {
            return !(*this < account);
        }
};

std::ostream& operator << (std::ostream& os, const Accountant& account)
{
    return os << account.full_name << "; " << account.subdivision << "; " << account.job_title << "; " << account.salary;
}

std::istream& operator >> (std::istream& is, Accountant& account)
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

/**
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
void readFromFile(std::fstream& myFile, Accountant accountants[], unsigned int sample_size)
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

/**
 * @brief Сортировка вставками
 *
 * Просто функция сортировки вставками
 *
 * @param a[] Массив элементов, который надо отсортировать
 * @param size Размер массива
 *
 * @return Ничего. Массив сортируется внутри
 */
template<class T>
void insertionSort(T a[], long size)
{
    for (long i = 0; i < size; ++i)
    {
        T tmpElem = a[i];
        for (long j = i - 1; j >= 0 && a[j] > tmpElem; --j)
        {
            a[j + 1] = a[j];
            a[j] = tmpElem;
        }
    }
}

/**
 * @brief Шейкер-Сортировка
 *
 * Функция Шейкер-сортировки
 *
 * @param a[] Массив элементов, который надо отсортировать
 * @param size Размер массива
 *
 * @return Ничего. Массив сортируется внутри
 */

template<class T>
void shakerSort(T a[], long size)
{
    size_t k = size - 1, j;
    T tmpObj;
    size_t lb = 1, ub = size - 1;
    do
    {
        for (j = lb; j <= ub; j++)
        {
            if (a[j - 1] > a[j])
            {
                tmpObj = a[j];
                a[j] = a[j - 1];
                a[j - 1] = tmpObj;
                k = j;
            }
        }

        ub = k - 1;

        for (j = ub; j > lb; j--)
        {
            if (a[j - 1] > a[j])
            {
                tmpObj = a[j];
                a[j] = a[j - 1];
                a[j - 1] = tmpObj;
                k = j;
            }
        }

        lb = k + 1;
    } while (lb < ub);
}

/**
 * @brief Вспомогательная функция слияния
 *
 * В этой функции идёт слияние двух массивов (хоть он и один, но его можно представить как 2, разделённых индексом mid) в один отсортированный
 *
 * @param a[] Массив элементов, который надо отсортировать
 * @param low Граница начала минимального массива
 * @param mid Граница конца минимального массива и начала максимального
 * @param high Граница конца максимального массива
 *
 * @return Ничего. Массив сортируется внутри
 */
template<class T>
void merge(T a[], long low, long mid, long high)
{
    T* b = new T[high + 1 - low];
    long h, i, j, k;
    h = low;
    j = mid+1;
    i = 0;
    while ((h <= mid) && (j <= high))
    {
        if (a[h] < a[j])
        {
            b[i] = a[h]; // Пихаем минимальный элемент в новый массив
            h++;
        }
        else
        {
            b[i] = a[j]; // Пихаем минимальный элемент в новый массив
            j++;
        }
        i++;
    }

    // Если в каком-то случае осталась неотсортированная часть массива 
    // (Это может быть в случае, если mid - low != high - mid + 1), то её надо дополнить. 
    // Вопрос: как? Массив уже отсортирован, так что мы просто пихаем эту часть в конец b
    if (j <= high)
    {
        for (k = j; k <= high; ++k)
        {
            b[i] = a[k];
            i++;
        }
    }
    else if (h <= mid)
    {
        for (k = h; k <= mid; ++k)
        {
            b[i] = a[k];
            i++;
        }
    }

    // Теперь просто скопируем результат слияния из b в a
    for (k = 0; k <= high - low; ++k)
    {
        a[k + low] = b[k];
    }
    delete[] b;
}

/**
 * @brief Сортировка слиянием
 *
 * Просто функция сортировки вставками
 *
 * @param a[] Массив элементов, который надо отсортировать
 * @param size Размер массива
 *
 * @return Ничего. Массив сортируется внутри
 */
template<class T>
void mergeSort(T a[], long low, long high)
{
    if (low < high)
    {
        long mid = (low + high) / 2;
        mergeSort(a, low, mid);
        mergeSort(a, mid+1, high);
        merge(a, low, mid, high);
    }
}

template<class T>
void mergeSortAdapter(T a[], long size)
{
    mergeSort(a, 0, size - 1);
}


/**
 * @brief Подсчёт времени
 *
 * Функция для вычисления времени работы алгоритма
 *
 * @param func Функция с алгоритмом сортировки
 * @param a[] Массив элементов, который надо отсортировать
 * @param size Размер массива
 *
 * @return Время работы алгоритма
 */
template<class T>
double calc_algo_time(void (*func)(T[], long), T a[], long size)
{
    clock_t start = clock();
    func(a, size);
    clock_t end = clock();
    return (double)(end - start) / (double)(CLOCKS_PER_SEC);
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage ./lab1 <count of files in tests directory>";
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

        std::fstream outFile;
        std::string outFilename = "results/result_" + std::to_string(test_id) + ".txt";
        outFile.open(outFilename, std::ios::out);
        if (!outFile)
        {
            std::cerr << "Unable to open output file: " << outFilename;
            return 500;
        }

        unsigned int sample_size;
        inFile >> sample_size;
        inFile.ignore(); // Чтобы пропустить символ переноса строки из буффера
        Accountant* accountants = new Accountant[sample_size];
        Accountant* accountantsBackup = new Accountant[sample_size];

        readFromFile(inFile, accountants, sample_size);

        // Создаём бэкап для восстановления массива
        std::copy(accountants, accountants + sample_size, accountantsBackup);

        // Считаем время сортировки вставками
        double insertion_algo_time = calc_algo_time(insertionSort, accountants, sample_size);
        std::cout << "Сортировка ВСТАВКАМИ под номером " << test_id << " " << sample_size << " элементов " << " заняла: " << insertion_algo_time << " секунд" << std::endl;

        // Возвращаем исходный массив
        std::copy(accountantsBackup, accountantsBackup + sample_size, accountants);

        // Считаем время Шейкер-сортировки
        double shaker_algo_time = calc_algo_time(shakerSort, accountants, sample_size);
        std::cout << "Шейкер-сортировка под номером " << test_id << " " << sample_size << " элементов " << " заняла: " << shaker_algo_time << " секунд" << std::endl;

        // Возвращаем исходный массив
        std::copy(accountantsBackup, accountantsBackup + sample_size, accountants);

        // Считаем время сортировки слиянием
        double merge_algo_time = calc_algo_time(mergeSortAdapter, accountants, sample_size);
        std::cout << "Сортировка слиянием под номером " << test_id << " " << sample_size << " элементов " << " заняла: " << merge_algo_time << " секунд" << std::endl;

        std::copy(accountantsBackup, accountantsBackup + sample_size, accountants);

        clock_t start = clock();
        std::sort(accountants, accountants+sample_size);
        clock_t end = clock();
        double real_time = (double)(end - start) / (double)(CLOCKS_PER_SEC);

        std::cout << "Встроенная сортировка под номером " << test_id << " " << sample_size << " элементов " << " заняла: " << real_time << " секунд" << std::endl << std::endl;

        analyticsFile << sample_size << " " << insertion_algo_time << " " << shaker_algo_time << " " << merge_algo_time << " " << real_time << std::endl;

        for (int i = 0; i < sample_size; ++i)
            outFile << "Сотрудник " << i + 1 << ": " << accountants[i] << std::endl;

        delete [] accountantsBackup;
        delete [] accountants;
    }
    return 0;
}