/*求一个数的二进制中1的个数*/

/* 
 * Use the following formula: n & (n-1) to turn off the rightmost 1-bit in a word, producing 0 if none.
 * e.g. 01011000 => 01010000
 */
int numberof1(int n)
{
	int count = 0;
	while (n) {
		count++;
		n = n & (n-1);
	}
	return count;
}

#define ROUND_UP(x)	
int round_up(int x)
{
}
