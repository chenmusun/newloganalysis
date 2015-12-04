#include<iostream>
#include<map>
using namespace std;
int main(int argc, char *argv[])
{
        map<int,int> m;
        m.insert(make_pair(1,1));
        m.insert(make_pair(2,2));
        m.insert(make_pair(3,3));
        m.insert(make_pair(4,4));
        m.erase(5);
        return 0;
}
