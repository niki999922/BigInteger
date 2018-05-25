#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "BigInteger.cpp"

using namespace std;

void test_new() {
    freopen("test.txt", "r", stdin);
    const int new_base = 27100;
    BigInteger<int64_t> a, b;
    BigInteger<int16_t, new_base> c;
    BigInteger<int16_t, new_base> d;

    cout << "Without cast :\n";
    cin >> a >> b;
    cout << "a = " << a << '\n' << "b = " << b << '\n';
    cout << "a + b = " << a + b << "\n";
    cout << "a - b = " << a - b << "\n";
    cout << "a * b = " << a * b << "\n\n";
    cin >> a >> b;
    cout << "a = " << a << '\n' << "b = " << b << '\n';
    cout << "a + b = " << a + b << "\n";
    cout << "a - b = " << a - b << "\n";
    cout << "a * b = " << a * b << "\n\n";
    cin >> a >> b;
    cout << "a = " << a << '\n' << "b = " << b << '\n';
    cout << "a + b = " << a + b << "\n";
    cout << "a - b = " << a - b << "\n";
    cout << "a * b = " << a * b << "\n\n";
    cin >> a >> b;
    cout << "a = " << a << '\n' << "b = " << b << '\n';
    cout << "a + b = " << a + b << "\n";
    cout << "a - b = " << a - b << "\n";
    cout << "a * b = " << a * b << "\n\n";
    cout << '\n';

    cout << "With cast:\n";
    cin >> a >> b;
    c = static_cast< BigInteger<short, new_base> >(a);
    d = static_cast< BigInteger<short, new_base> >(b);
    cout << "c = " << c << '\n' << "d = " << d << '\n';
    cout << "c + d = " << c + d << "\n";
    cout << "c - d = " << c - d << "\n";
    cout << "c * d = " << c * d << "\n\n";
    cin >> a >> b;
    c = static_cast< BigInteger<short, new_base> >(a);
    d = static_cast< BigInteger<short, new_base> >(b);
    cout << "c = " << c << '\n' << "d = " << d << '\n';
    cout << "c + d = " << c + d << "\n";
    cout << "c - d = " << c - d << "\n";
    cout << "c * d = " << c * d << "\n\n";
    cin >> a >> b;
    c = static_cast< BigInteger<short, new_base> >(a);
    d = static_cast< BigInteger<short, new_base> >(b);
    cout << "c = " << c << '\n' << "d = " << d << '\n';
    cout << "c + d = " << c + d << "\n";
    cout << "c - d = " << c - d << "\n";
    cout << "c * d = " << c * d << "\n\n";
    cin >> a >> b;
    c = static_cast< BigInteger<short, new_base> >(a);
    d = static_cast< BigInteger<short, new_base> >(b);
    cout << "c = " << c << '\n' << "d = " << d << '\n';
    cout << "c + d = " << c + d << "\n";
    cout << "c - d = " << c - d << "\n";
    cout << "c * d = " << c * d << "\n\n";
    cout << '\n';

    cout << "Long without cast:\n";
    cin >> a >> b;
    cout << "a = " << a << '\n' << "b = " << b << "\n\n";
    cout << "a + b = 1234152316940361952954414644190402923710\n        " << a + b << "\n\n";
    cout << "a - b = -1234152316937892817291952180512144303464\n        " << a - b << "\n\n";
    cout << "a * b = 1523644749332538364895763877423118962868862248661600306672589441201\n        " << a * b << "\n\n";

    cout << "Long with cast:\n";
    c = static_cast< BigInteger<short, new_base> >(a);
    d = static_cast< BigInteger<short, new_base> >(b);
    cout << "c = " << c << '\n' << "d = " << d << "\n\n";
    cout << "c + d = 1234152316940361952954414644190402923710\n        " << c + d << "\n\n";
    cout << "c - d = -1234152316937892817291952180512144303464\n        " << c - d << "\n\n";
    cout << "c * d = 1523644749332538364895763877423118962868862248661600306672589441201\n        " << c * d << "\n\n";


    cout << "Long without cast:\n";
    cin >> a >> b;
    cout << "a = " << a << '\n' << "b = " << b << "\n\n";
    cout << "a + b = 12353459712491242451242294847648631321234913807973096784819824718436913561432726921436\n        " << a + b << "\n\n";
    cout << "a - b = -12353459712491242451241603401376238582865877398533404579880799906187711063191897703188\n        " << a - b << "\n\n";
    cout << "a * b = 4270876834677969522500094762817072672936457890033527291137547544964781535122345878196264091992410558958662801368319919634434563852977387662092734688\n        " << a * b << "\n\n";

    cout << "Long with cast:\n";
    c = static_cast< BigInteger<short, new_base> >(a);
    d = static_cast< BigInteger<short, new_base> >(b);
    cout << "c = " << c << '\n' << "d = " << d << "\n\n";
    cout << "c + d = 12353459712491242451242294847648631321234913807973096784819824718436913561432726921436\n        " << c + d << "\n\n";
    cout << "c - d = -12353459712491242451241603401376238582865877398533404579880799906187711063191897703188\n        " << c - d << "\n\n";
    cout << "c * d = 4270876834677969522500094762817072672936457890033527291137547544964781535122345878196264091992410558958662801368319919634434563852977387662092734688\n        " << c * d << "\n\n";
}

template<typename T, size_t Base>
void BI_out(BigInteger<T, Base> &a, BigInteger<T, Base> &b, string s, vector<string> ans) {
    ofstream fou(s+".txt", ios::app);
    fou << "______________________________________\n";
    fou << "a = " << a << '\n';
    fou << "b = " << b << "\n\n";
    cout << "\'a\' and \'b\' output complete.\n";
    fou << "From real BigInt: a + b = " << ans.at(0) << "\n";
    fou << "From my BigInt:   a + b = " << a + b << "\n\n";
    cout << "Add complete.\n";
    fou << "From real BigInt: a - b = " << ans.at(1) << "\n";
    fou << "From my BigInt:   a - b = " << a - b << "\n\n";
    cout << "Substract complete.\n";
    fou << "From real BigInt: a * b = " << ans.at(2) << "\n";
    fou << "From my BigInt:   a * b = " << a * b << "\n";
    cout << "Mul complete.\n";
    fou.close();
}


void test_pro(string s) {
    ifstream fin(s + ".txt");
    BigInteger<int64_t> a, b;
    int n;
    string where = "result", tmp;
    ofstream fou(where + ".txt", ios::app);
    fou << "\n-------------------------------------\n" << s << " test:\n-------------------------------------\n";
    fou.close();
    fin >> n;
    for (size_t i = 0; i < n; ++i) {
        cout << "Start number " << i + 1 << '\n';
        vector<string> ans;
        fin >> a >> b;
        for (size_t j = 0; j < 3; ++j) {
            fin >> tmp;
            ans.push_back(tmp);
        }
        BI_out(a, b, where, ans);
        cout << "End." << '\n';
    }
}

void clear_file(string s) {
    ifstream fin(s + ".txt");
}

//для Just_super_long_number.txt если будет не лень закину в java BigInt и посичтаю *, ибо сервисы падают в инете((((
int main() {
    //clear_file("result");
    //test_new();
    //test_pro("test1");
    //test_pro("dead_test");
    test_pro("Just_super_long_number");
    return 0;
}