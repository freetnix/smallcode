/*求一个数的二进制中1的个数*/
/* calculate how many '1' in a specific number in binary */
int numberof1(int n)
{
	int count = 0;
	while (n) {
		count++;
		n = n & (n-1);
	}
	return count;
}
