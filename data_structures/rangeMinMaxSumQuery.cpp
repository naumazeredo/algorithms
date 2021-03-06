/**
 * For static array A (one that does not update very frequently), use
 * the Dynamic Programming solution!
 */
#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;

// Segment tree
// the segment tree is stored like a heap array
#define RANGE_SUM 0
#define RANGE_MIN 1
#define RANGE_MAX 2
vector<int> segment_tree;

void init_segment_tree(int N)
{
  // If original array size is N,
  // the required segment_tree array lenght is 2*2^(floor(log2(N)) + 1)
  int length = (int)(2 * pow(2.0, floor((log((double)N) / log(2.0)) + 1)));
  segment_tree.resize(length, 0);
}

void build_segment_tree(int code, int A[], int node, int b, int e)
{
  if (b == e) // as b == e, either one is fine
  {
    if (code == RANGE_SUM) segment_tree[node] = A[b]; // Store value of this cell
    else                   segment_tree[node] = b;    // If RANGE_MIN/MAX, store index
  }
  else // recursively compute the values in the left and right subtrees
  {
    int leftIdx = 2 * node,
        rightIdx = 2 * node + 1;
    build_segment_tree(code, A, leftIdx , b              , (b + e) / 2);
    build_segment_tree(code, A, rightIdx, (b + e) / 2 + 1, e          );
    int lContent = segment_tree[leftIdx],
        rContent = segment_tree[rightIdx];

    if (code == RANGE_SUM) // make this segment contains sum of left and right subtree
      segment_tree[node] = lContent + rContent;
    else // code == RANGE_MIN/MAX
    {
      int lValue = A[lContent],
          rValue = A[rContent];
      if (code == RANGE_MIN) segment_tree[node] = (lValue <= rValue) ? lContent : rContent;
      else                   segment_tree[node] = (lValue >= rValue) ? lContent : rContent;
    }
  }
}

int query(int code, int A[], int node, int b, int e, int i, int j)
{
  if (i > e || j < b) // if the current interval does not intersect query interval
    return -1;

  if (b >= i && e <= j) // if the current interval is inside query interval
    return segment_tree[node];

  // compute the minimum position in the left and right part of the interval
  int p1 = query(code, A, 2 * node    , b              , (b + e) / 2, i, j);
  int p2 = query(code, A, 2 * node + 1, (b + e) / 2 + 1, e          , i, j);

  // return the position where the overall minimum is
  if (p1 == -1) return p2; // can happen if we try to access segment outside query
  if (p2 == -1) return p1;

  if (code == RANGE_SUM)      return p1 + p2;
  else if (code == RANGE_MIN) return (A[p1] <= A[p2]) ? p1 : p2;
  else                        return (A[p1] >= A[p2]) ? p1 : p2;
}

void update(int code, int A[], int node, int b, int e, int index)
{
  if (index < b || index > e) // if the updated index is outside interval
    return;

  if (b == e) // as b == e, either one is fine
  {
    if (code == RANGE_SUM) segment_tree[node] = A[b]; // Store value of this cell
    else                   segment_tree[node] = b;    // If RANGE_MIN/MAX, store index
  }
  else
  {
    int leftIdx = 2 * node,
        rightIdx = 2 * node + 1;

    update(code, A, leftIdx , b              , (b + e) / 2, index);
    update(code, A, rightIdx, (b + e) / 2 + 1, e          , index);

    int lContent = segment_tree[leftIdx],
        rContent = segment_tree[rightIdx];

    if (code == RANGE_SUM) // make this segment contains sum of left and right subtree
      segment_tree[node] = lContent + rContent;
    else // code == RANGE_MIN/MAX
    {
      int lValue = A[lContent],
          rValue = A[rContent];
      if (code == RANGE_MIN) segment_tree[node] = (lValue <= rValue) ? lContent : rContent;
      else                   segment_tree[node] = (lValue >= rValue) ? lContent : rContent;
    }
  }
}

int main()
{
  int A[] = {8, 7, 3, 9, 5, 1, 10};
  init_segment_tree(7);
  build_segment_tree(RANGE_MIN, A, 1, 0, 6);

  printf("%d\n", query(RANGE_MIN, A, 1, 0, 6, 4, 6)); // Answer is index 2

  A[5] = 100;
  update(RANGE_MIN, A, 1, 0, 6, 5);
  printf("%d\n", query(RANGE_MIN, A, 1, 0, 6, 4, 6)); // Answer is index 2

  return 0;
}
