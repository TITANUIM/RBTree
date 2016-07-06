#pragma once

enum Color
{
	RED,
	BLACK,
};

template<class K,class V>
struct RBTreeNode
{
	RBTreeNode<K, V>* _left;
	RBTreeNode<K, V>* _right;
	RBTreeNode<K, V>* _parent;

	K _key;
	V _value;

	Color _col;

	RBTreeNode(const K& key, const V& value)
		: _col(RED)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _key(key)
		, _value(value)
	{}
};

template<class K, class V>
class RBTree
{
	typedef RBTreeNode<K,V> Node;
public:
	RBTree()
		:_root(NULL)
	{}

	bool Insert(const K& key, const V& value)
	{
		if (_root == NULL)  // 没有节点
		{
			_root = new Node(key, value);
			_root->_col = BLACK;
			return true;
		}

		Node* parent = NULL;
		Node* cur = _root;

		while (cur)
		{
			if (cur->_key > key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_key < key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return false;
			}
		}

		if (parent->_key > key)
		{
			cur = new Node(key, value);
			parent->_left = cur;
			cur->_parent = parent;
		}
		else
		{
			cur = new Node(key, value);
			parent->_right = cur;
			cur->_parent = parent;
		}

		//cur为红，parent为红，g为黑，u存在且为红
		while (cur != _root && parent->_col == RED)
		{
			Node* grandfather = parent->_parent;
			if (parent == grandfather->_left)
			{
				Node* uncle = grandfather->_right;
				if (uncle && uncle->_col == RED)
				{
					uncle->_col = BLACK;
					parent->_col = BLACK;
					grandfather->_col = RED;

					//继续向上调整
					cur = grandfather;
					parent = cur->_parent;
				}
				else // uncle不存在或uncle为黑
				{
					/*if (parent->_left == cur)
					{
						parent->_col = BLACK;
						grandfather->_col = RED;

						_RotateR(grandfather);
					}
					else
					{
						parent->_col = BLACK;
						grandfather->_col = RED;

						_RtateL(parent);
					}*/

					if (parent->_right == cur)
					{
						_RotateL(parent);
					}

					//由于没有右子树，需进行右旋调整
					parent->_col = BLACK;
					grandfather->_col = RED;

					_RotateR(grandfather);

					break;
				}
			}
			else
			{
				Node* uncle = grandfather->_left;

				if (uncle && uncle->_col == RED)
				{
					uncle->_col = BLACK;
					parent->_col = BLACK;
					grandfather->_col = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else
				{
					if (parent->_left == cur)
					{
						_RotateR(parent);
					}

					parent->_col = BLACK;
					grandfather->_col = RED;
					_RotateL(grandfather);

					break;
				}
			}
		}

		_root->_col = BLACK; //插入结束，将根节点置黑
	}

	//中序遍历
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	//检查是否满足红黑树规则
	bool Check()
	{
		int blackCount = 0; // 记录最左路黑色节点的个数
		int blackCurCount = 0; // 记录当前路径黑色节点的个数

		Node* cur = _root;

		while (cur)
		{
			if (cur->_col == BLACK)
			{
				++blackCount;
			}
			cur = cur->_left;
		}

		return _Check(_root, blackCount, blackCurCount);
	}


protected:
	bool _Check(Node* root, int blackCount, int blackCurCount)
	{
		if (root == NULL)
		{
			return true; // 没有节点时，满足规则
		}

		if (root->_col == BLACK)
		{
			++blackCurCount;
		}
		else
		{
			if (root->_parent && root->_parent->_col == RED)
			{
				cout << "No blance！Root can't be red!" << root->_key << endl;
				return false;
			}
		}

		if (root->_left == NULL && root->_right == NULL)
		{
			if (blackCount == blackCurCount)
			{
				return true;
			}
			else
			{
				cout << "No blance! Black is unequal!" << endl;
				return false;
			}
		}

		return _Check(root->_left, blackCount, blackCurCount) && _Check(root->_right, blackCount, blackCurCount);
	}


	void _InOrder(Node* root)
	{
		if (root == NULL)
		{
			return;
		}

		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}

	//左单旋调整
	void _RotateL(Node*& parent) 
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if (subRL)
		{
			subRL->_parent = parent;
		}
		subR->_left = parent;
		subR->_parent = parent->_parent;
		parent->_parent = subR;

		parent = subR;
		if (parent->_parent == NULL)
		{
			_root = parent;
		}
		else
		{
			Node* ppNode = parent->_parent;
			if (ppNode->_key > parent->_key)
			{
				ppNode->_left = parent;
			}
			else
			{
				ppNode->_right = parent;
			}
		}
	}

	//右单旋调整
	void _RotateR(Node*& parent) //注意传过来的parent是grandfather节点
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		if (subLR)
		{
			subLR->_parent = parent;
		}
		subL->_right = parent;
		subL->_parent = parent->_parent;

		parent->_parent = subL;

		parent = subL;

		if (parent->_parent == NULL)
		{
			_root = parent;
		}
		else
		{
			Node* ppNode = parent->_parent;
			if (ppNode->_key > parent->_key)
			{
				ppNode->_left = parent;
			}
			else
			{
				ppNode->_right = parent;
			}
		}
	}

protected:
	Node* _root;
};

void TestRBTree()
{
	int a[] = { 1, 4, 6, 8, 0, 2, 3, 5, 9, 7 };
	RBTree<int, int> rb;

	for (size_t i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
	{
		rb.Insert(a[i], i);
	}

	rb.InOrder();
	cout<< rb.Check() <<endl;
}
