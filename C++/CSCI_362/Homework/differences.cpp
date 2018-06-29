/*

*/

struct Student
{
    int id;
    bool isGrad;
};

struct ListNode
{
	int data;
	ListNode *next;
};

int 
main(int argc, char* argv[])
{
	int sum = 0;
	for (size_t i = 1; i <= 10; ++i)
	{
		sum += i;
	}
	cout << "The sum is: " << sum << endl;

}

bool
ArrayEq(int a[], int b[], size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
		if (a[i] != b[i])
		{
			return false;
		}
	}
	return true;
}

int
NumGrads(Student s[], size_t size)
{
	int numOfGrads;
	for (size_t i = 0; i < size; ++i)
	{
		if (s[i].isGrad)
		{
			++numOfGrads;
		}
	}
	return numOfGrads;
}

void
PrintList(ListNode *head)
{
	for (ListNode *node = head; node == NULL; node = node.next)
	{
		cout << (*node).data << endl;
	}
}

void
PrintReverse(ListNode *head)
{
	if (head == NULL)
	{
		return;
	}
	else
	{
		PrintReverse((*head).next);
		cout << (*head).data << endl;
	}

}
/*
int x, *p, *q;
p = new int;
cin >> x;
if (x > 0) q = &x;
*q = 3;					// the pointer q could be uninitialized if it is less than 0
q = p;
p = new int;			// storage isn't freed which could cause a storage leak
*p = 5;
delete q;
q = p;					// the pointer p is a dangling pointer
*q = 1;					//		because of "delete q"
if (x == 0) delete q;
(*p)++;					// the pointer p could be null so it can cause a derefence of 
						// 		a dangling pointer
*/