#define LOCAL_TEST
#include <iostream>
#include <cassert>
typedef long long ll;
using namespace std;

const static int PL = 100, S = 5000;
const static int P = 998244353;

int invmemo[PL+1];

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

private:
  Mod inverse(Mod arg)
  {
    assert( arg.val() != 0 );

    // TODO may remove invmemo, as it is not very effective
    if(arg.val() <= 100)
    {
      if(invmemo[arg.val()] != 0)
      {
        return Mod(invmemo[arg.val()]);
      }
      Mod inv = power(arg, P-2);
      invmemo[arg.val()] = inv.val();
      return inv;
    }
    return power(arg, P-2);
  }

  Mod power(Mod arg, int pow)
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

  int _val;
};

struct RngdSum
{
  // Initialize memo to -1, i.e. not visited(calculated)
  RngdSum(int rng) : _rng(rng)
  {
    for (int len=0; len<PL; len++)
    {
      for (int sum=0; sum<S; sum++)
      {
        _memo[len][sum] = Mod(P-1); // TODO is this safe?
      }
    }
  }

  /**
   * calculate # of combination of sum of ranged integers
   *
   * Returns # of possible RngdSumnations of a[] hold following:
   * a[1] + a[2] + ... + a[len] = sum where a[i] in [0, _rng]
   *
   * TODO faster log n dp - this seems (much) slower
   */
  Mod calc(int len, int sum)
  {
    if(!(_memo[len][sum] == P-1))
    {
      return _memo[len][sum];
    }

    if(2*sum > len * _rng)
    {
      return calc(len, len * _rng - sum);
    }

    if(len * _rng < sum)
    {
      _memo[len][sum] = Mod(0);
      return 0;
    }

    if(sum < 0)
    {
      return 0;
    }

    if(sum == 0)
    {
      _memo[len][sum] = Mod(1);
      return 1;
    }

    if(len == 1)
    {
      if(_rng >= sum)
      {
        _memo[len][sum] = Mod(1);
        return 1;
      }
      else
      {
        _memo[len][sum] = Mod(0);
        return 0;
      }
    }

    Mod val = calc( len, sum-1 )
            + calc( len-1, sum )
            - calc( len-1, sum-_rng-1 );
    _memo[len][sum] = val;
    return val;
  }

private:
  Mod _memo[PL][S];
  int _rng;
};

// calculate (n choose k) in Mod
Mod combi(int n, int k)
{
  if(n < 2*k){ return combi(n, n-k); }

  Mod ret(1);
  for(int i=0; i<k; i++)
  {
    ret = ret * Mod(n-i);
    ret = ret / Mod(i+1);
  }
  return ret;
}

Mod calc(int len, int rng, int sum)
{
  Mod ret(0);
  for(int i=0; i<=len; i++)
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
  // RngdSum test
  {
    RngdSum cb3(3);
    assert(cb3.calc(1, 0) == 1);
    assert(cb3.calc(1, 1) == 1);
    assert(cb3.calc(2, 0) == 1);
    assert(cb3.calc(2, 1) == 2);
    assert(cb3.calc(3, 0) == 1);
    assert(cb3.calc(3, 1) == 3);
    assert(cb3.calc(2, 6) == 1);
    assert(cb3.calc(5, 14) == 5);

    RngdSum cb5(5);
    assert(cb5.calc(3, 15) == 1);
    assert(cb5.calc(3, 14) == 3);

    RngdSum cb(2000);
    assert(cb.calc(100, 5000) == 104171511);
  }
  
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
    assert( combi(6, 2) == 15 );
    assert( combi(6, 3) == 20 );
    assert( combi(6, 4) == 15 );
    combi(5000, 2500); // for speed concern
  }

  // calc test
  {
    assert( calc(3, 5, 14) == 3 );
    assert( calc(5, 3, 14) == 5 );
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
    RngdSum rs(r+k-1);
    for (int l=1; l<=p && s-(r+k)*l >= 0; l++)
    {
      Mod temp(1);
      temp = temp * rs.calc( p-l, s-(r+k)*l );
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

  test();
  //prob();

  return 0;
}

