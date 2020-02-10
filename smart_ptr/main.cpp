#include <iostream>
/*
    unique_ptr的实现
*/

using namespace std;
//共享简化版  线程非安全版本
class shared_count
{
public:
    shared_count():m_count(1){}//初始化
    void add_count(){
       m_count++;
    }//增加计数
    long reduce_count(){
        --m_count;
        return m_count;
    }//减少计数
    long get_count()const{
        return m_count;
    }//获取计数
private:
    long m_count;
};


template<typename T>
class smart_ptr{
public:
    explicit smart_ptr(T * ptr=nullptr):m_ptr(ptr){
        if(ptr){

            m_ref_count=new shared_count();

        }
    }
    template<typename U>
    smart_ptr(const smart_ptr<U> &other){
        m_ptr=other.m_ptr;
        if(m_ptr){
           other.m_ref_count->add_count();
           m_ref_count=other.m_ref_count;
        }
    }
    template<typename U>
    smart_ptr(smart_ptr<U> &&other){
        m_ptr=other.m_ptr;
        if(m_ptr){
           m_ref_count=other.m_ref_count;
           other.m_ptr=nullptr;
        }
    }
    smart_ptr(const smart_ptr & ptr){
        m_ptr=ptr.m_ptr;
        if(m_ptr){
           ptr.m_ref_count->add_count();
           m_ref_count=ptr.m_ref_count;
        }
    }
    smart_ptr &operator=( smart_ptr ptr){
        smart_ptr(ptr).swap(*this);   //构造临时对象  交换指针
        return  *this;
    }
    ~smart_ptr(){
        if(!m_ref_count->reduce_count() && m_ptr){
            delete  m_ref_count;
            delete  m_ptr;
        }
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
        std::swap(m_ref_count,rhs.m_ref_count);
    }
    long use_count(){
        if(m_ptr){
            return m_ref_count->get_count();
        }
        else{
            return 0;
        }
    }
private:
    T * m_ptr;
    shared_count * m_ref_count;
};

int main()
{
    int *b=new int(1);
    smart_ptr<int>tempA(b);

    cout<<tempA.use_count();
    {
       smart_ptr<int>tempB(tempA);
       cout<<tempB.use_count();
    }
    smart_ptr<int>tempB(tempA);
    cout<<tempB.use_count();
    return 0;
}
