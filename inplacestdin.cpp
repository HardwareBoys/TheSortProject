#include <stdio.h>
#include <fstream>
#include <iostream>

#define SIZE 100
 
int compare(int idx1, int idx2);
void exchange(int idx1, int idx2);
void assign(int idx, int val);
int item(int idx);

int lower (int from, int to, int val);
int upper (int from, int to, int val);
void insertsort (int from, int to);
int gcd (int m, int n);
void rotate (int from, int mid, int to);
void merge(int from, int pivot, int to, int len1, int len2);
void mergesort(int from, int to);
void PrintArr(int* arr, int size);
int* LoadedArr(char* name);

using namespace std;

int* MyArr;
int arrSize;

int* LoadedArr(char* name)
{
    std::fstream myfile(name, std::ios_base::in);
    int* arr;
    int n;
    int i=0;
    myfile >> arrSize;
    arr = new int[arrSize];
    for (int i=0; i<arrSize; i++)
    {
      myfile >> arr[i];
    }
    if ( (myfile >> n ) != false )
      cout << "Something was not read !!!" << endl;
    return arr;
}

void PrintArr(int* arr, int size)
{
  for (int i=0; i<size; i++)
    cout << arr[i] << " ";
}

int main ( int argc, char *argv[] )
{
  bool runMergeSort = false;
  if (argc != 2) 
  {
    cout << "Invalid args ";
    return -1;
  } 
  if (strcmp("--merge", argv[1]) ==0)
    runMergeSort = true;
  else if (strcmp("--insert", argv[1]) ==0)
    {}
  else 
  {
    cout << "invalid args";
    return -1;
  }

  int i=0;
  cin >> arrSize;
  MyArr = new int[arrSize];
  for (int i=0; i<arrSize; i++) 
    cin >> MyArr[i];
  cout << "reading finished, I read this : " << std::endl;
  PrintArr(MyArr, arrSize );
  if (runMergeSort)
  {
    cout << endl <<"going to run merge sort" << endl;
    mergesort(0, arrSize);
  }
  else
  {
    cout << endl << "going to run inplace sort" << endl;
    insertsort(0, arrSize);
  }

  cout << "sort finished" << endl;
  PrintArr(MyArr, arrSize );
}


int compare(int idx1, int idx2)
{
  return MyArr[idx1] - MyArr[idx2];
}

void assign(int idx, int val)
{
  MyArr[idx] =val;
}

int item(int idx)
{
  return MyArr[idx];
}


void exchange(int idx1, int idx2)
{
  int tmp = MyArr[idx1];
  MyArr[idx1] = MyArr[idx2];
  MyArr[idx2] = tmp;
}

int lower (int from, int to, int val) 
{ 
  int len = to - from, half;
  while (len > 0) 
  {
    half = len/2;
    int mid= from + half;
    if (compare (mid, val) < 0) 
    {
      from = mid+1;
      len = len - half -1;
    } 
    else len = half;
  }
  return from;
}

int upper (int from, int to, int val) 
{
  int len = to - from, half;
  while (len > 0) 
  {
    half = len/2;
    int mid= from + half;
    if (compare (val, mid) < 0)
    len = half;
    else 
    {
      from = mid+1;
      len = len - half -1;
    }
  }
  return from;
}

void insertsort (int from, int to) 
{
  if (to > from+1) 
  {
    for (int i = from+1; i < to; i++) 
    {
      for (int j = i; j > from; j--) 
      {
        if (compare(j, j-1)<0)
        exchange(j, j-1);
        else break;
      }
    }
  }
}

int gcd (int m, int n) 
{
  while (n!=0) { int t = m % n; m=n; n=t; }
  return m;
}

void reverse (int from, int to) 
{
  while (from < to) 
  {
    exchange(from++, to--);
  }
}

void rotate (int from, int mid, int to) 
{
  /*  a less sophisticated but costlier version:
  reverse(from, mid-1);
  reverse(mid, to-1);
  reverse(from, to-1);
  */
  if (from==mid || mid==to) return;
  int n = gcd(to - from, mid - from);
  while (n-- != 0) 
  {
    int val = item(from+n);
    int shift = mid - from;
    int p1 = from+n, p2=from+n+shift;
    while (p2 != from + n) 
    {
      assign(p1, item(p2));
      p1=p2;
      if ( to - p2 > shift) p2 += shift;
      else p2=from + (shift - (to - p2));
    }
    assign(p1, val);
  }
}

void merge(int from, int pivot, int to, int len1, int len2) 
{
  if (len1 == 0 || len2==0) return;
  if (len1+len2 == 2) 
  {
    if (compare(pivot, from) < 0)
    exchange(pivot, from);
    return;
  }
  int first_cut, second_cut;
  int len11, len22;
  if (len1 > len2) 
  {
    len11=len1/2;
    first_cut = from + len11;
    second_cut = lower(pivot, to, first_cut);
    len22 = second_cut - pivot;
  } 
  else 
  {
    len22 = len2/2;
    second_cut = pivot + len22;
    first_cut = upper(from, pivot, second_cut);
    len11=first_cut - from;
  }
  rotate(first_cut, pivot, second_cut);
  int new_mid=first_cut+len22;
  merge(from, first_cut, new_mid, len11, len22);
  merge(new_mid, second_cut, to, len1 - len11, len2 - len22);
}
 
void mergesort(int from, int to) 
{
  if (to - from < 12) 
  {
   insertsort (from, to);
   return;
  }
  int middle = (from + to)/2;
  mergesort (from, middle);
  mergesort (middle, to);
  merge(from, middle, to, middle-from, to - middle);
 }
 