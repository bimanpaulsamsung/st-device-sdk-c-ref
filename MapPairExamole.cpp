#include <iostream>
#include <iterator>
#include <map>

using namespace std;

struct ComplexNum {
	int real;
	int img;

public:
	ComplexNum(int a, int b) {
		real = a;
		img = b;
	}
};

int main()
{
    // empty map container
    map<void *, ComplexNum *> gquiz1;
    ComplexNum Na(1,1);
    ComplexNum Nb(2,2);
    ComplexNum Nc(3,3);
    void *a = (void*)1;
    void *b = (void*)2;
    void *c = (void*)3;

    // insert elements in random order
    gquiz1.insert(pair<void *, ComplexNum *>(a, &Na));
    gquiz1.insert(pair<void *, ComplexNum *>(b, &Nb));
    gquiz1.insert(pair<void *, ComplexNum *>(c, &Nc));

    // printing map gquiz1
    map<void *, ComplexNum *>::iterator itr;
    cout << "\nThe map gquiz1 is : \n";
    cout << "\tKEY\tELEMENT\n";
    for (itr = gquiz1.begin(); itr != gquiz1.end(); ++itr) {
    	pair<void *, ComplexNum *> P = *itr;
    	void *p = P.first;
    	ComplexNum *n = P.second;
        cout << '\t' << "as"
             << '\t' << n->real << '\n';
    }
    cout << endl;


    // remove all elements up to
    // element with key=2 in gquiz1
    cout << "\ngquiz1 after removal of"
            " elements less than key=3 : \n";
    cout << "\tKEY\tELEMENT\n";
    gquiz1.erase(gquiz1.begin(), gquiz1.find(b));

    cout << "\nThe map gquiz1 is : \n";
    cout << "\tKEY\tELEMENT\n";
    for (itr = gquiz1.begin(); itr != gquiz1.end(); ++itr) {
    	pair<void *, ComplexNum *> P = *itr;
    	void *p = P.first;
    	ComplexNum *n = P.second;
        cout << '\t' << "as"
             << '\t' << n->real << '\n';
    }
    cout << endl;

    // remove all elements with key = 3
    int num;
    num = gquiz1.erase(c);
    cout << "\ngquiz1.erase(3) : ";
    cout << num << " removed \n";
    cout << "\nThe map gquiz1 is : \n";
    cout << "\tKEY\tELEMENT\n";
    for (itr = gquiz1.begin(); itr != gquiz1.end(); ++itr) {
    	pair<void *, ComplexNum *> P = *itr;
    	void *p = P.first;
    	ComplexNum *n = P.second;
        cout << '\t' << "as"
             << '\t' << n->real << '\n';
    }

    cout << endl;

    return 0;
}
