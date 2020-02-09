#include <iostream>
/*
    仿shared_ptr实现
    智能指针最基本的功能：对超出作用域的对象进行释放
*/

using namespace std;
template<typename T>
class smart_ptr{
    explicit smart_ptr(T * ptr=nullptr):m_ptr(ptr){

    }
    ~smart_ptr(){
        delete  m_ptr;
    }
    T * get(){
        return m_ptr;
    }
private:
    T * m_ptr;
};

int main()
{
    cout << "Hello World!" << endl;
    return 0;
}
