#include<iostream>
using namespace std;
enum Color
{
	Red,
	Black
};
template<class K, class V>
struct RBTreeNode
{
	RBTreeNode<K, V>* _pLeft;
	RBTreeNode<K, V>* _pRight;
	RBTreeNode<K, V>* _pParent;
	K _key;
	V _value;
	Color _color;
	RBTreeNode(const K& key, const V& value)
		:_pLeft(NULL)
		, _pRight(NULL)
		, _pParent(NULL)
		, _key(key)
		, _value(value)
		, _color(Red){}
};
template<class K, class V>
class RBTree
{
public:
	typedef RBTreeNode<K, V> Node;
	typedef Node* PNode;
private:
	PNode _pRoot;
public:
	RBTree()
		:_pRoot(NULL){}
	bool Insert(const K& key, const V& value)
	{
		if (NULL == _pRoot)//根节点为黑色
		{
			_pRoot = new Node(key, value);
			_pRoot->_color = Black;
			return true;
		}
		PNode cur = _pRoot;
		PNode pParent = NULL;
		while (cur)//找插入的位置
		{
			if (cur->_key == key)
				return false;
			else if (cur->_key > key)
			{
				pParent = cur;
				cur = cur->_pLeft;
			}
			else
			{
				pParent = cur;
				cur = cur->_pRight;
			}
		}
		//找到插入的位置以后开始插入
		PNode pNewNode = new Node(key, value);//新插入的结点的颜色默认是红的
		if (key<pParent->_key)
		{
			pParent->_pLeft = pNewNode;
			pNewNode->_pParent = pParent;
		}
		else
		{
			pParent->_pRight = pNewNode;
			pNewNode->_pParent = pParent;
		}
		////当结点插入好了以后我们就要根据红黑树的性质来判断结点是否满足，从而调整结点
		while (pParent)
		{
			if (pParent->_color == Black)//父结点是黑的,不用调整直接退出
			{
				break;
			}
			//记录祖父结点和定义保存叔叔结点
			PNode gparent = pParent->_pParent;
			PNode uncle = NULL;
			if (pParent == gparent->_pLeft)//在其左子节点上
			{
				uncle = gparent->_pRight;
				if (uncle&& uncle->_color == Red)//叔叔结点存在切为红情况3,4
				{
					gparent->_color = Red;
					uncle->_color = pParent->_color = Black;
					pNewNode = gparent;//保存gg结点
					pParent = gparent->_pParent;
					continue;
				}
				else if (uncle == NULL || uncle->_color == Black)//叔叔不存在或者为黑
				{
					if (pNewNode == pParent->_pLeft)//外侧插入
					{
						_RBRolateR(gparent);
						gparent->_color = Red;
						pParent->_color = Black;
					}
					else//内测插入
					{
						_RBRolateL(pParent);
						std::swap(pParent, pNewNode);//交换pParent和插入结点指针的值
						_RBRolateR(gparent);
						gparent->_color = Red;
						pParent->_color = Black;
					}
				}
				break;//直接跳出循环
			}
			else//右边的情况
			{
				uncle = gparent->_pLeft;
				if (uncle&&uncle->_color == Red)//叔叔存在而且为红色
				{
					gparent->_color = Red;
					uncle->_color = pParent->_color = Black;
					pNewNode = gparent;
					pParent = pNewNode->_pParent;
					continue;
				}
				else if (uncle == NULL || uncle->_color == Black)//叔叔不存在或者为黑
				{
					if (pNewNode == pParent->_pRight)
					{
						_RBRolateL(gparent);
						gparent->_color = Red;
						pParent->_color = Black;
					}
					else
					{
						_RBRolateR(pParent);
						std::swap(pParent, pNewNode);
						_RBRolateL(gparent);
						gparent->_color = Red;
						pParent->_color = Black;
					}
				}
				break;
			}
		}
		_pRoot->_color = Black;
		return true;
	}
	void InOrder()
	{
		_InOrder(_pRoot);
	}
	bool IsRBTree()
	{
		if (_pRoot == NULL)//根节点 为空是红黑树
			return true;
		if (_pRoot->_color == Red)//根节点为红色肯定不是红黑树
			return false;
		int count = 0;//统计黑色结点的数目
		PNode cur = _pRoot;
		while (cur)//根出一条参考路径的黑色结点的数目
		{
			if (cur->_color == Black)
				++count;
			cur = cur->_pLeft;
		}
		int k = 0;
		return _IsRBTree(_pRoot, count, k);
	}
private:
	bool _IsRBTree(PNode pRoot, int& count, int k)//这里的K不能传引用
	{
		if (pRoot == NULL)
			return true;
		//出现两个连续的红色的结点
		if (pRoot->_pParent&&pRoot->_color == Red&&pRoot->_pParent->_color == Red)
			return false;
		//如果是黑色结点k++
		if (pRoot->_color == Black)
			k++;
		if (pRoot->_pLeft == NULL&&pRoot->_pRight == NULL)//如果是叶子结点的话进行判断k和count
		{
			if (k == count)
				return true;
			else
				return false;
		}
		return _IsRBTree(pRoot->_pLeft, count, k) && _IsRBTree(pRoot->_pRight, count, k);
	}
	//右单旋转
	void _RBRolateR(PNode parent)
	{
		if (NULL == parent)
			return;
		PNode SubL = parent->_pLeft;//
		PNode SubLR = SubL->_pRight;

		parent->_pLeft = SubLR;
		if (SubLR != NULL)
			SubLR->_pParent = parent;
		PNode pParent = parent->_pParent;
		SubL->_pRight = parent;
		parent->_pParent = SubL;
		if (pParent == NULL)
		{
			_pRoot = SubL;
			SubL->_pParent = NULL;
		}
		else
		{
			if (pParent->_pLeft == parent)
			{
				pParent->_pLeft = SubL;
				SubL->_pParent = parent;
			}
			else
			{
				pParent->_pRight = SubL;
				SubL->_pParent = parent;
			}
		}
	}
	void _RBRolateL(PNode parent)
	{
		if (NULL == parent)
			return;
		PNode SubR = parent->_pRight;
		PNode SubRL = SubR->_pLeft;
		parent->_pRight = SubRL;
		if (SubRL)
		{
			SubRL->_pParent = parent;
		}
		PNode pParent = parent->_pParent;
		SubR->_pLeft = parent;
		parent->_pParent = SubR;
		if (pParent == NULL)
		{
			_pRoot = SubR;
			SubR->_pParent = NULL;
		}
		else
		{
			if (pParent->_pLeft == parent)
			{
				pParent->_pLeft = SubR;
				SubR->_pParent = pParent;
			}
			else
			{
				pParent->_pRight = SubR;
				SubR->_pRight = pParent;
			}
		}
	}
	void _InOrder(PNode pRoot)
	{
		if (pRoot)
		{
			_InOrder(pRoot->_pLeft);
			cout << pRoot->_key << " ";
			_InOrder(pRoot->_pRight);
		}
	}
};
int main()
{
	RBTree<int, int> s;
	int arr[] = { 5, 3, 7, 1, 4, 6, 13, 8, 15, 12 };
	int size = sizeof(arr) / sizeof(*arr);
	for (int i = 0; i < size; i++)
		s.Insert(arr[i], i);
	s.InOrder();
	cout << s.IsRBTree();
	system("pause");
	return 0;
}
