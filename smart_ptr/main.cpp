#include <iostream>
/*
    最早auto_ptr实现
    智能指针最基本的功能：对超出作用域的对象进行释放  从根本上消除资源泄漏的可能性
    auto_ptr 是通过由 new 表达式获得的对象，并在 auto_ptr 自身被销毁时删除该对象的智能指针。它可用于为动态分配的对象提供异常安全、传递动态分配对象的所有权给函数和从函数返回动态分配的对象。
    复制 auto_ptr ，会复制指针并转移所有权给目标： auto_ptr 的复制构造和复制赋值都会修改其右侧参数，而且“副本”不等于原值。因为这些不常见的复制语义，不可将 auto_ptr 置于标准容器中。此用途及其他使用更适合用 std::unique_ptr 。 (C++11 起)
*/

using namespace std;
template<typename T>
class smart_ptr{
public:
    explicit smart_ptr(T * ptr=nullptr):m_ptr(ptr){

    }
    template<typename U>
    smart_ptr(smart_ptr<U> &&other){
        m_ptr=other.relese(); //类型转换
    }
    smart_ptr(smart_ptr && ptr){
        m_ptr=ptr.relese();
    }
    smart_ptr &operator=( smart_ptr ptr){
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
