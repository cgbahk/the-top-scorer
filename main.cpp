#define LOCAL_TEST
#include <iostream>
#include <cassert>
typedef long long ll;
using namespace std;

const static int P = 998244353;

// class for Modulo P operation
struct Mod
{
  Mod()
  {
    // Do nothing
  }
  Mod(int val)
  {
    _val = val % P;
  }

  Mod(ll val)
  {
    _val = (int) ( val % (ll)P );
  }

  int val(){ return _val; }

  Mod operator+(Mod rhs)
  {
    int temp = this->val() + rhs.val();
    temp %= P;
    return Mod(temp);
  }

  Mod operator-(Mod rhs)
  {
    int temp = this->val() - rhs.val() + P;
    temp %= P;
    return Mod(temp);
  }

  Mod operator*(Mod rhs)
  {
    ll temp = ( (ll)(this->val()) * (ll)(rhs.val()) ) % (ll)P;
    return Mod(temp);
  }

  Mod operator/(Mod rhs)
  {
    return (*this) * inverse(rhs);
  }

  bool operator==(Mod rhs)
  {
    return *this == rhs.val();
  }

  bool operator==(int rhs)
  {
    return (this->val() == rhs) ? true : false;
  }

  static Mod power(Mod arg, int pow)
  {
    if(pow == 1){ return arg; }
    if(pow % 2)
    { 
      Mod temp = power(arg, pow/2);
      return temp * temp * arg;
    }
    else
    { 
      Mod temp = power(arg, pow/2);
      return temp * temp;
    }
  }

  private:
  Mod inverse(Mod arg)
  {
    assert( arg.val() != 0 );
    return power(arg, P-2);
  }

  int _val;
};

// calculate (n choose k) in Mod
Mod combi(int n, int k)
{
  if(n < 2*k){ return combi(n, n-k); }

  Mod num(1), den(1);
  for(int i=0; i<k; i++)
  {
    num = num * Mod(n-i);
    den = den * Mod(i+1);
  }
  return num / den;
}

Mod calc(int len, int rng, int sum)
{
  if(len == 0 && sum != 0){ return Mod(0); }
  Mod ret(0);
  for(int i=0; i<=len && sum - i * (rng+1) >= 0; i++)
  {
    Mod temp(1);
    temp = temp * combi(len, i);
    temp = temp * combi(len + sum - i * (rng+1) - 1, len - 1);
    if(i % 2)
    {
      ret = ret - temp;
    }
    else
    {
      ret = ret + temp;
    }
  }
  return ret;
}

void test()
{
  // Mod test
  {
    assert( Mod(P-1) + Mod(P-1) == P-2 );
    assert( Mod(1) - Mod(2) == P-1 );
    assert( Mod(0) - Mod(P-1) == 1 );
    assert( Mod(P-1) * Mod(P-1) == 1 );
    assert( Mod(P-1) * Mod(P-1) * Mod(P-1) == P-1 );
    assert( Mod(1) / Mod(P-1) == P-1 );

    assert( Mod(7) / Mod(8) == 124780545);
    assert( Mod(1)/Mod(2) + Mod(1)/Mod(3) == Mod(5)/Mod(6) );
    assert( Mod(1)/Mod(2) - Mod(1)/Mod(3) == Mod(1)/Mod(6) );
    assert( (Mod(1)/Mod(2)) / Mod(3) == Mod(1)/Mod(6) );
  }

  // combi test
  {
    assert( combi(1, 1) == 1 );
    assert( combi(2, 1) == 2 );
    assert( combi(6, 0) == 1 );
    assert( combi(6, 2) == 15 );
    assert( combi(6, 3) == 20 );
    assert( combi(6, 4) == 15 );
    assert( combi(6, 6) == 1 );

    int n = 10000;
    Mod lhs(0);
    for (int i=0; i<n; i++)
    {
      lhs = lhs + combi(n, i);
    }
    Mod rhs = Mod::power(Mod(2), n);
    assert( lhs + Mod(1) == rhs );
  }

  // calc test
  {
    assert(calc(1, 3, 0) == 1);
    assert(calc(1, 3, 1) == 1);
    assert(calc(2, 3, 0) == 1);
    assert(calc(2, 3, 1) == 2);
    assert(calc(3, 3, 0) == 1);
    assert(calc(3, 3, 1) == 3);
    assert(calc(2, 3, 6) == 1);
    assert(calc(5, 3, 14) == 5);
    assert(calc(3, 5, 15) == 1);
    assert(calc(3, 5, 14) == 3);
    assert(calc(0, 1, 0) == 1);
    assert(calc(0, 1, 1) == 0);

    int x=100, y=2000, z=5000;
    assert( calc(x,y,z) == calc(x,y,z-1) + calc(x-1,y,z) - calc(x-1,y,z-y-1) );
  }
  cout << "Test passed" << endl;
}

void prob()
{
  int p, s, r;
  cin >> p >> s >> r;

  Mod num(0), den(0);

  // numerator
  for(int k=0; k<=s-r; k++)
  {
    for (int l=1; l<=p && s-(r+k)*l >= 0; l++)
    {
      Mod temp(1);
      temp = temp * calc( p-l, r+k-1, s-(r+k)*l );
      temp = temp * combi(p-1, l-1);
      temp = temp / Mod(l);

      num = num + temp;
    }
  }

  // denominator
  den = den + combi(p+s-r-1, p-1);

  cout << ( num / den ).val() << endl;
}

int main()
{
#ifdef LOCAL_TEST
  //  freopen("input.txt", "r", stdin);
#endif

  //test();
  prob();

  return 0;
}

