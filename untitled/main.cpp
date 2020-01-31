#include<iostream>
#include<vector>
#include<memory>
#include<cstdio>
#include<fstream>
#include<cassert>
#include<functional>
using namespace std;
struct B{
public:
    virtual void bar(){
        cout<<"B::bar"<<endl;
    }
    ~B()=default;
};
struct D:B{
public:
    D(){
        cout<<"D:D"<<endl;
    }
    ~D(){
        cout<<"D:~D"<<endl;
    }
    void bar() override{
        cout<<"D::bar"<<endl;
    }
};
unique_ptr<D> pass_throught(unique_ptr<D>p){
    p->bar();
    return p;
}
void close_file(FILE *fp){
    fclose(fp);
}
int main(){
    cout<<"unique ownershio semantics demon"<<endl;
    {
    auto p=make_unique<D>();
    auto q=pass_throught(move(p));
    assert(!p);
    q->bar();
    }
    cout<<"Runtime polymorphism demo"<<endl;
    {
        unique_ptr<B> p=make_unique<D>();
        p->bar();
        vector<unique_ptr<B>>v;
        v.push_back(make_unique<D>());
        v.push_back(move(p));
        v.emplace_back(new D);
        v.emplace_back(new B);
        for(auto &i:v){
            i->bar();
        }
    }

}





