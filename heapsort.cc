#include <iostream>
#include <vector>

using std::vector;
using std::cout;
using std::endl;

template<typename T>
void swap(T &lhs, T &rhs){
    T temp = rhs;
    rhs = lhs;
    lhs = temp;
}

template <typename T, typename Compare = std::less<T>>
class HeapSort
{
public:
 HeapSort(T *arr, size_t size, Compare com);
 void heapAdjust(size_t ,size_t ,Compare &com);
 void sort(Compare &com);
 void print();

private:
 vector<T> vec;
};

template<typename T, typename Compare>
HeapSort<T, Compare>::HeapSort(T *arr, size_t size, Compare com)
{
    for (size_t i = 0; i < size; ++i) { //将数组存入容器
        vec.push_back(arr[i]);
    }
    for (size_t i = 0; i < size/2; i--) {
        heapAdjust(i, size, com);
    }
    swap(vec[0], vec[size - 1]);
    sort(com);
}

template<typename T, typename Compare>
void HeapSort<T, Compare>::heapAdjust(size_t pos, size_t len, Compare &com)
{
    size_t dad = pos;//父结点
    size_t son = 2*pos;//左孩子节点
    while (son < dad) {
        if (son + 1 < len && com(vec[son], vec[son + 1])) {
            ++son;
        }
        if (com(vec[dad], vec[son])) {
            swap(vec[dad], vec[son]);
            dad = son;
            son = 2*dad + 1;
        }
        else 
        {
            break;
        }
    }
}


template<typename T, typename Compare>
void HeapSort<T, Compare>::sort(Compare &com)
{
    size_t len = vec.size();
    for (size_t i = len; i > 1; i--) {
        swap(vec[0], vec[i - 1]);
        heapAdjust(0, i, com);
    }
}


template<typename T, typename Compare>
void HeapSort<T, Compare>::print()
{
    for (auto elem : vec) {
        cout << elem << " ";
    } 
    cout << endl;
}

void test0()
{
int arr[11] = {17, 99, 88, 23, 7, 51, 9 ,36 ,34, 66, 74};
    HeapSort<int> heap(arr, 11, std::less<int>());
    heap.print();
}

int main()
{
    test0();
    return 0;
}


