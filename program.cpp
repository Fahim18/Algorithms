#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

using namespace std;

string product(string x, string y);
string powr(int n);
string add(string p, string q);
string actual_add(string v, string w);

int main() {

	string first, last;
	first = "3141592653589793238462643383279502884197169399375105820974944592";
	last = "2718281828459045235360287471352662497757247093699959574966967627";
    cout << product(first, last) << endl;
    return 0;
}

string product(string x, string y) {
    int n = x.size();

    if( n == 1 ) {
        int x1 = x[0] - '0';
        int y1 = y[0] - '0';
        int multiplied = x1*y1;
        stringstream ss;
        ss << multiplied;
        string pro = ss.str();
        return pro;
    }

    int middle = n/2;
    int length = middle + 1;

    char a[length], b[length], c[length], d[length];

    for(int i = 0; i < middle; i++) {
        a[i] = x[i];
    }

    for(int i = middle; i < n; i++) {
        b[i - middle] = x[i];
    }

    for(int i = 0; i < middle; i++) {
        c[i] = y[i];
    }

    for(int i = middle; i < n; i++) {
        d[i - middle] = y[i];
    }

    a[middle] = '\0';
    b[middle] = '\0';
    c[middle] = '\0';
    d[middle] = '\0';


    string a_c = product(a, c);
    string bd = product(b, d);
    string ad = product(a, d);
    string bc = product(b, c);

    string ac = a_c + powr(n);
    string adbc = add(ad, bc);
    string ad_bc = adbc + powr(middle);

    return add(ac, add(ad_bc, bd));
}

string powr(int n) {
    if(n == 0) {
        return "";
    }
    string power = "0";
    for(int i = 0; i < (n-1); i++) {
        power = power + "0";
    }
    return power;
}

string add(string p, string q) {
    int p_len = p.size();
    int q_len = q.size();

    if( p_len >= q_len ) {
        int diff = p_len - q_len;

        char p_inverse[p_len + 1];
        char q_inverse[q_len + 1];

        for(int i = 0; i < p_len; i++) {
            p_inverse[i] = p[p_len - 1 - i];
        }

        p_inverse[p_len] = '\0';
        string sp_inverse(p_inverse);

        for(int i = 0; i < q_len; i++) {
            q_inverse[i] = q[q_len - 1 - i];
        }

        q_inverse[q_len] = '\0';
        string sq_inverse(q_inverse);

        sq_inverse = sq_inverse + powr(diff);

        return actual_add(sp_inverse, sq_inverse);
    }

    else {
        int diff = q_len - p_len;

        char p_inverse[p_len + 1];
        char q_inverse[q_len + 1];

        for(int i = 0; i < p_len; i++) {
            p_inverse[i] = p[p_len - 1 - i];
        }

        p_inverse[p_len] = '\0';
        string sp_inverse(p_inverse);

        for(int i = 0; i < q_len; i++) {
            q_inverse[i] = q[q_len - 1 - i];
        }

        q_inverse[q_len] = '\0';
        string sq_inverse(q_inverse);

        sp_inverse = sp_inverse + powr(diff);

        return actual_add(sp_inverse, sq_inverse);
    }

}

string actual_add(string v, string w) {
    int length = v.size();

    char v_w[length + 2];
    int carry = 0;

    for(int i = 0; i < length; i++) {
        int v1 = v[i] - '0';
        int w1 = w[i] - '0';

        int v1w1 = ( v1 + w1 ) + carry;

        stringstream vw;
        vw << v1w1;
        string vw_added = vw.str();

        if ( vw_added.size() == 1 ) {
            v_w[i] = vw_added[0];
            carry = 0;
        }
        else {
            v_w[i] = vw_added[1];
            carry = vw_added[0] - '0';
        }
    }

    stringstream lc;
    lc << carry;
    string last_carry = lc.str();

    v_w[length] = last_carry[0];
    v_w[length + 1] = '\0';

    char v_w_added[length + 2];

    int j = 0;

    for(int i = 0; i < (length + 1); i++) {
        v_w_added[i] = v_w[length - i];
    }

    int k = 0;
    if ( v_w_added[k] == '0' ) {
        k++;
    }

    char returning[length + 2 - k];

    for( int i = k; i < (length + 1); i++ ) {
        returning[i - k] = v_w_added[i];
    }

    returning[length - k + 1] = '\0';
    string truereturn(returning);
    return truereturn;
}
