void rotateSuperman (int from, int mid, int to)
{
    if (from==mid || mid==to) return;
    int n = gcd(to - from, mid - from);
    bool isfirst=1;
    int FirstRoundVectorFactor = 0;
    while (n-- != 0) 
    {
      int VectorFactor = ( n == 0 ) ? 1 : 2; 
      if (isfirst)
      {
        FirstRoundVectorFactor = VectorFactor;
        isfirst = false;
      }

      if (VectorFactor == 2) //swap 2
      {
        int val_0 = nums[from+n];
        int val_1 = nums[from+n];
        int shift = mid - from;
        int p1_0 = from+n; 
        int p1_1 = from+n+1;
        int p2_0 = from+n+shift;
        int p2_1 = from+n+shift+1;
        while (p2_0 != from + n) 
        {
            nums[p1_0] = nums[p2_0];
            nums[p1_1] = nums[p2_1];
            p1_0=p2_0;
            p1_1=p2_1;
            if ( to - p2_0 > shift) p2_0 += shift;
            else p2_0=from + (shift - (to - p2_0));
            if ( to - p2_1 > shift) p2_1 += shift;
            else p2_1=from + (shift - (to - p2_1));
        }
        nums[p1_0] = val_0;
        nums[p1_1] = val_1;
      }
      if (VectorFactor == 1) //swap 1
      {
        int val_0 = nums[from+n];
        int shift = mid - from;
        int p1_0 = from+n;
        int p2_0 = from+n+shift;
        while (p2_0 != from + n) 
        {
            nums[p1_0] = nums[p2_0];
            p1_0=p2_0;
            if ( to - p2_0 > shift) p2_0 += shift;
            else p2_0=from + (shift - (to - p2_0));
        }
        nums[p1_0] = val_0;
      }
    }
}

