#include <iostream>
/*
    仿shared_ptr实现
    智能指针最基本的功能：对超出作用域的对象进行释放  从根本上消除资源泄漏的可能性
*/

using namespace std;
template<typename T>
class smart_ptr{
public:
    explicit smart_ptr(T * ptr=nullptr):m_ptr(ptr){

    }
    smart_ptr(smart_ptr & ptr){
        m_ptr=ptr.relese();
    }
    smart_ptr &operator=( smart_ptr &ptr){
        smart_ptr(ptr).swap(*this);   //构造临时对象  交换指针
        return  *this;
    }
    ~smart_ptr(){
        delete  m_ptr;
    }
    T * get(){
        return m_ptr;
    }
    T & operator *()const {
        return *m_ptr;
    }
    T*  operator ->()const {
        return m_ptr;
    }
    operator bool ()const {
        return m_ptr;
    }
    T* relese(){
        T* Ptr=m_ptr;
        m_ptr=nullptr;
        return Ptr;
    }
    void swap(smart_ptr &rhs){
        std::swap(m_ptr,rhs.m_ptr);
    }
private:
    T * m_ptr;
};

int main()
{

    int *b=new int(1);
    smart_ptr<int>tempA(b);
    cout<<*tempA;
    return 0;
}
